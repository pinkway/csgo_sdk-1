#include "../features.h"
#include "../../hooks/hooks.h"

i_material* c_chams::create_material(const std::string& material_name, const std::string& shader_type, const std::string& material_data) {
	auto key_values = static_cast<c_key_values*>(interfaces::mem_alloc->alloc(36u));

	key_values->init(shader_type.c_str());
	key_values->load_from_buffer(material_name.c_str(), material_data.c_str());

	return interfaces::material_system->create_material(material_name.c_str(), key_values);
}

bool c_chams::override_material(int type, const col_t& clr, bool ignorez) {
	i_material* material = nullptr;

	switch (type) {
	case material_regular: material = ignorez ? m_regular_z : m_regular; break;
	case material_flat: material = ignorez ? m_flat_z : m_flat; break;
	case dont_draw: return false; break;
	}

	material->alpha_modulate(clr.a() / 255.f);
	material->color_modulate(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f);

	if (auto $alpha = material->find_var(_("$alpha"), nullptr, false))
		$alpha->set_value(clr.a() / 255.f);

	if (auto $envmaptint = material->find_var(_("$envmaptint"), nullptr, false))
		$envmaptint->set_value(vec3_t(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f));

	interfaces::model_render->forced_material_override(material);

	return true;
}

void c_chams::on_dme(i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones, bool& should_call_original) {
	static const auto original = hooks::m_model_render->get_original<hooks::draw_model_execute::t>(hooks::draw_model_execute::index);

	auto model_name = interfaces::model_info->get_model_name(info.m_model);
	if (strlen(model_name) <= 19
		|| model_name[13] != '/'
		|| model_name[7] != 'p'
		|| model_name[0] != 'm')
		return;

	auto player = reinterpret_cast<c_cs_player*>(interfaces::entity_list->get_client_entity(info.m_index));
	if (!player
		|| !player->is_enemy()
		|| !player->is_alive())
		return;

	override_material(material_regular, col_t::palette_t::purple(), true);
	original(ecx, context, state, info, bones);

	should_call_original = override_material(material_regular, col_t::palette_t::purple(), false);
}