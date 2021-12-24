#pragma once

#include "../sdk/sdk.hpp"

#define OFFSET( type, name, offset ) \
	ALWAYS_INLINE type& name { \
		return *sdk::address_t{ this }.self_offset( offset ).as< type* >( ); \
	} \

#define VFUNC( type, name, index, ... ) \
	ALWAYS_INLINE auto name { \
		using fn_t = type; \
		\
		return ( *sdk::address_t{ this }.as< fn_t** >( )[ index ] )( this, __VA_ARGS__ ); \
	} \

#define OFFSET_VFUNC( type, name, offset, ... ) \
	ALWAYS_INLINE auto name { \
		return offset.as< type >( )( this, __VA_ARGS__ ); \
	} \

#include "ctx/ctx.hpp"

#include "valve/valve.hpp"

#include "hacks/hacks.hpp"
#include "hooks/hooks.hpp"