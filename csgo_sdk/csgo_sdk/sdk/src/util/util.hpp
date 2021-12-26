#pragma once

#include "detail/detail.hpp"

#define BYTESEQ( value ) \
    [ & ] ( ) { \
        static_assert( std::is_same_v< const char*, std::decay_t< decltype( value ) > > || std::is_pointer_v< decltype( value ) > ); \
    \
        constexpr auto size = sdk::detail::byte_seq_t< sizeof( value ) >::measure( value ); \
    \
        return sdk::detail::byte_seq_t< size >{ [ & ] ( ) { return value; }, std::make_index_sequence< size >{} }; \
    }( ) \

namespace sdk {
    using hash_t = std::size_t;

    using ulong_t = unsigned long;

    ALWAYS_INLINE std::string to_multi_byte( const std::wstring_view str );

    ALWAYS_INLINE std::wstring to_wide_char( const std::string_view str );

    ALWAYS_INLINE constexpr hash_t hash( const std::string_view str );

    template < typename _enum_t >
        requires std::is_enum_v< _enum_t >
    ALWAYS_INLINE constexpr std::underlying_type_t< _enum_t > to_underlying( const _enum_t value );
}

#include "impl/util.inl"