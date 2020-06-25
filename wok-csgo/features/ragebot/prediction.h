#pragma once
#include "../../globals.h"

class c_prediction : public c_singleton<c_prediction> {
private:
	struct {
		float m_cur_time, m_frame_time;
		bool m_in_prediction, m_first_time_predicted;
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

			static const auto post_think_v_physics_fn = SIG("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB").cast<bool(__thiscall*)(void*)>();
			post_think_v_physics_fn(player);
		}

		static const auto simulate_player_simulated_entities_fn = SIG("client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72").cast<bool(__thiscall*)(void*)>();
		simulate_player_simulated_entities_fn(player);

		return memory::get_vfunc<int(__thiscall*)(void*)>(interfaces::model_cache, 34)(interfaces::model_cache);
	}
public:
	c_prediction() {
		m_move_data = malloc(182u);
		m_prediction_player = *SIG("client.dll", "89 35 ? ? ? ? F3 0F 10 48").self_offset(0x2).cast<int**>();
		m_prediction_random_seed = *SIG("client.dll", "A3 ? ? ? ? 66 0F 6E 86").self_offset(0x1).cast<int**>();
	}

	void pre_start();
	void start(c_cs_player* player, c_user_cmd* cmd);
	void end(c_cs_player* player, c_user_cmd* cmd);
};
#define engine_prediction c_prediction::instance()