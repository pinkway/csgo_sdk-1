#pragma once

#include "../math.hpp"

namespace sdk {
	template < typename _value_t > requires std::is_arithmetic_v< _value_t >
	ALWAYS_INLINE constexpr auto to_deg( const _value_t rad ) {
		using flt_t = std::conditional_t< sizeof( _value_t ) <= sizeof( float ), float, double >;

		return static_cast< flt_t >( rad * k_rad_pi< flt_t > );
	}

	template < typename _value_t > requires std::is_arithmetic_v< _value_t >
	ALWAYS_INLINE constexpr auto to_rad( const _value_t deg ) {
		using flt_t = std::conditional_t< sizeof( _value_t ) <= sizeof( float ), float, double >;

		return static_cast< flt_t >( deg * k_deg_pi< flt_t > );
	}

	template < typename _value_t > requires is_addable< _value_t, _value_t > && is_multipliable< _value_t, float >
	ALWAYS_INLINE constexpr _value_t lerp( const _value_t& from, const _value_t& to, const float amt ) {
		return from * ( 1.f - amt ) + to * amt;
	}
}