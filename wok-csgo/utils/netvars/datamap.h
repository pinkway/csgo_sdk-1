#pragma once

struct inputdata_t;

enum field_type_t {
	FIELD_VOID = 0,
	FIELD_FLOAT,
	FIELD_STRING,
	FIELD_VECTOR,
	FIELD_quaternion_t,
	FIELD_INTEGER,
	FIELD_BOOLEAN,
	FIELD_SHORT,
	FIELD_CHARACTER,
	FIELD_COLOR32,
	FIELD_EMBEDDED,
	FIELD_CUSTOM,

	FIELD_CLASSPTR,
	FIELD_EHANDLE,
	FIELD_EDICT,

	FIELD_POSITION_VECTOR,
	FIELD_TIME,
	FIELD_TICK,
	FIELD_MODELNAME,
	FIELD_SOUNDNAME,

	FIELD_INPUT,
	FIELD_FUNCTION,

	FIELD_VMATRIX,

	FIELD_VMATRIX_WORLDSPACE,
	FIELD_MATRIX3X4_WORLDSPACE,

	FIELD_INTERVAL,
	FIELD_MODELINDEX,
	FIELD_MATERIALINDEX,

	FIELD_VECTOR2D,

	FIELD_TYPECOUNT,
};

class c_base_entity;
typedef void (c_base_entity::*inputfunc_t)(inputdata_t &data);

struct datamap_t;

enum {
	TD_OFFSET_NORMAL = 0,
	TD_OFFSET_PACKED = 1,
	TD_OFFSET_COUNT
};

class type_description_t {
public:
	int32_t		m_field_type;
	char*		m_field_name;
	int			m_field_offset[TD_OFFSET_COUNT];
	int16_t		m_field_size;
	int16_t		m_flags;
	char		pad0[12];
	datamap_t*	m_datamap;
	char		pad1[24];
};

struct datamap_t {
	type_description_t*	m_data_description;
	int                 m_data_num_fields;
	char const*			m_data_class_name;
	datamap_t*			m_base_map;

	bool                m_chains_validated;
	bool                m_packed_offsets_computed;
	int                 m_packed_size;
};

__declspec(noinline) static uint32_t find_in_data_map(datamap_t* map, uint32_t hash) {
	while (map) {
		for (int i = 0; i < map->m_data_num_fields; i++) {
			if (!map->m_data_description[i].m_field_name)
				continue;

			if (hash == fnv1a_rt(map->m_data_description[i].m_field_name))
				return map->m_data_description[i].m_field_offset[TD_OFFSET_NORMAL];

			if (map->m_data_description[i].m_field_type == FIELD_EMBEDDED) {
				if (map->m_data_description[i].m_datamap) {
					if (const auto offset = find_in_data_map(map->m_data_description[i].m_datamap, hash))
						return offset;
				}
			}
		}

		map = map->m_base_map;
	}

	return 0;
}

#define DATAMAP(func, type, name) \
	type& func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = find_in_data_map(get_pred_desc_map(), hash); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset ); \
	}