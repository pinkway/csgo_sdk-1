#pragma once

namespace math {
	void sin_cos(float radian, float* sin, float* cos);

	void vector_transform(const vec3_t in1, const matrix3x4_t in2, vec3_t& out);

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr);

	void normalize_angles(qangle_t& angles);

	qangle_t calc_angle(const vec3_t src, const vec3_t dst);

	void vector_angles(const vec3_t& forward, qangle_t& angles);

	float fast_asin(float x);

	float fast_atan2(const float y, const float x);

	float fast_atan(const float x);

	float fast_sin(float x);

	float fast_cos(const float x);

	float fast_acos(float x);

	void fast_rsqrt(float a, float* out);
}