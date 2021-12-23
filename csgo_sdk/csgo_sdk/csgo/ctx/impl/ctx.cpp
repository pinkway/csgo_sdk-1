#include "../../csgo.hpp"

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved ) {
	if ( reason != DLL_PROCESS_ATTACH )
		return 0;

	DisableThreadLibraryCalls( instance );

	std::jthread{ [ ]( ) {
		csgo::g_ctx->init( );
	} }.detach( );

	return 1;
}

namespace csgo {
	void c_ctx::init( ) {

	}
}