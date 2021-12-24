#pragma once

namespace csgo::valve {
	ALWAYS_INLINE void user_cmd_t::clamp( ) {
		constexpr auto lims = sdk::qang_t( 89.f, 180.f, 50.f );

		for ( auto i = 0u; i < m_view_angles.size( ); ++i )
			m_view_angles.at( i ) = std::clamp( m_view_angles.at( i ), -lims.at( i ), lims.at( i ) );
	}
}