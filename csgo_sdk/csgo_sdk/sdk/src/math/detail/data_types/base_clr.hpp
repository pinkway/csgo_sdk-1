#pragma once

namespace sdk::detail {
    template < typename _value_t, std::size_t _hue_limit, std::size_t _limit >
        requires ( _hue_limit <= std::numeric_limits< _value_t >::max( ) && _limit <= std::numeric_limits< _value_t >::max( ) )
    struct base_ahsv_t;

    template < typename _value_t = std::uint8_t, std::size_t _limit = std::numeric_limits< _value_t >::max( ) >
        requires ( _limit <= std::numeric_limits< _value_t >::max( ) )
    struct base_argb_t final : public array_wrapper_t< _value_t, 4u, base_argb_t< _value_t, _limit > > {
    private:
        using base_t = array_wrapper_t< _value_t, 4u, base_argb_t< _value_t, _limit > >;
    public:
        ALWAYS_INLINE constexpr base_argb_t( ) = default;

        template < typename _arg_t >
            requires std::is_arithmetic_v< _arg_t >
        ALWAYS_INLINE constexpr base_argb_t(
            const _arg_t a, const _arg_t r,
            const _arg_t g, const _arg_t b
        ) : base_t{
            static_cast< _value_t >( b ),
            static_cast< _value_t >( g ),
            static_cast< _value_t >( r ),
            static_cast< _value_t >( a )
        } {}

        template < typename _ahsv_value_t, std::size_t _ahsv_hue_limit, std::size_t _ahsv_limit >
        ALWAYS_INLINE static constexpr base_argb_t< _value_t, _limit > from_ahsv(
            const base_ahsv_t< _ahsv_value_t, _ahsv_hue_limit, _ahsv_limit >& ahsv
        ) {
            /* note: y and z values always have an error ( = 1 ),
               probably missing a cast somewhere... */
            constexpr auto k_col_chans = 3u;

            base_argb_t< _value_t, _limit > argb{};

            argb.a( ) = static_cast< _value_t >( ( ahsv.a( ) / ahsv.limit( ) ) * argb.limit( ) );

            const auto h = ( ahsv.h( ) / ahsv.hue_limit( ) ) * 6.f;
            const auto s = ahsv.s( ) / ahsv.limit( );
            const auto v = ahsv.v( ) / ahsv.limit( );

            const auto hue_seg = static_cast< int >( h );
            const auto rem = h - hue_seg;

            const auto x = 1.f - s;
            const auto y = 1.f - s * rem;
            const auto z = 1.f - s * ( 1.f - rem );

            /* don't ask */
            const auto lim_pos = ( ( hue_seg + 1u ) / 2u ) % k_col_chans;
            const auto x_pos = ( hue_seg / 2u + 2u ) % k_col_chans;
            const auto yz_pos = ( k_col_chans - 1u ) - ( hue_seg + 1u ) % k_col_chans;

            const auto scale = v * argb.limit( );

            /* note: k_col_chans - 1u is color indices mapping,
               original placement was rgba, current - abgr */
            argb.at( k_col_chans - 1u - lim_pos ) = static_cast< _value_t >( 1.f * scale );
            argb.at( k_col_chans - 1u - x_pos ) = static_cast< _value_t >( x * scale );
            argb.at( k_col_chans - 1u - yz_pos ) = static_cast< _value_t >( ( hue_seg % 2u != 0u ) ? ( y * scale ) : ( z * scale ) );

            return argb;
        }

        ALWAYS_INLINE constexpr _value_t& a( ) { return base_t::at( 3u ); }

        ALWAYS_INLINE constexpr _value_t a( ) const { return base_t::at( 3u ); }

        ALWAYS_INLINE constexpr _value_t& r( ) { return base_t::at( 2u ); }

        ALWAYS_INLINE constexpr _value_t r( ) const { return base_t::at( 2u ); }

        ALWAYS_INLINE constexpr _value_t& g( ) { return base_t::at( 1u ); }

        ALWAYS_INLINE constexpr _value_t g( ) const { return base_t::at( 1u ); }

        ALWAYS_INLINE constexpr _value_t& b( ) { return base_t::at( 0u ); }

        ALWAYS_INLINE constexpr _value_t b( ) const { return base_t::at( 0u ); }

        ALWAYS_INLINE constexpr auto limit( ) const {
            return static_cast< enough_float_t< _value_t > >( _limit );
        }
    };

    template < typename _value_t = float, std::size_t _hue_limit = 360u, std::size_t _limit = 1u >
        requires ( _hue_limit <= std::numeric_limits< _value_t >::max( ) && _limit <= std::numeric_limits< _value_t >::max( ) )
    struct base_ahsv_t final : public array_wrapper_t< _value_t, 4u, base_ahsv_t< _value_t, _hue_limit, _limit > > {
    private:
        using base_t = array_wrapper_t< _value_t, 4u, base_ahsv_t< _value_t, _hue_limit, _limit > >;
    public:
        ALWAYS_INLINE constexpr base_ahsv_t( ) = default;

        template < typename _arg_t >
            requires std::is_arithmetic_v< _arg_t >
        ALWAYS_INLINE constexpr base_ahsv_t(
            const _arg_t a, const _arg_t h,
            const _arg_t s, const _arg_t v
        ) : base_t{
            static_cast< _value_t >( v ),
            static_cast< _value_t >( s ),
            static_cast< _value_t >( h ),
            static_cast< _value_t >( a )
        } {}

        template < typename _argb_value_t, std::size_t _argb_limit >
        ALWAYS_INLINE static constexpr base_ahsv_t< _value_t, _hue_limit, _limit > from_argb(
            const base_argb_t< _argb_value_t, _argb_limit >& argb
        ) {
            base_ahsv_t< _value_t, _hue_limit, _limit > ahsv{};

            ahsv.a( ) = static_cast< _value_t >( ( argb.a( ) / argb.limit( ) ) * ahsv.limit( ) );

            const auto r_frac = argb.r( ) / argb.limit( );
            const auto g_frac = argb.g( ) / argb.limit( );
            const auto b_frac = argb.b( ) / argb.limit( );

            const auto min = std::min( { r_frac, g_frac, b_frac } );
            const auto max = std::max( { r_frac, g_frac, b_frac } );

            const auto delta = max - min;
            if ( delta == 0.f )
                ahsv.h( ) = static_cast< _value_t >( 0.f );
            else {
                constexpr auto k_unscaled_limit = 6.f;

                enough_float_t< _value_t > unscaled_hue{};

                if ( max == r_frac )
                    unscaled_hue = ( g_frac - b_frac ) / delta;
                else if ( max == g_frac )
                    unscaled_hue = 2.f + ( b_frac - r_frac ) / delta;
                else if ( max == b_frac )
                    unscaled_hue = 4.f + ( r_frac - g_frac ) / delta;

                ahsv.h( ) = static_cast< _value_t >( ( unscaled_hue / k_unscaled_limit ) * ahsv.hue_limit( ) );
            }

            ahsv.s( ) = static_cast< _value_t >( ( max != 0.f ) ? ( delta / max ) * ahsv.limit( ) : 0.f );
            ahsv.v( ) = static_cast< _value_t >( max * ahsv.limit( ) );

            return ahsv;
        }

        ALWAYS_INLINE constexpr _value_t& a( ) { return base_t::at( 3u ); }

        ALWAYS_INLINE constexpr _value_t a( ) const { return base_t::at( 3u ); }

        ALWAYS_INLINE constexpr _value_t& h( ) { return base_t::at( 2u ); }

        ALWAYS_INLINE constexpr _value_t h( ) const { return base_t::at( 2u ); }

        ALWAYS_INLINE constexpr _value_t& s( ) { return base_t::at( 1u ); }

        ALWAYS_INLINE constexpr _value_t s( ) const { return base_t::at( 1u ); }

        ALWAYS_INLINE constexpr _value_t& v( ) { return base_t::at( 0u ); }

        ALWAYS_INLINE constexpr _value_t v( ) const { return base_t::at( 0u ); }

        ALWAYS_INLINE constexpr auto limit( ) const {
            return static_cast< enough_float_t< _value_t > >( _limit );
        }

        ALWAYS_INLINE constexpr auto hue_limit( ) const {
            return static_cast< enough_float_t< _value_t > >( _hue_limit );
        }
    };
}