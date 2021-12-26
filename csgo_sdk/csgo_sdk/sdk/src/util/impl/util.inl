#include "../util.hpp"

namespace sdk {
    ALWAYS_INLINE std::string to_multi_byte( const std::wstring_view wstr ) {
        if ( wstr.empty( ) )
            return {};

        std::size_t len{};
        std::mbstate_t state{};

        auto src = wstr.data( );

        if ( wcsrtombs_s( &len, nullptr, 0u, &src, wstr.size( ), &state ) )
            return {};

        std::string str{};

        str.resize( len - 1u );

        if ( wcsrtombs_s( &len, str.data( ), len, &src, wstr.size( ), &state ) )
            return {};

        return str;
    }

    ALWAYS_INLINE std::wstring to_wide_char( const std::string_view str ) {
        if ( str.empty( ) )
            return {};

        std::size_t len{};
        std::mbstate_t state{};

        auto src = str.data( );

        if ( mbsrtowcs_s( &len, nullptr, 0u, &src, str.size( ), &state ) )
            return {};

        std::wstring wstr{};

        wstr.resize( len - 1u );

        if ( mbsrtowcs_s( &len, wstr.data( ), len, &src, str.size( ), &state ) )
            return {};

        return wstr;
    }

    ALWAYS_INLINE constexpr hash_t hash( const std::string_view str ) {
#if defined( _M_IX86 ) || defined( __i386__ ) || defined( _M_ARM ) || defined( __arm__ )
        constexpr auto k_basis = 0x811c9dc5u;
        constexpr auto k_prime = 0x1000193u;
#else
        constexpr auto k_basis = 0xcbf29ce484222325u;
        constexpr auto k_prime = 0x100000001b3u;
#endif

        auto hash = k_basis;

        for ( const auto& chr : str ) {
            hash ^= chr;
            hash *= k_prime;
        }

        return hash;
    }

    template < typename _enum_t >
        requires std::is_enum_v< _enum_t >
    ALWAYS_INLINE constexpr std::underlying_type_t< _enum_t > to_underlying( const _enum_t value ) {
        return static_cast< std::underlying_type_t< _enum_t > >( value );
    }
}