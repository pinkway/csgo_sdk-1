#pragma once

class i_client_networkable;
using create_client_class_fn = i_client_networkable*(__cdecl*)(int, int);
using create_event_fn = i_client_networkable*(__cdecl*)();

class c_client_class {
public:
	create_client_class_fn	m_create_fn;
	create_event_fn			m_create_event_fn;
	const char*				m_network_name;
	c_recv_table*			m_recv_table;
	c_client_class*			m_next;
	e_class_id				m_class_id;
};