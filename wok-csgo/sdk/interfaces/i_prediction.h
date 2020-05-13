#pragma once

class c_move_data {
public:
	bool		m_first_run_of_functions : 1;
	bool		m_game_code_moved_player : 1;
	int			m_player_handle;
	int			m_impulse_cmd;
	qangle_t	m_viewangles;
	qangle_t	m_abs_viewangles;
	int			m_buttons;
	int			m_old_buttons;
	vec3_t		m_move;
	float		m_max_speed;
	float		m_client_max_speed;
	vec3_t		m_velocity;
	qangle_t	m_angles;
	qangle_t	m_old_angles;
	float		m_out_step_height;
	vec3_t		m_out_wish_velocity;
	vec3_t		m_out_jump_velocity;
	vec3_t		m_constraint_center;
	float		m_constraint_radius;
	float		m_constraint_width;
	float		m_constraint_speed_factor;
	float		pad0[5];
	vec3_t		m_abs_origin;
};


class c_base_player;

class i_prediction {
public:
	VFUNC(update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command), 3, void(__thiscall*)(void*, int, bool, int, int), startframe, validframe, incoming_acknowledged, outgoing_command)
	VFUNC(in_prediction(), 14, bool(__thiscall*)(void*))
	VFUNC(check_moving_on_ground(c_base_player* player, float frame_time), 18, void(__thiscall*)(void*, c_base_player*, double), player, frame_time)
	VFUNC(run_command(c_base_player* player, c_user_cmd* cmd, i_move_helper* move_helper), 19, void(__thiscall*)(void*, c_base_player*, c_user_cmd *, i_move_helper*), player, cmd, move_helper)
	VFUNC(setup_move(c_base_player* player, c_user_cmd* cmd, i_move_helper* move_helper, c_move_data* move_data), 20, void(__thiscall*)(void*, c_base_player*, c_user_cmd*, i_move_helper*, c_move_data*), player, cmd, move_helper, move_data)
	VFUNC(finish_move(c_base_player* player, c_user_cmd* cmd, c_move_data* move_data), 21, void(__thiscall*)(void*, c_base_player*, c_user_cmd*, c_move_data*), player, cmd, move_data)

	char pad0[8];
	bool m_in_prediction;
	char pad1[1];
	bool m_engine_paused;
	char pad2[13];
	bool m_first_time_predicted;
	char pad3[7];
	int	 m_server_commands_acknowledged;
	bool m_had_prediction_errors;
};