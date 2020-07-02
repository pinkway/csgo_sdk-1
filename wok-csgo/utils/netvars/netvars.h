#pragma once
#include "../../sdk/datatypes/recv_prop.h"

namespace netvars {
	struct data_t {
		recv_prop* m_prop;
		uint32_t m_offset;
	};

	void init();

	void dump_recursive(const char* base_class, recv_table* table, uint32_t offset);

	uint32_t get_offset(uint32_t hash);

	recv_prop* get_prop(uint32_t hash);

	__declspec(noinline) static uint32_t get_offset_by_hash(uint32_t hash) { return get_offset(hash); }

	extern std::unordered_map<uint32_t, data_t> m_props;
};

#define NETVAR(func, type, name) \
	type& func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = netvars::get_offset_by_hash(hash); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
	}

#define ANETVAR(func, type, size, name) \
	std::array<type, size>& func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = netvars::get_offset_by_hash(hash); \
		return *reinterpret_cast<std::array<type, size>*>(reinterpret_cast<uintptr_t>(this) + offset); \
	}

#define PNETVAR(func, type, name) \
	type* func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = netvars::get_offset_by_hash(hash); \
		return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
	}

#define NETVAR_OFFSET(func, type, name, add) \
	type& func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = netvars::get_offset_by_hash(hash); \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset + add); \
	}

#define MNETVAR(func, type, name, modifier) \
	type& func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto offset = netvars::get_offset_by_hash(hash); \
		return **reinterpret_cast<type**>(reinterpret_cast<uintptr_t>(this) + offset * modifier); \
	}

#define NETPROP(func, name) \
	static recv_prop* func { \
		static const auto hash = fnv1a_rt(name); \
		static const auto prop = netvars::get_prop(hash); \
		return prop; \
	}

#define OFFSET(func, type, offset) \
	type& func { \
		static const auto offset_ = offset; \
		return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset_); \
	}

#define POFFSET(func, type, offset) \
	type* func { \
		static const auto offset_ = offset; \
		return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset_); \
	}

#define PPOFFSET(func, type, offset) \
	type& func { \
		static const auto offset_ = offset; \
		return **reinterpret_cast<type**>(reinterpret_cast<uintptr_t>(this) + offset_); \
	}

#define MOFFSET(func, type, offset, modifier) \
	type& func { \
		static const auto offset_ = offset; \
		return **reinterpret_cast<type**>(reinterpret_cast<uintptr_t>(this) + offset_ * modifier); \
	}