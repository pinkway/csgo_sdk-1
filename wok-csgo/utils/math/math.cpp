#include "../utils.h"

namespace math {
	void sin_cos(float radian, float& sin, float& cos) {
		sin = std::sin(radian);
		cos = std::cos(radian);
	}

	void vector_transform(const vec3_t in1, const matrix3x4_t in2, vec3_t& out) {
		out.x = in1.dot_product(in2.m_matrix[0]) + in2.m_matrix[0][3];
		out.y = in1.dot_product(in2.m_matrix[1]) + in2.m_matrix[1][3];
		out.z = in1.dot_product(in2.m_matrix[2]) + in2.m_matrix[2][3];
	}

	void angle_vectors(const qangle_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
		vec3_t cos, sin;
		sin_cos(DEG2RAD(angles.x), sin.x, cos.x);
		sin_cos(DEG2RAD(angles.y), sin.y, cos.y);
		sin_cos(DEG2RAD(angles.z), sin.z, cos.z);

		if (forward) {
			forward->x = cos.x * cos.y;
			forward->y = cos.x * sin.y;
			forward->z = -sin.x;
		}

		if (right) {
			right->x = -sin.z * sin.x * cos.y + -cos.z * -sin.y;
			right->y = -sin.z * sin.x * sin.y + -cos.z * cos.y;
			right->z = -sin.z * cos.x;
		}

		if (up) {
			up->x = cos.z * sin.x * cos.y + -sin.z * -sin.y;
			up->y = cos.z * sin.x * sin.y + -sin.z * cos.y;
			up->z = cos.z * cos.x;
		}
	}

	void normalize_angles(qangle_t& angles) {
		angles.x = std::clamp(remainderf(angles.x, 360.f), -89.f, 89.f);
		angles.y = std::clamp(remainderf(angles.y, 360.f), -180.f, 180.f);
		angles.z = std::clamp(remainderf(angles.z, 360.f), -45.f, 45.f);
	}

	qangle_t calc_angle(const vec3_t src, const vec3_t dst) {
		auto delta = src - dst;
		if (delta.empty())
			return qangle_t();

		auto len = delta.length();

		if (delta.z == 0.f && len == 0.f
			|| delta.y == 0.f && delta.x == 0.f)
			return qangle_t();

		qangle_t angles;
		angles.x = (fast_asin(delta.z / delta.length()) * M_RADPI);
		angles.y = (fast_atan(delta.y / delta.x) * M_RADPI);
		angles.z = 0.f;

		if (delta.x >= 0.f)
			angles.y += 180.f;

		normalize_angles(angles);

		return angles;
	}

	void vector_angles(const vec3_t& forward, qangle_t& angles) {
		float yaw, pitch;
		if (forward.z == 0.f && forward.x == 0.f) {
			yaw = 0;

			if (forward.z > 0.f)
				pitch = 90.f;
			else
				pitch = 270.f;
		}
		else {
			yaw = (fast_atan2(forward.y, forward.x) * 180.f / M_PI);

			if (yaw < 0.f)
				yaw += 360.f;

			auto sqin = forward.x * forward.x + forward.y * forward.y;

			pitch = (fast_atan2(-forward.z, fast_sqrt(sqin)) * 180.f / M_PI);

			if (pitch < 0.f)
				pitch += 360.f;
		}

		pitch -= floorf(pitch / 360.f + 0.5f) * 360.f;
		yaw -= floorf(yaw / 360.f + 0.5f) * 360.f;

		if (pitch > 89.f)
			pitch = 89.f;
		else if (pitch < -89.f)
			pitch = -89.f;

		angles.x = pitch;
		angles.y = yaw;
		angles.z = 0;
	}

	float fast_asin(float x) {
		const auto negate = float(x < 0);
		x = abs(x);
		auto ret = -0.0187293;
		ret *= x;
		ret += 0.0742610;
		ret *= x;
		ret -= 0.2121144;
		ret *= x;
		ret += 1.5707288;
		ret = 3.14159265358979 * 0.5 - sqrt(1.0 - x)*ret;
		return float(ret - 2 * negate * ret);
	}

	float fast_atan2(const float y, const float x) {
		const auto xabs = fabs(x);
		const auto yabs = fabs(y);
		double t3 = xabs;
		double t1 = yabs;
		auto t0 = max(t3, t1);
		t1 = min(t3, t1);
		t3 = 1 / t0;
		t3 = t1 * t3;

		const auto t4 = t3 * t3;
		t0 = -0.013480470;
		t0 = t0 * t4 + 0.057477314;
		t0 = t0 * t4 - 0.121239071;
		t0 = t0 * t4 + 0.195635925;
		t0 = t0 * t4 - 0.332994597;
		t0 = t0 * t4 + 0.999995630;
		t3 = t0 * t3;

		t3 = (yabs > xabs) ? 1.570796327 - t3 : t3;
		t3 = (x < 0) ? 3.141592654 - t3 : t3;
		t3 = (y < 0) ? -t3 : t3;

		return float(t3);
	}

	float fast_atan(const float x) {
		return fast_atan2(x, float(1));
	}

	float fast_sin(float x) {
		x *= float(0.159155);
		x -= floor(x);
		const auto xx = x * x;
		auto y = -6.87897;
		y = y * xx + 33.7755;
		y = y * xx - 72.5257;
		y = y * xx + 80.5874;
		y = y * xx - 41.2408;
		y = y * xx + 6.28077;
		return float(x * y);
	}

	float fast_cos(const float x) {
		return fast_sin(x + 1.5708f);
	}

	float fast_acos(float x) {
		const auto negate = float(x < 0);
		x = abs(x);
		auto ret = float(-0.0187293);
		ret = ret * x;
		ret = ret + 0.0742610;
		ret = ret * x;
		ret = ret - 0.2121144;
		ret = ret * x;
		ret = ret + 1.5707288;
		ret = ret * sqrt(1.0 - x);
		ret = ret - 2 * negate * ret;
		return negate * 3.14159265358979 + ret;
	}

	void fast_rsqrt(float a, float* out) {
		const auto xx = _mm_load_ss(&a);
		auto xr = _mm_rsqrt_ss(xx);
		auto xt = _mm_mul_ss(xr, xr);
		xt = _mm_mul_ss(xt, xx);
		xt = _mm_sub_ss(_mm_set_ss(3.f), xt);
		xt = _mm_mul_ss(xt, _mm_set_ss(0.5f));
		xr = _mm_mul_ss(xr, xt);
		_mm_store_ss(out, xr);
	}
}