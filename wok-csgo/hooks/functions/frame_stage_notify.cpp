#include "../hooks.h"

void __stdcall hooks::frame_stage_notify::fn(client_frame_stage_t stage) {
	static const auto original = m_client_dll->get_original<t>(index);

	original(stage);
}