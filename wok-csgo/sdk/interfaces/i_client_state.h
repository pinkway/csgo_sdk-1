#pragma once

struct event_info_t {
	int				m_class_id;
	float			m_delay;
	char			pad1[4];
	c_client_class*	m_client_class;
	char			pad2[40];
	event_info_t*	m_next;
};

class i_client_state {
public:
	char				pad0[156];
	i_net_channel*		m_net_channel;
	int					m_challenge_nr;
	char				pad1[100];
	int					m_signon_state;
	char				pad2[8];
	float				m_next_cmd_time;
	int					m_server_count;
	int					m_cur_sequence;
	char				pad3[8];

	struct {
		float			m_clock_offsets[16];
		int				m_cur_clock_offset;
		int				m_server_tick;
		int				m_client_tick;
	} m_clock_drift_mgr;

	int					m_delta_tick;
	bool				m_paused;
	char				pad4[3];
	int					m_view_entity;
	int					m_player_slot;
	char				m_level_name[260];
	char				m_level_name_short[80];
	char				m_group_name[80];
	char				pad5[92];
	int					m_max_clients;
	char				pad6[18824];
	float				m_last_server_tick_time;
	bool				m_in_simulation;
	char				pad7[3];
	int					m_old_tickcount;
	float				m_tick_remainder;
	float				m_frame_time;
	int					m_last_outgoing_command;
	int					m_choked_commands;
	int					m_last_command_ack;
	int					m_command_ack;
	int					m_sound_sequence;
	char				pad8[80];
	qangle_t			m_view_angles;
	char				pad9[208];
	event_info_t*		m_events;
};