#include "../../../csgo.hpp"

namespace csgo::valve {
    key_values_t::~key_values_t( ) {
        g_mem_alloc->free( this );
    }
}