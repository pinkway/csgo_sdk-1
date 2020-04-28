#pragma once
#include "../../globals.h"

class c_prediction : public c_singleton<c_prediction> {
private:
	struct {
		float curtime, frametime;
		bool in_prediction, first_time_predicted;
	} m_backup;

	void* m_move_data;
	int* m_prediction_player;
	int* m_prediction_random_seed;

	int post_think(c_cs_player* player) {
		memory::get_vfunc<void(__thiscall*)(void*)>(interfaces::model_cache, 33)(interfaces::model_cache);

		if (player->is_alive()
			|| *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(player) + 0x3A81)) {
			memory::get_vfunc<void(__thiscall*)(void*)>(player, 339)(player);

			player->get_flags() & FL_ONGROUND ? player->get_fall_velocity() = 0.f : 0;

			player->get_sequence() == -1 ? memory::get_vfunc<void(__thiscall*)(void*, int)>(player, 218)(player, 0) : 0;

			memory::get_vfunc<void(__thiscall*)(void*)>(player, 219)(player);

			static const auto post_think_v_physics_fn = reinterpret_cast<bool(__thiscall*)(void*)>(SIG("client_panorama.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB"));
			post_think_v_physics_fn(player);
		}

		static const auto simulate_player_simulated_entities_fn = reinterpret_cast<bool(__thiscall*)(void*)>(SIG("client_panorama.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72"));
		simulate_player_simulated_entities_fn(player);

		return memory::get_vfunc<int(__thiscall*)(void*)>(interfaces::model_cache, 34)(interfaces::model_cache);
	}
public:
	c_prediction() {
		m_move_data = malloc(182u);
		m_prediction_player = *reinterpret_cast<int**>(SIG("client_panorama.dll", "89 35 ? ? ? ? F3 0F 10 48") + 0x2);
		m_prediction_random_seed = *reinterpret_cast<int**>(SIG("client_panorama.dll", "A3 ? ? ? ? 66 0F 6E 86") + 0x1);
	}

	void pre_start();
	void start(c_cs_player* player, c_user_cmd* cmd);
	void end(c_cs_player* player, c_user_cmd* cmd);
};