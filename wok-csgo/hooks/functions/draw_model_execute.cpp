#include "../hooks.h"

void __fastcall hooks::draw_model_execute::fn(i_model_render* ecx, void* edx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones) {
	static const auto original = m_model_render->get_original<T>(index);

	auto should_call_original = true;

	chams->on_dme(ecx, context, state, info, bones, should_call_original);

	should_call_original ? original(ecx, context, state, info, bones) : 0;

	interfaces::model_render->forced_material_override();
}