#pragma once

class i_client_networkable;
typedef i_client_networkable*(*create_client_class_fn)(int, int);
typedef i_client_networkable*(*create_event_fn)();

class client_class {
public:
	create_client_class_fn	m_create_fn;
	create_event_fn			m_create_event_fn;
	const char*				m_network_name;
	c_recv_table*			m_recv_table;
	client_class*			m_next;
	class_id				m_class_id;
};