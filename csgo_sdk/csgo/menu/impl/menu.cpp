#include "../../csgo.hpp"

namespace csgo {
    void c_menu::render( ) {
        if ( m_main.m_hidden )
            return;

        ImGui::Begin( "wok sdk" );

        ImGui::Checkbox( "bhop", &m_main.m_move.m_bhop );

        if ( ImGui::Button( "load" ) )
            sdk::g_cfg->load( "example" );

        if ( ImGui::Button( "save" ) )
            sdk::g_cfg->save( "example" );

        ImGui::End( );
    }
}