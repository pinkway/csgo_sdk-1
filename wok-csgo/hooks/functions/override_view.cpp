#include "../hooks.h"

void __stdcall hooks::override_view::fn(c_view_setup* setup) {
	static const auto original = client_mode->get_original<t>(index);

	original(setup);
}