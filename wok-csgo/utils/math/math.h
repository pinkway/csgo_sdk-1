#pragma once

namespace math {
	void sin_cos(float rad, float& sin, float& cos);

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);

	qangle_t calc_angle(const vec3_t& src, const vec3_t& dst);

	float clamp(float value, float min, float max);

	float fast_asin(float x);

	float fast_atan2(const float y, const float x);

	float fast_atan(const float x);

	float fast_sin(float x);

	float fast_cos(const float x);

	float fast_acos(float x);

	void fast_rsqrt(float a, float* out);
}