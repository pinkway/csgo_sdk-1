#include "../../csgo.hpp"

namespace csgo::hacks {
    void c_move::handle( valve::user_cmd_t& cmd ) const {
        if ( !( g_local_player->self( )->flags( ) & valve::e_ent_flags::on_ground ) )
            cmd.m_buttons &= ~valve::e_buttons::in_jump;
    }

    void c_move::rotate( valve::user_cmd_t& cmd, const sdk::qang_t& wish_angles ) const {

    }
}