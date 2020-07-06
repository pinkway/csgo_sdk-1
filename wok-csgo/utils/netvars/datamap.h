#pragma once

enum offset_type_t {
	TD_OFFSET_NORMAL,
	TD_OFFSET_PACKED
};

enum field_type_t {
	FIELD_VOID,
	FIELD_FLOAT,
	FIELD_STRING,
	FIELD_VECTOR,
	FIELD_QUATERNION,
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
	FIELD_VECTOR2D
};

struct datamap_t;

struct type_description_t {
	int			m_field_type;
	char*		m_field_name;
	int			m_field_offset[2];
	short		m_field_size;
	short		m_flags;
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
	do {
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
	} while (map);

	return 0;
}

#define DATAMAP(func, type, name) \
	type& func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = find_in_data_map(get_pred_desc_map(), hash); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset ); \
	}