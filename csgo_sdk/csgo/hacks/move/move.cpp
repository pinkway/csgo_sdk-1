#include "../../csgo.hpp"

namespace csgo::hacks {
    void c_move::handle( valve::user_cmd_t& cmd ) const {
        if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) )
            cmd.m_buttons &= ~valve::e_buttons::in_jump;
    }

    void c_move::rotate( valve::user_cmd_t& cmd, const sdk::qang_t& wish_angles ) const {
        const auto local_player = g_local_player->self( );

        const auto speed_2d = cmd.m_move.length( 2u );
        if ( speed_2d == 0.f )
           return;

        if ( cmd.m_view_angles.z( ) != 0.f
            && !( local_player->flags( ) & valve::e_ent_flags::on_ground ) )
            cmd.m_move.y( ) = 0.f;

        const auto delta = cmd.m_view_angles.y( ) - wish_angles.y( );

        const auto ang = sdk::to_rad( std::remainder(
            delta + sdk::to_deg( std::atan2( cmd.m_move.y( ) / speed_2d, cmd.m_move.x( ) / speed_2d ) ),
            360.f ) );

        auto move_2d = sdk::vec2_t( std::cos( ang ), std::sin( ang ) )
            * speed_2d * std::cos(
                sdk::to_rad( std::remainder( sdk::to_deg( std::atan2( 0.f, speed_2d ) ), 360.f ) )
            );

        const auto on_ladder = local_player->move_type( ) == valve::e_move_type::ladder;

        if ( on_ladder ) {
            if ( wish_angles.x( ) < 45.f && cmd.m_view_angles.x( ) >= 45.f
                && std::abs( delta ) <= 65.f )
                move_2d.x( ) *= -1.f;
        }
        else if ( std::abs( cmd.m_view_angles.x( ) ) > 90.f )
            move_2d.x( ) *= -1.f;

        cmd.m_move.x( ) = move_2d.x( );
        cmd.m_move.y( ) = move_2d.y( );

        cmd.m_buttons &= ~( valve::e_buttons::in_fwd | valve::e_buttons::in_back
            | valve::e_buttons::in_move_right | valve::e_buttons::in_move_left );

        const auto in_x_move = on_ladder ? std::abs( cmd.m_move.x( ) ) > 200.f
           : cmd.m_move.x( ) != 0.f;
        const auto in_y_move = on_ladder ? std::abs( cmd.m_move.y( ) ) > 200.f
           : cmd.m_move.y( ) != 0.f;

        if ( in_x_move )
            cmd.m_buttons |= ( cmd.m_move.x( ) > 0.f ? valve::e_buttons::in_fwd
               : valve::e_buttons::in_back );

        if ( in_y_move )
            cmd.m_buttons |= ( cmd.m_move.y( ) > 0.f ? valve::e_buttons::in_move_right
               : valve::e_buttons::in_move_left );
    }
}