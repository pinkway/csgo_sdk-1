#pragma once

#include "../ctx.hpp"

namespace csgo {
    ALWAYS_INLINE const auto& c_ctx::offsets( ) const { return m_offsets; }

    ALWAYS_INLINE const auto& c_ctx::cvars( ) const { return m_cvars; }
}