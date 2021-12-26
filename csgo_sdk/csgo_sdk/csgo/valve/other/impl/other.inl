#pragma once

namespace csgo::valve {
    ALWAYS_INLINE void user_cmd_t::sanitize( ) {
        m_view_angles.normalize( );

        const auto& cvars = g_ctx->cvars( );

        constexpr auto k_max_roll = 50.f;

        m_view_angles.x( ) = std::clamp( m_view_angles.x( ), -cvars.m_cl_pitchup->get_float( ), cvars.m_cl_pitchdown->get_float( ) );
        m_view_angles.z( ) = std::clamp( m_view_angles.z( ), -k_max_roll, k_max_roll );

        const auto max_fwd_speed = cvars.m_cl_forwardspeed->get_float( );
        const auto max_side_speed = cvars.m_cl_sidespeed->get_float( );
        const auto max_up_speed = cvars.m_cl_upspeed->get_float( );

        m_move = {
            std::clamp( m_move.x( ), -max_fwd_speed, max_fwd_speed ),
            std::clamp( m_move.y( ), -max_side_speed, max_side_speed ),
            std::clamp( m_move.z( ), -max_up_speed, max_up_speed )
        };
    }
}