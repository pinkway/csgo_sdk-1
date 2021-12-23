#pragma once

namespace sdk::detail {
	/* the basic funcs that each clr should have */
	template < typename _value_t >
		requires std::_Is_any_of_v< _value_t, std::uint8_t, float >
	struct clr_helper_t {
	protected:
		std::array< _value_t, 4u > m_elements{};
	public:
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
	public:
		ALWAYS_INLINE constexpr base_clr_t( ) = default;

		/* so that we can construct from int etc without casts */
		template < typename _value_t >
			requires ( !std::is_same_v< _value_t, float > )
		ALWAYS_INLINE constexpr base_clr_t(
			const _value_t a, const _value_t r,
			const _value_t g, const _value_t b
		) : base_t{
			static_cast< std::uint8_t >( b ),
			static_cast< std::uint8_t >( g ),
			static_cast< std::uint8_t >( r ),
			static_cast< std::uint8_t >( a )
		} {}

		ALWAYS_INLINE constexpr base_clr_t(
			const float a, const float h,
			const float s, const float v
		) {
			base_t::a( ) = static_cast< std::uint8_t >( a * 0xff );

			const auto hue = h >= 360.f ? 0.f : h / 60.f;
			const auto f = hue - static_cast< int >( hue );
			const auto p = v * ( 1.f - s );
			const auto q = v * ( 1.f - s * f );
			const auto t = v * ( 1.f - ( s * ( 1.f - f ) ) );

			if ( hue < 1.f ) {
				r( ) = static_cast< std::uint8_t >( v * 0xff );
				g( ) = static_cast< std::uint8_t >( t * 0xff );
				b( ) = static_cast< std::uint8_t >( p * 0xff );
			}
			else if ( hue < 2.f ) {
				r( ) = static_cast< std::uint8_t >( q * 0xff );
				g( ) = static_cast< std::uint8_t >( v * 0xff );
				b( ) = static_cast< std::uint8_t >( p * 0xff );
			}
			else if ( hue < 3.f ) {
				r( ) = static_cast< std::uint8_t >( p * 0xff );
				g( ) = static_cast< std::uint8_t >( v * 0xff );
				b( ) = static_cast< std::uint8_t >( t * 0xff );
			}
			else if ( hue < 4.f ) {
				r( ) = static_cast< std::uint8_t >( p * 0xff );
				g( ) = static_cast< std::uint8_t >( q * 0xff );
				b( ) = static_cast< std::uint8_t >( v * 0xff );
			}
			else if ( hue < 5.f ) {
				r( ) = static_cast< std::uint8_t >( t * 0xff );
				g( ) = static_cast< std::uint8_t >( p * 0xff );
				b( ) = static_cast< std::uint8_t >( v * 0xff );
			}
			else {
				r( ) = static_cast< std::uint8_t >( v * 0xff );
				g( ) = static_cast< std::uint8_t >( p * 0xff );
				b( ) = static_cast< std::uint8_t >( q * 0xff );
			}
		}

		ALWAYS_INLINE constexpr std::uint8_t& r( ) { return base_t::at( 2u ); }

		ALWAYS_INLINE constexpr std::uint8_t r( ) const { return base_t::at( 2u ); }

		ALWAYS_INLINE constexpr std::uint8_t& g( ) { return base_t::at( 1u ); }

		ALWAYS_INLINE constexpr std::uint8_t g( ) const { return base_t::at( 1u ); }

		ALWAYS_INLINE constexpr std::uint8_t& b( ) { return base_t::at( 0u ); }

		ALWAYS_INLINE constexpr std::uint8_t b( ) const { return base_t::at( 0u ); }
	};

	template <>
	struct base_clr_t< float > final : public clr_helper_t< float > {
	private:
		using base_t = clr_helper_t< float >;
	public:
		ALWAYS_INLINE constexpr base_clr_t( ) = default;

		ALWAYS_INLINE constexpr base_clr_t(
			const float a, const float h,
			const float s, const float v
		) : base_t{ v, s, h, a } {}

		template < typename _value_t >
			requires ( !std::is_same_v< _value_t, float > )
		ALWAYS_INLINE constexpr base_clr_t(
			const _value_t a, const _value_t r,
			const _value_t g, const _value_t b
		) {
			base_t::a( ) = a / 255.f;

			const auto red = r / 255.f;
			const auto green = g / 255.f;
			const auto blue = b / 255.f;

			const auto min = std::min( { red, green, blue } );
			const auto max = std::max( { red, green, blue } );

			const auto delta = max - min;
			if ( delta == 0.f )
				h( ) = 0.f;
			else {
				if ( max == red )
					h( ) = ( green - blue ) / delta;
				else if ( max == green )
					h( ) = 2.f + ( blue - red ) / delta;
				else if ( max == blue )
					h( ) = 4.f + ( red - green ) / delta;

				h( ) *= 60.f;
			}

			s( ) = ( max != 0.f ) ? delta / max : 0.f;
			v( ) = max;
		}

		ALWAYS_INLINE constexpr float& h( ) { return base_t::at( 2u ); }

		ALWAYS_INLINE constexpr float h( ) const { return base_t::at( 2u ); }

		ALWAYS_INLINE constexpr float& s( ) { return base_t::at( 1u ); }

		ALWAYS_INLINE constexpr float s( ) const { return base_t::at( 1u ); }

		ALWAYS_INLINE constexpr float& v( ) { return base_t::at( 0u ); }

		ALWAYS_INLINE constexpr float v( ) const { return base_t::at( 0u ); }
	};
}