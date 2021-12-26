#pragma once

namespace csgo {
	namespace valve { struct cvar_t; }

	class c_ctx {
	private:
		struct {

		} m_offsets{};

		struct {
			valve::cvar_t*	m_cl_forwardspeed{},
							*m_cl_sidespeed{}, *m_cl_upspeed{},
							*m_cl_pitchdown{}, *m_cl_pitchup{};
		} m_cvars{};
	public:
		void init( );

		ALWAYS_INLINE const auto& offsets( ) const;

		ALWAYS_INLINE const auto& cvars( ) const;
	};

	inline const auto g_ctx = std::make_unique< c_ctx >( );
}

#include "impl/ctx.inl"