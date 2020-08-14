#pragma once

namespace random {
	constexpr auto m_time = __TIME__;
	constexpr auto m_seed = static_cast<unsigned>(m_time[7]) + static_cast<unsigned>(m_time[6]) * 10 + static_cast<unsigned>(m_time[4]) * 60 + static_cast<unsigned>(m_time[3]) * 600 + static_cast<unsigned>(m_time[1]) * 3600 + static_cast<unsigned>(m_time[0]) * 36000;

	template <int N>
	struct gen_t {
	private:
		static constexpr unsigned m_a = 16807;
		static constexpr unsigned m_b = 2147483647;

		static constexpr unsigned m_s = gen_t<N - 1>::m_value;
		static constexpr unsigned m_lo0 = m_a * (m_s & 0xFFFFu);
		static constexpr unsigned m_hi0 = m_a * (m_s >> 16u);
		static constexpr unsigned m_lo1 = m_lo0 + ((m_hi0 & 0x7FFFu) << 16u);
		static constexpr unsigned m_hi1 = m_hi0 >> 15u;
		static constexpr unsigned m_lo2 = m_lo1 + m_hi0;
	public:
		static constexpr unsigned m_max = m_b;
		static constexpr unsigned m_value = m_lo2 > m_b ? m_lo2 - m_b : m_lo2;
	};

	template <>
	struct gen_t<0> {
		static constexpr unsigned m_value = m_seed;
	};

	template <int N, int M>
	struct int_t {
		static constexpr auto m_value = gen_t<N + 1>::m_value % M;
	};

	template <int N>
	struct char_t {
		static const char m_value = static_cast<char>(1 + int_t<N, 0x7F - 1>::m_value);
	};
}

template <int N, char K>
struct xor_str_t {
private:
	__forceinline char dec(char c) const { return c ^ K; }
	constexpr __forceinline char enc(char c) const { return c ^ K; }
public:
	template <int... S>
	constexpr __forceinline xor_str_t(const char* str, std::index_sequence<S...>) { m_encrypted = { enc(str[S])... }; }

	__forceinline std::string dec() const {
		auto ret = std::string(N, 0);

		for (int i = 0; i < N; i++) {
			ret.at(i) = dec(m_encrypted.at(i));
		}

		return ret;
	}

	__forceinline std::string ot(bool decrypt = true) {
		auto ret = std::string(N, 0);

		for (int i = 0; i < N; i++) {
			ret.at(i) = decrypt ? dec(m_encrypted.at(i)) : m_encrypted.at(i);

			m_encrypted.at(i) = 0;
		}

		return ret;
	}

	std::array<char, N> m_encrypted = {};
};

#define _(s) xor_str_t<sizeof(s), random::char_t<__COUNTER__>::m_value>(s, std::make_index_sequence<sizeof(s)>()).dec().c_str()
#define _ot(s) xor_str_t<sizeof(s), random::char_t<__COUNTER__>::m_value>(s, std::make_index_sequence<sizeof(s)>()).ot().c_str()
#define __(s) []() -> std::pair<std::string, char> { \
	constexpr auto key = random::char_t<__COUNTER__>::m_value; \
	return std::make_pair(xor_str_t<sizeof(s), key>(s, std::make_index_sequence<sizeof(s)>()).ot(false), key); \
}()
#define _rt(n, s) auto (n) = reinterpret_cast<char*>(alloca(((s).first.size() + 1) * sizeof(char))); \
	for (int i = 0; i < (s).first.size(); i++) \
        (n)[i] = (s).first[i] ^ (s).second; \
    (n)[(s).first.size()] = 0