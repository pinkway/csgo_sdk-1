#include "../utils.h"

vec3_t vec3_t::transform(const matrix3x4_t& in) const {
	return vec3_t(dot_product(in[0]) + in[0][3], dot_product(in[1]) + in[1][3], dot_product(in[2]) + in[2][3]);
}

vec3_t vec3_t::i_transform(const matrix3x4_t& in) const {
	return vec3_t(
		(x - in[0][3]) * in[0][0] + (y - in[1][3]) * in[1][0] + (z - in[2][3]) * in[2][0],
		(x - in[0][3]) * in[0][1] + (y - in[1][3]) * in[1][1] + (z - in[2][3]) * in[2][1],
		(x - in[0][3]) * in[0][2] + (y - in[1][3]) * in[1][2] + (z - in[2][3]) * in[2][2]
	);
}

vec3_t vec3_t::rotate(const matrix3x4_t& in) const {
	return vec3_t(dot_product(in[0]), dot_product(in[1]), dot_product(in[2]));
}

vec3_t vec3_t::i_rotate(const matrix3x4_t& in) const {
	return vec3_t(
		x * in[0][0] + y * in[1][0] + z * in[2][0],
		x * in[0][1] + y * in[1][1] + z * in[2][1],
		x * in[0][2] + y * in[1][2] + z * in[2][2]
	);
}

qangle_t vec3_t::angle() const {
	auto ret = qangle_t();

	if (z == 0.f
		&& x == 0.f) {
		ret.y = 0.f;
		ret.x = z > 0.f ? 90.f : 270.f;
	}
	else {
		ret.y = math::rad_to_deg(math::atan2(y, x));

		if (ret.y < 0.f) {
			ret.y += 360.f;
		}

		ret.x = math::rad_to_deg(math::atan2(-z, length_2d()));

		if (ret.x < 0.f) {
			ret.x += 360.f;
		}
	}

	return ret.sanitize();
}

qangle_t vec3_t::angle(const vec3_t& up) const {
	auto ret = qangle_t();

	const auto left = up.cross_product(*this).normalize();

	const auto dist = length_2d();

	if (dist > 0.001f) {
		ret.x = math::rad_to_deg(math::atan2(-z, dist));
		ret.y = math::rad_to_deg(math::atan2(y, x));
		ret.z = math::rad_to_deg(math::atan2(left.z, left.y * x - left.x * y));
	}
	else {
		ret.x = math::rad_to_deg(math::atan2(-z, dist));
		ret.y = math::rad_to_deg(math::atan2(-left.x, left.y));
		ret.z = 0.f;
	}

	return ret;
}

bool vec3_t::to_screen(vec2_t& value) const {
	static const auto& matrix = *reinterpret_cast<v_matrix*>(*SIG("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9").self_offset(0x3).cast<uintptr_t*>() + 0xB0);

	value.x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3];
	value.y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3];

	const auto w = matrix[3][0] * x + matrix[3][1] * y + matrix[3][2] * z + matrix[3][3];
	if (w < 0.001f)
		return false;

	value /= w;

	value.x = render::m_screen_size.x * 0.5f + (value.x * render::m_screen_size.x) * 0.5f;
	value.y = render::m_screen_size.y * 0.5f - (value.y * render::m_screen_size.y) * 0.5f;

	return true;
}

qangle_t& qangle_t::sanitize() {
	x = math::clamp(remainderf(x, 360.f), -89.f, 89.f);
	y = math::clamp(remainderf(y, 360.f), -180.f, 180.f);
	z = math::clamp(remainderf(z, 360.f), -50.f, 50.f);

	return *this;
}

matrix3x4_t qangle_t::matrix() const {
	vec3_t sin, cos;

	math::sin_cos(math::deg_to_rad(x), sin.x, cos.x);
	math::sin_cos(math::deg_to_rad(y), sin.y, cos.y);
	math::sin_cos(math::deg_to_rad(z), sin.z, cos.z);

	const auto crcy = cos.z * cos.y;
	const auto crsy = cos.z * sin.y;
	const auto srcy = sin.z * cos.y;
	const auto srsy = sin.z * sin.y;

	return matrix3x4_t(
		cos.x * cos.y, sin.x * srcy - crsy, sin.x * crcy + srsy, 0.f,
		cos.x * sin.y, sin.x * srsy + crcy, sin.x * crsy - srcy, 0.f,
		-sin.x, sin.z * cos.x, cos.z * cos.x, 0.f
	);
}

qangle_t matrix3x4_t::angle() const {
	auto ret = qangle_t();

	const auto dist = math::sqrt(m_value[0][0] * m_value[0][0] + m_value[1][0] * m_value[1][0]);

	if (dist > 0.001f) {
		ret.x = math::rad_to_deg(math::atan2(-m_value[2][0], dist));
		ret.y = math::rad_to_deg(math::atan2(m_value[1][0], m_value[0][0]));
		ret.z = math::rad_to_deg(math::atan2(m_value[2][1], m_value[2][2]));
	}
	else {
		ret.x = math::rad_to_deg(math::atan2(-m_value[2][0], dist));
		ret.y = math::rad_to_deg(math::atan2(-m_value[0][1], m_value[1][1]));
		ret.z = 0.f;
	}

	return ret;
}