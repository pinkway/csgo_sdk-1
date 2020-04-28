#pragma once

class c_event_info {
public:
	char			pad0[4];
	float			m_delay;
	char			pad1[4];
	client_class*	m_client_class;
	char			pad2[40];
	c_event_info*	m_next;
};

class c_client_state {
	class c_clock_drift_mgr {
	public:
		float			m_clock_offsets[16];
		uint32_t		m_cur_clock_offset;
		uint32_t		m_server_tick;
		uint32_t		m_client_tick;
	};
public:
	char				pad0[156];
	i_net_channel*		m_net_channel;
	uint32_t			m_challenge_nr;
	char				pad1[100];
	uint32_t			m_signon_state;
	char				pad2[8];
	float				m_next_cmd_time;
	uint32_t			m_server_count;
	uint32_t			m_cur_sequence;
	char				pad3[8];
	c_clock_drift_mgr	m_clock_drift_mgr;
	uint32_t			m_delta_tick;
	bool				m_paused;
	char				pad4[3];
	uint32_t			m_view_entity;
	uint32_t			m_player_slot;
	char				m_level_name[260];
	char				m_level_name_short[80];
	char				m_group_name[80];
	char				pad5[92];
	uint32_t			m_max_clients;
	char				pad6[18824];
	float				m_last_server_tick_time;
	bool				m_in_simulation;
	char				pad7[3];
	uint32_t			m_old_tickcount;
	float				m_tick_remainder;
	float				m_frame_time;
	int					m_last_outgoing_command;
	int					m_choked_commands;
	int					m_last_command_ack;
	int					m_command_ack;
	int					m_sound_sequence;
	char				pad8[80];
	qangle_t			m_viewangles;
	char				pad9[208];
	c_event_info*		m_events;
};