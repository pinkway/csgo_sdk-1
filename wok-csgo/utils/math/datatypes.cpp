#include "../utils.h"

vec3_t vec3_t::transform(const matrix3x4_t& in2) const {
	return vec3_t(dot_product(in2[0]) + in2[0][3], dot_product(in2[1]) + in2[1][3], dot_product(in2[2]) + in2[2][3]);
}

vec3_t vec3_t::i_transform(const matrix3x4_t& in2) const {
	return vec3_t(
		(x - in2[0][3]) * in2[0][0] + (y - in2[1][3]) * in2[1][0] + (z - in2[2][3]) * in2[2][0],
		(x - in2[0][3]) * in2[0][1] + (y - in2[1][3]) * in2[1][1] + (z - in2[2][3]) * in2[2][1],
		(x - in2[0][3]) * in2[0][2] + (y - in2[1][3]) * in2[1][2] + (z - in2[2][3]) * in2[2][2]
	);
}

vec3_t vec3_t::rotate(const matrix3x4_t& in2) const {
	return vec3_t(dot_product(in2[0]), dot_product(in2[1]), dot_product(in2[2]));
}

vec3_t vec3_t::i_rotate(const matrix3x4_t& in2) const {
	return vec3_t(
		x * in2[0][0] + y * in2[1][0] + z * in2[2][0],
		x * in2[0][1] + y * in2[1][1] + z * in2[2][1],
		x * in2[0][2] + y * in2[1][2] + z * in2[2][2]
	);
}