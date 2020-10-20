#pragma once

struct model_t {
	char		pad0[4];
	char		m_name[260];
	int		m_load_flags;
	int		m_server_count;
	int		m_type;
	int		m_flags;
	vec3_t	m_mins;
	vec3_t	m_maxs;
	float		m_radius;
	c_key_values*		m_key_values;
	char		pad1[24];
};

struct model_render_info_t {
	vec3_t					m_origin;
	qangle_t					m_angles;
	char						pad[4];
	i_client_renderable*	m_renderable;
	const model_t*			m_model;
	const matrix3x4_t*	m_model_to_world;
	const matrix3x4_t*	m_lighting_offset;
	const vec3_t*			m_lighting_origin;
	int						m_flags;
	int						m_index;
	int						m_skin;
	int						m_body;
	int						m_hitboxset;
	uint16_t					m_instance;
};

struct studiohwdata_t;

struct draw_model_state_t {
	studiohdr_t*			m_studio_hdr;
	studiohwdata_t*		m_studio_hw_data;
	i_client_renderable*	m_renderable;
	const matrix3x4_t*	m_bones;
};

class i_model_render {
public:
	virtual int		draw_model						(int flags, i_client_renderable* renderable, uint16_t instance, int entity_index, const model_t* model,
																 vec3_t const& origin, qangle_t const& angles, int skin, int body, int hitbox, const matrix3x4_t* world_model = 0, const matrix3x4_t* lighting_offset = 0) = 0;
	virtual void	forced_material_override	(i_material* material = nullptr, e_override_type type = OVERRIDE_NORMAL, int material_index = 0) = 0;
	virtual bool	is_forced_material_override() = 0;

	VFUNC(draw_model_execute(void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones), 21, void(__thiscall*)(void*, void*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*), context, state, info, bones)
};