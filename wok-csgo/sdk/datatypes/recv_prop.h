#pragma once

class recv_prop;
class recv_table;
struct c_recv_proxy_data;
using recv_var_proxy_fn = void(__cdecl *)(c_recv_proxy_data*, void*, void*);

struct d_variant {
	union {
		float		m_float;
		long		m_int;
		char*		m_string;
		void*		m_data;
		vec3_t		m_vector;
		int64_t		m_int64;
	};

	int				m_type;
};

struct c_recv_proxy_data {
	const recv_prop*	m_recv_prop;
	d_variant			m_value;
	int					m_element;
	int					m_object_id;
};

enum send_prop_type_t : int {
	DPT_INT = 0,
	DPT_FLOAT,
	DPT_VECTOR,
	DPT_VECTORXY,
	DPT_STRING,
	DPT_ARRAY,
	DPT_DATATABLE,
	DPT_INT64,
	DPT_NUMSENDPROPTYPES
};

class recv_prop {
public:
	char*				m_var_name;
	send_prop_type_t	m_recv_type;
	int					m_flags;
	int					m_string_buffer_size;
	bool				m_inside_array;
	const void*			m_extra_data;
	recv_prop*			m_array_prop;
	void*				m_array_length_proxy;
	recv_var_proxy_fn	m_proxy_fn;
	void*				m_data_table_proxy_fn;
	recv_table*			m_data_table;
	int					m_offset;
	int					m_element_stride;
	int					m_elements;
	const char*			m_parent_array_prop_name;
};

class recv_table {
public:
	recv_prop*		m_props;
	int				m_num_props;
	void*			m_decoder;
	char*			m_net_table_name;
	bool			m_initialized;
	bool			m_in_main_list;
};
