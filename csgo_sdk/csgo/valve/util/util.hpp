#pragma once

namespace csgo::valve {
    template < typename _value_t, typename _index_t >
    struct utl_mem_t {
    private:
        _value_t*   m_ptr{};
        int         m_alloc_count{};
        int         m_grow_size{};
    public:
        ALWAYS_INLINE _value_t& at( const _index_t i );

        ALWAYS_INLINE const _value_t& at( const _index_t i ) const;

        ALWAYS_INLINE void clear( );

        ALWAYS_INLINE void grow( const int count );

        ALWAYS_INLINE int alloc_count( ) const;
    };

    template < typename _value_t >
    struct utl_vec_t {
    private:    
        utl_mem_t< _value_t, int >  m_mem{};
        int                         m_size{};
        _value_t*                   m_elements{};
    public:
        ALWAYS_INLINE constexpr utl_vec_t( ) = default;

        ALWAYS_INLINE _value_t& at( const int i );

        ALWAYS_INLINE const _value_t& at( const int i ) const;

        ALWAYS_INLINE void clear( );

        ALWAYS_INLINE void reserve( const int size );

        ALWAYS_INLINE int size( ) const;

        ALWAYS_INLINE utl_vec_t< _value_t >& operator =( const utl_vec_t< _value_t >& other );
    };

    struct key_values_t {
        using get_symbol_proc_t = bool( __cdecl* )( const char* );

        std::uint32_t       m_key_name : 24u,
                            m_key_name_case_sensitive1 : 8u;

        char*               m_str{};
        wchar_t*            m_wstr{};

        union {
            int             m_int;
            float           m_float;
            sdk::address_t  m_ptr;
            sdk::argb_t     m_clr{};
        };

        char                m_data_type{},
                            m_has_esc_sequences{};
        std::uint16_t       m_key_name_case_sensitive2{};

        key_values_t*       m_peer{}, *m_sub{}, *m_chain{};

        get_symbol_proc_t   m_get_symbol_proc_fn{};
    };
}

#include "impl/util.inl"