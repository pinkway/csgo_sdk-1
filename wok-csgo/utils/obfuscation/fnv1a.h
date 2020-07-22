#pragma once
#include <array>

template<typename T, T value>
struct constant_holder_t {
	enum class c_value_holder : T {
		m_value = value
	};
};

#define constant(value) ((decltype(value))constant_holder_t<decltype(value), value>::c_value_holder::m_value)

constexpr auto seed = 0x45C3370D;
constexpr auto prime = 0x1000193;

__forceinline uint32_t fnv1a_fl_rt(const char* key, uint32_t length) {
	auto hash = seed;

	for (int i = 0; i < length; i++) {
		hash ^= key[i];
		hash *= prime;
	}

	return hash;
}

__forceinline uint32_t fnv1a_rt(const char* key) { return fnv1a_fl_rt(key, strlen(key)); }

constexpr uint32_t fnv1a_ct(const char* str, uint32_t value = seed) noexcept { return !*str ? value : fnv1a_ct(str + 1, static_cast<unsigned>(1ull * (value ^ static_cast<uint8_t>(*str)) * prime)); }

#define fnv1a(txt) constant(fnv1a_ct(txt))
