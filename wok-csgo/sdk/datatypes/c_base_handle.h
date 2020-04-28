#pragma once

class c_base_handle {
public:
	c_base_handle();
	c_base_handle(const c_base_handle &other);
	c_base_handle(unsigned long value);
	c_base_handle(int entry, int serial_num);

	void init(int entry, int serial_num);
	void term();

	bool is_valid() const;

	int get_entry_index() const;
	int get_serial_number() const;

	int to_int() const;
	bool operator !=(const c_base_handle &other) const;
	bool operator ==(const c_base_handle &other) const;
	bool operator ==(const i_handle_entity* pEnt) const;
	bool operator !=(const i_handle_entity* pEnt) const;
	bool operator <(const c_base_handle &other) const;
	bool operator <(const i_handle_entity* pEnt) const;

	const c_base_handle& operator=(const i_handle_entity *pEntity);
	const c_base_handle& set(const i_handle_entity *pEntity);

	i_handle_entity* get() const;
protected:
	unsigned long  m_index;
};

inline c_base_handle::c_base_handle() { m_index = INVALID_EHANDLE_INDEX; }

inline c_base_handle::c_base_handle(const c_base_handle &other) { m_index = other.m_index; }

inline c_base_handle::c_base_handle(unsigned long value) { m_index = value; }

inline c_base_handle::c_base_handle(int entry, int serial_num) { init(entry, serial_num); }

inline void c_base_handle::init(int entry, int serial_num) { m_index = (unsigned long)(entry | (serial_num << NUM_SERIAL_NUM_SHIFT_BITS)); }

inline void c_base_handle::term() { m_index = INVALID_EHANDLE_INDEX; }

inline bool c_base_handle::is_valid() const { return m_index != INVALID_EHANDLE_INDEX; }

inline int c_base_handle::get_entry_index() const {
	if (!is_valid())
		return NUM_ENT_ENTRIES - 1;

	return m_index & ENT_ENTRY_MASK;
}

inline int c_base_handle::get_serial_number() const { return m_index >> NUM_SERIAL_NUM_SHIFT_BITS; }

inline int c_base_handle::to_int() const { return (int)m_index; }

inline bool c_base_handle::operator !=(const c_base_handle &other) const { return m_index != other.m_index; }

inline bool c_base_handle::operator ==(const c_base_handle &other) const { return m_index == other.m_index; }

inline bool c_base_handle::operator ==(const i_handle_entity* pEnt) const { return get() == pEnt; }

inline bool c_base_handle::operator !=(const i_handle_entity* pEnt) const { return get() != pEnt; }

inline bool c_base_handle::operator <(const c_base_handle &other) const { return m_index < other.m_index; }

inline bool c_base_handle::operator <(const i_handle_entity* entity) const {
	unsigned long other_index = entity ? entity->get_handle().m_index : INVALID_EHANDLE_INDEX;
	return m_index < other_index;
}

inline const c_base_handle& c_base_handle::operator=(const i_handle_entity *pEntity) { return set(pEntity); }

inline const c_base_handle& c_base_handle::set(const i_handle_entity* entity) {
	if (entity)
		*this = entity->get_handle();
	else
		m_index = INVALID_EHANDLE_INDEX;

	return *this;
}