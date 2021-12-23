#pragma once

namespace csgo {
	class c_ctx {
	private:

	public:
		void init( );
	};

	inline const auto g_ctx = std::make_unique< c_ctx >( );
}