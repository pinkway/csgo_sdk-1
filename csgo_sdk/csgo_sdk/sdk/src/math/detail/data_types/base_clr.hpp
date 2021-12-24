#pragma once

namespace sdk::detail {
	/* the basic funcs that each clr should have */
	template < typename _value_t >
		requires std::_Is_any_of_v< _value_t, std::uint8_t, float >
	struct clr_helper_t {
	protected:
		std::array< _value_t, 4u > m_elements{};
	public:
		using value_t = _value_t;

		ALWAYS_INLINE constexpr clr_helper_t( ) = default;

		ALWAYS_INLINE constexpr clr_helper_t(
			const _value_t v0, const _value_t v1,
			const _value_t v2, const _value_t a
		) : m_elements{ v0, v1, v2, a } {}

		ALWAYS_INLINE constexpr _value_t& at( const std::size_t i ) {
			return m_elements.at( i );
		}

		ALWAYS_INLINE constexpr _value_t at( const std::size_t i ) const {
			return m_elements.at( i );
		}

		ALWAYS_INLINE constexpr _value_t& a( ) { return m_elements.at( 3u ); }

		ALWAYS_INLINE constexpr _value_t a( ) const { return m_elements.at( 3u ); }
	};

	/* std::uint8_t - argb | float - ahsv ( 360.f, 1.f, 1.f, 1.f ) */
	template < typename _value_t >
		requires std::_Is_any_of_v< _value_t, std::uint8_t, float >
	struct base_clr_t final : public clr_helper_t< _value_t > {
	private:
		using base_t = clr_helper_t< _value_t >;
	public:
		ALWAYS_INLINE constexpr base_clr_t( ) = default;

		ALWAYS_INLINE constexpr base_clr_t(
			const _value_t v0, const _value_t v1,
			const _value_t v2, const _value_t a
		) : base_t{ v0, v1, v2, a } {}
	};

	template <>
	struct base_clr_t< std::uint8_t > final : public clr_helper_t< std::uint8_t > {
	private:
		using base_t = clr_helper_t< std::uint8_t >;

		using value_t = base_t::value_t;
	public:
		ALWAYS_INLINE constexpr base_clr_t( ) = default;

		/* so that we can construct from int etc without casts 
			note: it is probably bad practice to use float values to construct from the hsb value
			and all other for rgb ctor
			creating an additional function ( ex. conv::hsb_to_rgb ) should be the solution */
		template < typename _ctor_t >
			requires ( !std::is_same_v< _ctor_t, float > )
		ALWAYS_INLINE constexpr base_clr_t(
			const _ctor_t a, const _ctor_t r,
			const _ctor_t g, const _ctor_t b
		) : base_t{
			static_cast< value_t >( b ),
			static_cast< value_t >( g ),
			static_cast< value_t >( r ),
			static_cast< value_t >( a )
		} {}

		ALWAYS_INLINE constexpr base_clr_t(
			const float a, const float h,
			const float s, const float v
		) {
			constexpr auto k_value_limit = std::numeric_limits< value_t >::max( );

			base_t::a( ) = static_cast< value_t >( a * k_value_limit );

			const auto sextant = h >= 360.f ? 0.f : h / 60.f;
			const auto intermediate_color_perc = sextant - static_cast< int >( sextant );
			const auto p = v * ( 1.f - s );
			const auto q = v * ( 1.f - s * intermediate_color_perc );
			const auto t = v * ( 1.f - ( s * ( 1.f - intermediate_color_perc ) ) );

			switch ( static_cast< int >( sextant ) ) {
			case 0:
				r( ) = static_cast< value_t >( v * k_value_limit );
				g( ) = static_cast< value_t >( t * k_value_limit );
				b( ) = static_cast< value_t >( p * k_value_limit );

				break;
			case 1:
				r( ) = static_cast< value_t >( q * k_value_limit );
				g( ) = static_cast< value_t >( v * k_value_limit );
				b( ) = static_cast< value_t >( p * k_value_limit );

				break;
			case 2:
				r( ) = static_cast< value_t >( p * k_value_limit );
				g( ) = static_cast< value_t >( v * k_value_limit );
				b( ) = static_cast< value_t >( t * k_value_limit );

				break;
			case 3:
				r( ) = static_cast< value_t >( p * k_value_limit );
				g( ) = static_cast< value_t >( q * k_value_limit );
				b( ) = static_cast< value_t >( v * k_value_limit );

				break;
			case 4:
				r( ) = static_cast< value_t >( t * k_value_limit );
				g( ) = static_cast< value_t >( p * k_value_limit );
				b( ) = static_cast< value_t >( v * k_value_limit );

				break;
			case 5:
				r( ) = static_cast< value_t >( v * k_value_limit );
				g( ) = static_cast< value_t >( p * k_value_limit );
				b( ) = static_cast< value_t >( q * k_value_limit );

				break;
			}
		}

		ALWAYS_INLINE constexpr value_t& r( ) { return base_t::at( 2u ); }

		ALWAYS_INLINE constexpr value_t r( ) const { return base_t::at( 2u ); }

		ALWAYS_INLINE constexpr value_t& g( ) { return base_t::at( 1u ); }

		ALWAYS_INLINE constexpr value_t g( ) const { return base_t::at( 1u ); }

		ALWAYS_INLINE constexpr value_t& b( ) { return base_t::at( 0u ); }

		ALWAYS_INLINE constexpr value_t b( ) const { return base_t::at( 0u ); }
	};

	template <>
	struct base_clr_t< float > final : public clr_helper_t< float > {
	private:
		using base_t = clr_helper_t< float >;

		using value_t = base_t::value_t;
	public:
		ALWAYS_INLINE constexpr base_clr_t( ) = default;

		ALWAYS_INLINE constexpr base_clr_t(
			const float a, const float h,
			const float s, const float v
		) : base_t{ v, s, h, a } {}

		template < typename _ctor_t >
			requires ( !std::is_same_v< _ctor_t, float > )
		ALWAYS_INLINE constexpr base_clr_t(
			const _ctor_t a, const _ctor_t r,
			const _ctor_t g, const _ctor_t b
		) {
			constexpr auto k_def_channel_limit = std::numeric_limits< std::uint8_t >::max( );

			base_t::a( ) = a / static_cast< value_t >( k_def_channel_limit );

			const auto r_frac = r / static_cast< value_t >( k_def_channel_limit );
			const auto g_frac = g / static_cast< value_t >( k_def_channel_limit );
			const auto b_frac = b / static_cast< value_t >( k_def_channel_limit );

			const auto max = std::max( { r_frac, g_frac, b_frac } );
			const auto delta = max - std::min( { r_frac, g_frac, b_frac } );

			if ( delta == 0.f )
				h( ) = 0.f;
			else {
				if ( max == r_frac )
					h( ) = ( g_frac - b_frac ) / delta;
				else if ( max == g_frac )
					h( ) = 2.f + ( b_frac - r_frac ) / delta;
				else
					h( ) = 4.f + ( r_frac - g_frac ) / delta;

				h( ) *= 60.f;
			}

			s( ) = ( max != 0.f ) ? delta / max : 0.f;
			v( ) = max;
		}

		ALWAYS_INLINE constexpr value_t& h( ) { return base_t::at( 2u ); }

		ALWAYS_INLINE constexpr value_t h( ) const { return base_t::at( 2u ); }

		ALWAYS_INLINE constexpr value_t& s( ) { return base_t::at( 1u ); }

		ALWAYS_INLINE constexpr value_t s( ) const { return base_t::at( 1u ); }

		ALWAYS_INLINE constexpr value_t& v( ) { return base_t::at( 0u ); }

		ALWAYS_INLINE constexpr value_t v( ) const { return base_t::at( 0u ); }
	};
}