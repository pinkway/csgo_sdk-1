#include "../globals.h"

std::string c_base_combat_weapon::get_name() {
	if (!this)
		return "";

	auto weapon_data = get_cs_weapon_data();
	if (!weapon_data)
		return "";

	auto localized_name = interfaces::localize->find(weapon_data->m_hud_name);

	std::wstring w(localized_name);
	std::string ret(w.begin(), w.end());
	std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);

	return ret;
}

c_cs_weapon_data* c_base_combat_weapon::get_cs_weapon_data() {
	if (!this)
		return nullptr;

	return interfaces::weapon_system->get_cs_weapon_data(get_item_definition_index());
}

player_info_t c_cs_player::get_info() {
	player_info_t info;
	interfaces::engine->get_player_info(get_index(), &info);
	return info;
}

int c_base_animating::get_sequence_activity(int sequence) {
	auto model = get_model();
	if (!model)
		return -1;

	auto hdr = interfaces::model_info->get_studio_model(model);
	if (!hdr)
		return -1;

	static const auto get_sequence_activity_fn = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(SIG("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));

	return get_sequence_activity_fn(this, hdr, sequence);
}

vec3_t c_base_player::get_hitbox_position(int hitbox, matrix3x4_t* matrix) {
	auto model = get_model();
	if (!model)
		return vec3_t();

	auto hdr = interfaces::model_info->get_studio_model(model);
	if (!hdr)
		return vec3_t();

	auto set = hdr->get_hitbox_set(0);
	if (!set)
		return vec3_t();

	auto hitbox_ = set->get_hitbox(hitbox);
	if (!hitbox_)
		return vec3_t();

	auto bones = !matrix ? get_bone_cache()->m_cached_bones : matrix;

	vec3_t min, max, center;
	math::vector_transform(hitbox_->min, bones[hitbox_->bone], min);
	math::vector_transform(hitbox_->max, bones[hitbox_->bone], max);

	center = (min + max) * 0.5f;

	return center;
}

c_base_combat_weapon* c_base_combat_character::get_active_weapon() {
	auto handle = get_active_weapon_handle();
	if (!handle.is_valid())
		return nullptr;

	return reinterpret_cast<c_base_combat_weapon*>(handle.get());
}

bool c_base_entity::is_enemy() {
	static const auto mp_teammates_are_enemies = interfaces::cvar->find_var(_("mp_teammates_are_enemies"));
	if (mp_teammates_are_enemies->get_bool())
		return true;

	return get_team() != g::local->get_team();
}

void c_base_player::reset_anim_state(c_anim_state* state) {
	static const auto reset_fn = reinterpret_cast<void(__thiscall*)(c_anim_state*)>(SIG("client_panorama.dll", "56 6A 01 68 ? ? ? ? 8B F1"));
	if (!reset_fn)
		return;

	reset_fn(state);
}

void c_base_player::update_anim_state(c_anim_state* state, qangle_t angle) {
	static const auto update_fn = SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");
	if (!update_fn)
		return;

	__asm {
		push 0
	}

	__asm {
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call update_fn
	}
}

void c_base_player::create_anim_state(c_anim_state* state) {
	static const auto create_fn = reinterpret_cast<void(__thiscall*)(c_anim_state*, c_base_player*)>(SIG("client_panorama.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));
	if (!create_fn)
		return;

	create_fn(state, this);
}