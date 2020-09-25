#include "../hooks.h"

void __stdcall hooks::client_mode::override_view::fn(c_view_setup* view) {
	static const auto original = m_client_mode->get_original<T>(index);

	original(view);
}