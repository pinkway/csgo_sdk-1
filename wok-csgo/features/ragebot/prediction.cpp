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
	m_backup.store();

	interfaces::global_vars->m_cur_time = TICKS_TO_TIME(player->get_tick_base());
	interfaces::global_vars->m_frame_time = interfaces::prediction->m_engine_paused ? 0.f : interfaces::global_vars->m_interval_per_tick;

	interfaces::prediction->m_in_prediction = true;
	interfaces::prediction->m_first_time_predicted = false;

	static const auto md5_pseudo_random_fn = SIG("client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A").cast<uint32_t(__thiscall*)(uint32_t)>();

	cmd->m_random_seed = md5_pseudo_random_fn(cmd->m_command_number) & 0x7FFFFFFF;
	
	player->get_cur_cmd() = cmd;
	player->get_last_cmd() = *cmd;

	*m_player = reinterpret_cast<int>(player);
	*m_random_seed = cmd->m_random_seed;

	cmd->m_buttons.add(player->get_buttons_forced());
	cmd->m_buttons.remove(~player->get_buttons_disabled());

	interfaces::move_helper->set_host(player);
	interfaces::game_movement->start_track_prediction_errors(player);

	if (cmd->m_weapon_select) {
		if (const auto weapon = reinterpret_cast<c_base_combat_weapon*>(interfaces::entity_list->get_client_entity(cmd->m_weapon_select))) {
			if (const auto weapon_data = weapon->get_cs_weapon_data()) {
				player->select_item(weapon_data->m_weapon_name, cmd->m_weapon_sub_type);
			}
		}
	}

	const auto vehicle = reinterpret_cast<c_base_entity*>(player->get_vehicle().get());

	if (cmd->m_impulse
		&& (!vehicle || player->using_standard_weapons_in_vehicle())) {
		player->get_impulse() = cmd->m_impulse;
	}

	const auto buttons = cmd->m_buttons;
	const auto buttons_changed = buttons ^ player->get_buttons();

	player->get_buttons_last() = player->get_buttons();
	player->get_buttons() = buttons;
	player->get_buttons_pressed() = buttons_changed & buttons;
	player->get_buttons_released() = buttons_changed & ~buttons;

	interfaces::prediction->check_moving_on_ground(player, interfaces::global_vars->m_frame_time);

	player->set_local_view_angles(cmd->m_view_angles);

	if (player->physics_run_think(0)) {
		player->pre_think();
	}

	if (player->get_next_think_tick()
		&& player->get_next_think_tick() != -1
		&& player->get_next_think_tick() <= player->get_tick_base()) {
		player->get_next_think_tick() = -1;

		player->unknown_think(0);

		player->think();
	}

	interfaces::prediction->setup_move(player, cmd, interfaces::move_helper, m_move_data);

	vehicle
		? memory::get_vfunc<void(__thiscall*)(c_base_entity*, c_cs_player*, c_move_data*)>(vehicle, 5)(vehicle, player, m_move_data)
		: interfaces::game_movement->process_movement(player, m_move_data);

	interfaces::prediction->finish_move(player, cmd, m_move_data);

	interfaces::move_helper->process_impacts();

	{
		interfaces::model_cache->lock();

		if (player->is_alive()
			|| player->is_ghost()) {
			player->update_collistion_bounds();

			if (player->get_flags().has(FL_ONGROUND)) {
				player->get_fall_velocity() = 0.f;
			}

			if (player->get_sequence() == -1) {
				player->set_sequence(0);
			}

			player->studio_frame_advance();

			player->post_think_v_physics();
		}

		player->simulate_player_simulated_entities();

		interfaces::model_cache->unlock();
	}
}

void c_prediction::end(c_cs_player* player, c_user_cmd* cmd) {
	interfaces::game_movement->finish_track_prediction_errors(player);
	interfaces::move_helper->set_host(nullptr);
	interfaces::game_movement->reset();

	player->get_cur_cmd() = nullptr;

	*m_random_seed = -1;
	*m_player = 0;

	if (!interfaces::prediction->m_engine_paused 
		&& interfaces::global_vars->m_frame_time > 0.f) {
		player->get_tick_base()++;
	}

	m_backup.restore();
}
