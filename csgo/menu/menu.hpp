#pragma once

class c_menu {
private:
    struct {
        struct {
            sdk::cfg_var_t< bool > m_bhop{ HASH( "move.bhop" ), false };
        } m_move{};

        bool m_hidden{};
    } m_main{};
public:
    void render( );

    ALWAYS_INLINE auto& main( );
};

inline const auto g_menu = std::make_unique< c_menu >( );

#include "impl/menu.inl"