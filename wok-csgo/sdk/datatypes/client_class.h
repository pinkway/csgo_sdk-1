#pragma once

class i_client_networkable;
using create_client_class_fn = i_client_networkable*(*)(int, int);
using create_event_fn = i_client_networkable *(*)();

class client_class {
public:
	create_client_class_fn	m_create_fn;
	create_event_fn			m_create_event_fn;
	const char*				m_network_name;
	recv_table*				m_recv_table;
	client_class*			m_next;
	class_id				m_class_id;
};