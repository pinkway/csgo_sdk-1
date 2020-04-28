#pragma once

class c_base_entity;
class i_handle_entity;

struct csurface_t {
	const char*		name;
	short			surface_props;
	unsigned short	flags;
};

struct cplane_t {
	vec3_t		normal;
	float		dist;
	byte		type;
	byte		signbits;
	byte		pad[2];
};

enum trace_type_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS
};

class i_trace_filter {
public:
	virtual bool should_hit_entity(i_handle_entity* handle, int mask) = 0;
	virtual trace_type_t get_trace_type() const = 0;
};

class c_trace_filter : public i_trace_filter {
public:
	c_trace_filter() = default;
	explicit c_trace_filter(i_handle_entity* skip, trace_type_t type = TRACE_EVERYTHING) {
		m_skip = skip;
		m_type = type;
	}

	bool should_hit_entity(i_handle_entity* handle, int mask) {
		return !(handle == m_skip);
	}

	trace_type_t get_trace_type() const {
		return m_type;
	}

	i_handle_entity* m_skip;
	trace_type_t m_type = TRACE_EVERYTHING;
};

class c_trace_filter_skip_two_entities : public i_trace_filter {
public:
	c_trace_filter_skip_two_entities() = default;
	c_trace_filter_skip_two_entities(i_handle_entity* skip1, i_handle_entity* skip2) {
		m_skip1 = skip1;
		m_skip2 = skip2;
	}

	bool should_hit_entity(i_handle_entity* handle, int mask) {
		return !(handle == m_skip1 || handle == m_skip2);
	}

	trace_type_t get_trace_type() const {
		return TRACE_EVERYTHING;
	}

	i_handle_entity* m_skip1;
	i_handle_entity* m_skip2;
};

class c_trace_filter_world_only : public i_trace_filter {
public:
	bool should_hit_entity(i_handle_entity* handle, int mask) {
		return false;
	}

	trace_type_t get_trace_type() const {
		return TRACE_WORLD_ONLY;
	}
};

class c_trace_filter_no_players : public c_trace_filter {
public:
	bool should_hit_entity(i_handle_entity* handle, int mask) override {
		return false;
	}
};

class c_trace_entity : public i_trace_filter {
public:
	c_trace_entity() = default;
	explicit c_trace_entity(i_handle_entity* hit) {
		m_hit = hit;
	}

	bool should_hit_entity(i_handle_entity* handle, int mask) {
		return (handle == m_hit);
	}

	trace_type_t get_trace_type() const {
		return TRACE_ENTITIES_ONLY;
	}

	i_handle_entity* m_hit;
};

struct ray_t {
private:
public:
	vector_aligned		m_start;
	vector_aligned		m_delta;
	vector_aligned		m_start_offset;
	vector_aligned		m_extents;
	const matrix3x4_t*	m_world_axis_transform;
	bool				m_is_ray;
	bool				m_is_swept;

	ray_t() : m_world_axis_transform(nullptr), m_is_ray{ false }, m_is_swept{ false } { }

	ray_t(vec3_t const &start, vec3_t const &end) {
		init(start, end);
	}

	ray_t(vec3_t const &start, vec3_t const &end, vec3_t const &mins, vec3_t const &maxs) :
		m_world_axis_transform(nullptr), m_is_ray{ false }, m_is_swept{ false } {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.length() != 0;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.length_sqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}

	void init(vec3_t const &start, vec3_t const &end) {
		m_delta = end - start;

		m_is_swept = m_delta.length_sqr() != 0;

		m_extents = vector_aligned(0, 0, 0);

		m_world_axis_transform = nullptr;
		m_is_ray = true;

		m_start_offset = vector_aligned(0, 0, 0);
		m_start = start;
	}

	void init(vec3_t const &start, vec3_t const &end, vec3_t const &mins, vec3_t const &maxs) {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.length() != 0;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.length_sqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}
};

class c_base_trace {
public:
	bool is_disp_surface() {
		return m_disp_flags & DISPSURF_FLAG_SURFACE;
	}

	bool is_disp_surface_walkable() {
		return m_disp_flags & DISPSURF_FLAG_WALKABLE;
	}

	bool is_disp_surface_buildable() {
		return m_disp_flags & DISPSURF_FLAG_BUILDABLE;
	}

	bool is_disp_surface_prop1() {
		return  m_disp_flags & DISPSURF_FLAG_SURFPROP1;
	}

	bool is_disp_surface_prop2() {
		return m_disp_flags & DISPSURF_FLAG_SURFPROP2;
	}

public:
	vec3_t			m_startpos;
	vec3_t			m_endpos;
	cplane_t		m_plane;

	float			m_fraction;

	int				m_contents;
	unsigned short	m_disp_flags;

	bool			m_allsolid;
	bool			m_startsolid;

	c_base_trace() {}
};

class trace_t : public c_base_trace {
public:
	int get_entity_index() const { return m_hit_entity ? m_hit_entity->get_index() : -1; }

	bool did_hit() const {
		return m_fraction < 1 || m_allsolid || m_startsolid;
	}

	bool did_hit_world() const {
		return get_entity_index() == 0;
	}

	bool did_hit_non_world_entity() const {
		return m_hit_entity && !did_hit_world();
	}

	bool is_visible() const {
		return m_fraction > 0.97f;
	}

public:
	float				m_fractionleftsolid;
	csurface_t			m_surface;
	int					m_hitgroup;
	short				m_physicsbone;
	unsigned short		m_world_surface_index;
	i_client_entity*	m_hit_entity;
	int					m_hitbox;

	trace_t() {}
private:
	trace_t(const trace_t &other) :
		m_fractionleftsolid(other.m_fractionleftsolid),
		m_surface(other.m_surface),
		m_hitgroup(other.m_hitgroup),
		m_physicsbone(other.m_physicsbone),
		m_world_surface_index(other.m_world_surface_index),
		m_hit_entity(other.m_hit_entity),
		m_hitbox(other.m_hitbox) {
		m_startpos = other.m_startpos;
		m_endpos = other.m_endpos;
		m_plane = other.m_plane;
		m_fraction = other.m_fraction;
		m_contents = other.m_contents;
		m_disp_flags = other.m_disp_flags;
		m_allsolid = other.m_allsolid;
		m_startsolid = other.m_startsolid;
	}
};