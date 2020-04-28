#pragma once

struct model_t {
	char        name[255];
};

enum mod_type_t {
	MOD_BAD = 0,
	MOD_BRUSH,
	MOD_SPRITE,
	MOD_STUDIO
};

typedef unsigned short model_instance_handle_t;

struct model_render_info_t {
	model_render_info_t() {
		model_to_world = nullptr;
		lighting_offset = nullptr;
		lighting_origin = nullptr;
	}

	vec3_t					origin;
	qangle_t				angles;
	char					pad[4];
	i_client_renderable*	renderable;
	const model_t*			model;
	const matrix3x4_t*		model_to_world;
	const matrix3x4_t*		lighting_offset;
	const vec3_t*			lighting_origin;
	int						flags;
	int						index;
	int						skin;
	int						body;
	int						hitboxset;
	model_instance_handle_t instance;
};

class studiohwdata_t;

struct draw_model_state_t {
	studiohdr_t*			m_studio_hdr;
	studiohwdata_t*			m_studio_hw_data;
	i_client_renderable*	m_renderable;
	const matrix3x4_t*		m_bones;
};

enum override_type_t {
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE,
};

class i_model_render {
public:
	VFUNC(forced_material_override(i_material* material = nullptr, override_type_t type = OVERRIDE_NORMAL), 1, void(__thiscall*)(void*, i_material*, override_type_t, int), material, type, 0)
	VFUNC(is_forced_material_override(), 2, bool(__thiscall*)(void*))
};