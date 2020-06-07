#include "../features.h"

/*
	fully reversed run command -____-
	u shouldnt predict like this........
*/

void c_prediction::pre_start() {
	interfaces::prediction->update(interfaces::client_state->m_delta_tick, interfaces::client_state->m_delta_tick > 0, interfaces::client_state->m_last_command_ack,
		interfaces::client_state->m_last_outgoing_command + interfaces::client_state->m_choked_commands);
}

void c_prediction::start(c_cs_player* player, c_user_cmd* cmd) {
	if (!player)
		return;

	m_backup.m_curtime = interfaces::global_vars->m_curtime;
	m_backup.m_frametime = interfaces::global_vars->m_frametime;

	m_backup.m_in_prediction = interfaces::prediction->m_in_prediction;
	m_backup.m_first_time_predicted = interfaces::prediction->m_first_time_predicted;

	interfaces::global_vars->m_curtime = TICKS_TO_TIME(player->get_tickbase());
	interfaces::global_vars->m_frametime = interfaces::prediction->m_engine_paused ? 0.f : interfaces::global_vars->m_interval_per_tick;

	interfaces::prediction->m_in_prediction = true;
	interfaces::prediction->m_first_time_predicted = false;

	player->get_cur_cmd() = cmd;
	*reinterpret_cast<c_user_cmd**>(reinterpret_cast<uintptr_t>(player) + 0x3288) = cmd;

	static const auto md5_pseudo_random_fn = reinterpret_cast<uint32_t(__thiscall*)(uint32_t)>(SIG("client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A"));

	*m_prediction_player = reinterpret_cast<int>(player);
	*m_prediction_random_seed = md5_pseudo_random_fn(cmd->m_commandnumber) & 0x7FFFFFFF;

	auto buttons_forced = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(player) + 0x3334);
	auto buttons_disabled = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(player) + 0x3330);

	cmd->m_buttons |= buttons_forced;
	cmd->m_buttons &= ~buttons_disabled;

	interfaces::move_helper->set_host(player);
	interfaces::game_movement->start_track_prediction_errors(player);

	if (cmd->m_weaponselect) {
		auto weapon = reinterpret_cast<c_base_combat_weapon*>(interfaces::entity_list->get_client_entity(cmd->m_weaponselect));
		if (weapon) {
			auto weapon_data = weapon->get_cs_weapon_data();
			weapon_data ? player->select_item(weapon_data->m_weapon_name, cmd->m_weaponsubtype) : 0;
		}
	}

	auto vehicle_handle = player->get_vehicle();
	auto vehicle = vehicle_handle.is_valid() ? reinterpret_cast<c_base_entity*>(vehicle_handle.get()) : nullptr;
	
	if (cmd->m_impulse 
		&& (!vehicle || player->using_standard_weapons_in_vehicle()))
		player->get_impulse() = cmd->m_impulse;

	auto buttons = cmd->m_buttons;
	auto buttons_changed = buttons ^ player->get_buttons();

	player->get_buttons_last() = player->get_buttons();
	player->get_buttons() = buttons;
	player->get_buttons_pressed() = buttons_changed & buttons;
	player->get_buttons_released() = buttons_changed & ~buttons;

	interfaces::prediction->check_moving_on_ground(player, interfaces::global_vars->m_frametime);

	player->set_local_view_angles(cmd->m_viewangles);

	player->physics_run_think(0) ? player->pre_think() : 0;

	if (player->get_next_think_tick()
		&& player->get_next_think_tick() != -1
		&& player->get_next_think_tick() <= player->get_tickbase()) {
		player->get_next_think_tick() = -1;
		player->unknown_think(0);
		player->think();
	}

	interfaces::prediction->setup_move(player, cmd, interfaces::move_helper, reinterpret_cast<c_move_data*>(m_move_data));

	vehicle
		? memory::get_vfunc<void(__thiscall*)(c_base_entity*, c_cs_player*, c_move_data*)>(vehicle, 5)(vehicle, player, reinterpret_cast<c_move_data*>(m_move_data))
		: interfaces::game_movement->process_movement(player, reinterpret_cast<c_move_data*>(m_move_data));

	interfaces::prediction->finish_move(player, cmd, reinterpret_cast<c_move_data*>(m_move_data));

	interfaces::move_helper->process_impacts();

	post_think(player);

	interfaces::prediction->m_in_prediction = m_backup.m_in_prediction;
	interfaces::prediction->m_first_time_predicted = m_backup.m_first_time_predicted;
}

void c_prediction::end(c_cs_player* player, c_user_cmd* cmd) {
	if (!player)
		return;

	interfaces::game_movement->finish_track_prediction_errors(player);
	interfaces::move_helper->set_host(nullptr);
	interfaces::game_movement->reset();

	player->get_cur_cmd() = nullptr;
	*m_prediction_random_seed = -1;
	*m_prediction_player = 0;

	!interfaces::prediction->m_engine_paused && interfaces::global_vars->m_frametime ? player->get_tickbase()++ : 0;

	interfaces::global_vars->m_curtime = m_backup.m_curtime;
	interfaces::global_vars->m_frametime = m_backup.m_frametime;
}