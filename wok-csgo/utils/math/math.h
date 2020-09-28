#pragma once

namespace math {
	void sin_cos(float rad, float& sin, float& cos);

	__forceinline float clamp(float value, float min, float max) {
		_mm_store_ss(&value, _mm_min_ss(_mm_max_ss(_mm_set_ss(value), _mm_set_ss(min)), _mm_set_ss(max)));
		return value;
	}

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);

	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst);

	float asin(float x);

	float atan2(float y, float x);

	float atan(float x);

	float sin(float x);

	float cos(float x);

	float acos(float x);
}