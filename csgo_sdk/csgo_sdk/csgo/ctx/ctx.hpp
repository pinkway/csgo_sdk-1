#pragma once

namespace csgo {
	class c_ctx {
	private:
		struct {

		} m_offsets{};

		struct {

		} m_cvars{};
	public:
		void init( );

		ALWAYS_INLINE const auto& offsets( ) const;

		ALWAYS_INLINE const auto& cvars( ) const;
	};

	inline const auto g_ctx = std::make_unique< c_ctx >( );
}

#include "impl/ctx.inl"