#pragma once

namespace csgo::valve {
	inline constexpr auto k_mp_backup = 150u;

	enum struct e_buttons : int {
		in_attack = 1 << 0,
		in_jump = 1 << 1,
		in_duck = 1 << 2,
		in_forward = 1 << 3,
		in_back = 1 << 4,
		in_use = 1 << 5,
		in_cancel = 1 << 6,
		in_left = 1 << 7,
		in_right = 1 << 8,
		in_move_left = 1 << 9,
		in_move_right = 1 << 10,
		in_attack2 = 1 << 11,
		in_run = 1 << 12,
		in_reload = 1 << 13,
		in_alt1 = 1 << 14,
		in_alt2 = 1 << 15,
		in_score = 1 << 16,
		in_speed = 1 << 17,
		in_walk = 1 << 18,
		in_zoom = 1 << 19,
		in_weapon1 = 1 << 20,
		in_weapon2 = 1 << 21,
		in_bullrush = 1 << 22,
		in_grenade1 = 1 << 23,
		in_grenade2 = 1 << 24,
		in_attack3 = 1 << 25
	};
	ENUM_BIT_OPERATORS( e_buttons, true );

	struct user_cmd_t {
		ALWAYS_INLINE void sanitize( );

		std::uint8_t	pad0[ 4u ]{};
		int				m_number{}, m_tick{};
		sdk::qang_t		m_view_angles{};
		sdk::vec3_t		m_aim_dir{}, m_move{};
		e_buttons		m_buttons{};
		std::uint8_t	m_impulse{};
		int				m_weapon_select{},
						m_weapon_sub_type{},
						m_random_seed{};
		sdk::s16vec2_t	m_mouse_accum{};
		bool			m_predicted{};
		sdk::qang_t		m_head_angles{};
		sdk::vec3_t		m_head_offset{};
	};

	struct cvar_t {
		using callback_t = void( __cdecl* )( );

		using change_callback_t = void( __cdecl* )( void*, const char* const, const float );

		VFUNC( const char*( __thiscall* )( decltype( this ) ), get_str( ), 11u );

		VFUNC( float( __thiscall* )( decltype( this ) ), get_float( ), 12u );

		VFUNC( int( __thiscall* )( decltype( this ) ), get_int( ), 13u );

		VFUNC( void( __thiscall* )( decltype( this ), const char* ), set_str( const char* value ), 14u, value );

		VFUNC( void( __thiscall* )( decltype( this ), float ), set_float( const float value ), 15u, value );

		VFUNC( void( __thiscall* )( decltype( this ), int ), set_int( const int value ), 16u, value );

		std::uint8_t					pad0[ 4u ]{};

		cvar_t*							m_next{};
		bool							m_registered{};

		const char*						m_name{}, *m_help{};
		int								m_flags{};

		callback_t						m_callback{};
		cvar_t*							m_parent{};

		const char*						m_def_value{}, *m_str{};
		int								m_str_len{};

		float							m_float{};
		int								m_int{};

		bool							m_has_min{};
		float							m_min{};

		bool							m_has_max{};
		float							m_max{};

		utl_vec_t< change_callback_t >	m_change_callbacks{};
	};
}

#include "impl/other.inl"