#pragma once
#include "../../globals.h"

class c_prediction : public c_singleton<c_prediction> {
private:
	struct {
		__forceinline void store() {
			m_cur_time = interfaces::global_vars->m_cur_time;
			m_frame_time = interfaces::global_vars->m_frame_time;

			m_in_prediction = interfaces::prediction->m_in_prediction;
			m_first_time_predicted = interfaces::prediction->m_first_time_predicted;
		}

		__forceinline void restore() {
			interfaces::global_vars->m_cur_time = m_cur_time;
			interfaces::global_vars->m_frame_time = m_frame_time;

			interfaces::prediction->m_in_prediction = m_in_prediction;
			interfaces::prediction->m_first_time_predicted = m_first_time_predicted;
		}

		float m_frame_time, m_cur_time;
		bool m_first_time_predicted, m_in_prediction;
	} m_backup;

	c_move_data* m_move_data;

	int* m_player;
	int* m_random_seed;
public:
	c_prediction() {
		m_player = *SIG("client.dll", "89 35 ? ? ? ? F3 0F 10 48").self_offset(0x2).cast<int**>();
		m_random_seed = *SIG("client.dll", "A3 ? ? ? ? 66 0F 6E 86").self_offset(0x1).cast<int**>();

		m_move_data = reinterpret_cast<c_move_data*>(interfaces::mem_alloc->alloc(sizeof(c_move_data)));
	}

	void pre_start();
	void start(c_cs_player* player, c_user_cmd* cmd);
	void end(c_cs_player* player, c_user_cmd* cmd);
};
#define engine_prediction c_prediction::instance()
