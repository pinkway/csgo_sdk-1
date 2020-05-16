#pragma once

class c_user_cmd {
public:
	virtual		~c_user_cmd() = default;

	int			m_commandnumber;
	int			m_tickcount;
	qangle_t	m_viewangles;
	vec3_t		m_aimdirection;
	vec3_t		m_move;
	int			m_buttons;
	char		m_impulse;
	int			m_weaponselect;
	int			m_weaponsubtype;
	int			m_random_seed;
	short		m_mousedx;
	short		m_mousedy;
	bool		m_predicted;
	char		pad0[24];
};

class c_verified_user_cmd {
public:
	c_user_cmd  m_cmd;
	uint32_t	m_crc;
};

class c_input {
public:
	char					pad0[12];
	bool					m_trackir_available;
	bool					m_mouse_initiated;
	bool					m_mouse_active;
	bool					m_joystick_advanced_init;
	char					pad1[44];
	void*					m_keys;
	char					pad3[108];
	bool					m_camera_intercepting_mouse;
	bool					m_camera_in_thirdperson;
	bool					m_camera_moving_with_mouse;
	vec3_t					m_camera_offset;
	bool					m_camera_distance_move;
	vec2_t					m_camera_old;
	vec2_t					m_camera;
	bool					m_camera_is_orthographic;
	vec3_t					m_previous_viewangles;
	vec3_t					m_previous_viewangles_tilt;
	float					m_last_forward_move;
	int						m_clear_input_state;
	char					pad4[8];
	c_user_cmd*				m_commands;
	c_verified_user_cmd*	m_verified_commands;

	VFUNC(get_user_cmd(int sequence_number), 8, c_user_cmd*(__thiscall*)(void*, int, int), 0, sequence_number)
	VFUNC(get_user_cmd(int slot, int sequence_number), 8, c_user_cmd*(__thiscall*)(void*, int, int), slot, sequence_number)

	c_verified_user_cmd* get_verified_cmd(int sequence_number) {
		auto verified_commands = *reinterpret_cast<c_verified_user_cmd**>(reinterpret_cast<uintptr_t>(this) + 0xF8);
		return &verified_commands[sequence_number % MULTIPLAYER_BACKUP];
	}
};