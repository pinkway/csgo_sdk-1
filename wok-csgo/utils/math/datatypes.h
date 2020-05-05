#pragma once

class vec2_t {
public:
	vec2_t() = default;
	vec2_t(float ix, float iy) { x = ix; y = iy; }

	float x = 0.f, y = 0.f;

	vec2_t operator+(const vec2_t& in) const {
		return vec2_t(x + in.x, y + in.y);
	}

	vec2_t operator-(const vec2_t& in) const {
		return vec2_t(x - in.x, y - in.y);
	}

	vec2_t operator+(const float in) const {
		return vec2_t(x + in, y + in);
	}

	vec2_t operator-(const float in) const {
		return vec2_t(x - in, y - in);
	}

	vec2_t operator/(const float in) const {
		return vec2_t(x / in, y / in);
	}

	vec2_t operator*(const float in) const {
		return vec2_t(x * in, y * in);
	}

	vec2_t& operator-=(const vec2_t& v) {
		x -= v.x;
		y -= v.y;

		return *this;
	}

	vec2_t& operator+=(const vec2_t& v) {
		x += v.x;
		y += v.y;

		return *this;
	}

	vec2_t& operator/=(const float in) {
		x /= in;
		y /= in;

		return *this;
	}

	vec2_t& operator*=(const float in) {
		x *= in;
		y *= in;

		return *this;
	}

	vec2_t operator-() const {
		return vec2_t(-x, -y);
	}

	void normalize() {
		auto l = length();

		x /= l;
		y /= l;
	}

	vec2_t& normalized() {
		normalize();
		return *this;
	}

	bool operator==(const vec2_t& in) const {
		return x == in.x && y == in.y;
	}

	bool operator!=(const vec2_t& in) const {
		return !(operator==(in));
	}

	float length_sqr() const {
		return (x * x) + (y * y);
	}

	float length() const {
		return fast_sqrt(length_sqr());
	}

	float dot_product(vec2_t in) const {
		return (x * in.x) + (y * in.y);
	}

	float dot_product(const float* in) const {
		return (x * in[0]) + (y * in[1]);
	}

	float dist_to(vec2_t in) const {
		return (*this - in).length();
	}

	bool is_valid() const {
		return std::isfinite(x) && std::isfinite(y);
	}

	bool empty() const {
		return x == 0.f && y == 0.f;
	}
};

class vec3_t {
public:
	vec3_t() = default;
	vec3_t(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }

	float x = 0.f, y = 0.f, z = 0.f;

	vec3_t operator+(const vec3_t& in) const {
		return vec3_t(x + in.x, y + in.y, z + in.z);
	}

	vec3_t operator-(const vec3_t& in) const {
		return vec3_t(x - in.x, y - in.y, z - in.z);
	}

	vec3_t operator-(const float in) const{
		return vec3_t(x - in, y - in, z - in);
	}

	vec3_t operator+(const float in) const {
		return vec3_t(x + in, y + in, z + in);
	}

	vec3_t operator/(const float in) const {
		return vec3_t(x / in, y / in, z / in);
	}

	vec3_t operator*(const float in) const {
		return vec3_t(x * in, y * in, z * in);
	}

	vec3_t operator-() const {
		return vec3_t(-x, -y, -z);
	}

	vec3_t& operator-=(const vec3_t& in) {
		x -= in.x;
		y -= in.y;
		z -= in.z;

		return *this;
	}

	vec3_t& operator+=(const vec3_t& in) {
		x += in.x;
		y += in.y;
		z += in.z;

		return *this;
	}

	vec3_t& operator/=(const float in) {
		x /= in;
		y /= in;
		z /= in;

		return *this;
	}

	vec3_t& operator*=(const float in) {
		x *= in;
		y *= in;
		z *= in;

		return *this;
	}

	void normalize() {
		auto l = length();

		x /= l;
		y /= l;
		z /= l;
	}

	vec3_t& normalized() {
		normalize();
		return *this;
	}

	bool operator==(const vec3_t& in) const {
		return x == in.x && y == in.y && z == in.z;
	}

	bool operator!=(const vec3_t& in) const {
		return !(operator==(in));
	}

	void make_absolute() {
		x = abs(x);
		y = abs(y);
		z = abs(z);
	}

	float length_sqr() const {
		return (x * x) + (y * y) + (z * z);
	}

	float length() const {
		return fast_sqrt(length_sqr());
	}

	float length_2d() const {
		return fast_sqrt((x * x) + (y * y));
	}

	float dot_product(const vec3_t in) const {
		return (x * in.x) + (y * in.y) + (z * in.z);
	}

	float dot_product(const float* in) const {
		return (x * in[0]) + (y * in[1]) + (z * in[2]);
	}

	vec3_t cross_product(const vec3_t in) const {
		return vec3_t(y * in.z - z * in.y, z * in.x - x * in.z, x * in.y - y * in.x);
	}

	float dist_to(const vec3_t in) const {
		return (*this - in).length();
	}

	float dist_to_2d(const vec3_t in) const {
		return (*this - in).length_2d();
	}

	bool is_valid() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}

	bool empty() const {
		return x == 0.f && y == 0.f && z == 0.f;
	}
};

class ALIGN16 vector_aligned : public vec3_t {
public:
	vector_aligned() = default;
	vector_aligned(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }
	vector_aligned(const vec3_t& in) { x = in.x; y = in.y; z = in.z; }

	vector_aligned& operator=(const vec3_t& in) {
		x = in.x; 
		y = in.y; 
		z = in.z;

		return *this;
	}

	vector_aligned& operator=(const vector_aligned& in)  {
		x = in.x;
		y = in.y;
		z = in.z;

		return *this;
	}

	float w;
};

class qangle_t {
public:
	qangle_t() = default;
	qangle_t(float ix, float iy, float iz) { x = ix; y = iy; z = iz; }

	float x = 0.f, y = 0.f, z = 0.f;

	qangle_t operator+(const qangle_t& in) const {
		return qangle_t(x + in.x, y + in.y, z + in.z);
	}

	qangle_t operator-(const qangle_t& in) const {
		return qangle_t(x - in.x, y - in.y, z - in.z);
	}

	qangle_t operator-(const float in) const {
		return qangle_t(x - in, y - in, z - in);
	}

	qangle_t operator+(const float in) const {
		return qangle_t(x + in, y + in, z + in);
	}

	qangle_t operator/(const float in) const {
		return qangle_t(x / in, y / in, z / in);
	}

	qangle_t operator*(const float in) const {
		return qangle_t(x * in, y * in, z * in);
	}

	qangle_t& operator-=(const qangle_t& in) {
		x -= in.x;
		y -= in.y;
		z -= in.z;

		return *this;
	}

	qangle_t& operator+=(const qangle_t& in) {
		x += in.x;
		y += in.y;
		z += in.z;

		return *this;
	}

	qangle_t& operator/=(const float in) {
		x /= in;
		y /= in;
		z /= in;

		return *this;
	}

	qangle_t& operator*=(const float in) {
		x *= in;
		y *= in;
		z *= in;

		return *this;
	}

	void normalize() {
		auto l = length();

		x /= l;
		y /= l;
		z /= l;
	}

	qangle_t& normalized() {
		normalize();
		return *this;
	}

	bool operator==(const qangle_t& in) const {
		return x == in.x && y == in.y && z == in.z;
	}

	bool operator!=(const qangle_t& in) const {
		return !(operator==(in));
	}

	float length_sqr() const {
		return (x * x) + (y * y) + (z * z);
	}

	float length() const {
		return fast_sqrt(length_sqr());
	}

	float length_2d() const {
		return fast_sqrt((x * x) + (y * y));
	}

	bool is_valid() const {
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}

	bool empty() const {
		return x == 0.f && y == 0.f && z == 0.f;
	}
};

struct col_t {
	col_t() = default;
	col_t(int r, int g, int b) { set(r, g, b, 255); }
	col_t(int r, int g, int b, int a) { set(r, g, b, a); }
	col_t(const col_t& col, int a) { set(col.r(), col.g(), col.b(), a); }
	col_t(int a) { set(clr[0], clr[1], clr[2], a); }

	std::array<uint8_t, 4> clr = {};

	void set(int r, int g, int b, int a) {
		clr[0] = static_cast<uint8_t>(r);
		clr[1] = static_cast<uint8_t>(g);
		clr[2] = static_cast<uint8_t>(b);
		clr[3] = static_cast<uint8_t>(a);
	}

	inline int r() const {
		return clr[0];
	}

	inline int g() const {
		return clr[1];
	}

	inline int b() const {
		return clr[2];
	}

	inline int a() const {
		return clr[3];
	}

	D3DCOLOR direct() const {
		return D3DCOLOR_ARGB(a(), r(), g(), b());
	}

	col_t& operator=(const col_t& in) {
		set(in.r(), in.g(), in.b(), in.a());
		return *this;
	}

	col_t& operator-=(const uint8_t in) {
		set(r() - in, g() - in, b() - in, a() - in);
		return *this;
	}

	col_t& operator+=(const uint8_t in) {
		set(r() + in, g() + in, b() + in, a() + in);
		return *this;
	}

	col_t& operator/=(const uint8_t in) {
		set(r() / in, g() / in, b() / in, a() / in);
		return *this;
	}

	col_t& operator*=(const uint8_t in) {
		set(r() * in, g() * in, b() * in, a() * in );
		return *this;
	}

	col_t& operator-=(const col_t& in) {
		set(r() - in.r(), g() - in.g(), b() - in.b(), a() - in.a());
		return *this;
	}

	col_t& operator+=(const col_t& in) {
		set(r() + in.r(), g() + in.g(), b() + in.b(), a() + in.a());
		return *this;
	}

	col_t operator-(const uint8_t in) const {
		return col_t(r() - in, g() - in, b() - in, a() - in);
	}

	col_t operator+(const uint8_t in) const {
		return col_t(r() + in, g() + in, b() + in, a() + in);
	}

	col_t operator/(const uint8_t in) const {
		return col_t(r() / in, g() / in, b() / in, a() / in);
	}

	col_t operator*(const uint8_t in) const {
		return col_t(r() * in, g() * in, b() * in, a() * in);
	}

	col_t operator-(const col_t& in) const {
		return col_t(r() - in.r(), g() - in.g(), b() - in.b(), a() - in.a());
	}

	col_t operator+(const col_t& in) const {
		return col_t(r() + in.r(), g() + in.g(), b() + in.b(), a() + in.a());
	}

	bool operator==(const col_t& in) const {
		return *const_cast<col_t*>(this) == *const_cast<col_t*>(&in);
	}

	bool operator!=(const col_t& in) const {
		return !(operator==(in));
	}

	float hue() const {
		auto r = clr[0] / 255.f;
		auto g = clr[1] / 255.f;
		auto b = clr[2] / 255.f;

		auto mx = max(r, max(g, b));
		auto mn = min(r, min(g, b));
		if (mx == mn)
			return 0.f;

		auto delta = mx - mn;

		auto hue = 0.f;
		if (mx == r)
			hue = (g - b) / delta;
		else if (mx == g)
			hue = 2.f + (b - r) / delta;
		else
			hue = 4.f + (r - g) / delta;

		hue *= 60.f;
		if (hue < 0.f)
			hue += 360.f;

		return hue / 360.f;
	}

	float saturation() const {
		auto r = clr[0] / 255.f;
		auto g = clr[1] / 255.f;
		auto b = clr[2] / 255.f;

		auto mx = max(r, max(g, b));
		auto mn = min(r, min(g, b));

		auto delta = mx - mn;

		if (mx == 0.f)
			return delta;

		return delta / mx;
	}

	float brightness() const {
		auto r = clr[0] / 255.f;
		auto g = clr[1] / 255.f;
		auto b = clr[2] / 255.f;

		return max(r, max(g, b));
	}

	static col_t from_hsb(float hue, float saturation, float brightness) {
		auto h = hue == 1.f ? 0 : hue * 6.f;
		auto f = h - (int)h;
		auto p = brightness * (1.f - saturation);
		auto q = brightness * (1.f - saturation * f);
		auto t = brightness * (1.f - (saturation * (1.f - f)));

		if (h < 1) {
			return col_t(
				(uint8_t)(brightness * 255),
				(uint8_t)(t * 255),
				(uint8_t)(p * 255)
			);
		}
		if (h < 2) {
			return col_t(
				(uint8_t)(q * 255),
				(uint8_t)(brightness * 255),
				(uint8_t)(p * 255)
			);
		}
		if (h < 3) {
			return col_t(
				(uint8_t)(p * 255),
				(uint8_t)(brightness * 255),
				(uint8_t)(t * 255)
			);
		}
		if (h < 4) {
			return col_t(
				(uint8_t)(p * 255),
				(uint8_t)(q * 255),
				(uint8_t)(brightness * 255)
			);
		}
		if (h < 5) {
			return col_t(
				(uint8_t)(t * 255),
				(uint8_t)(p * 255),
				(uint8_t)(brightness * 255)
			);
		}
		return col_t(
			(uint8_t)(brightness * 255),
			(uint8_t)(p * 255),
			(uint8_t)(q * 255)
		);
	}

	struct palette_t {
		static col_t red(int alpha = 255) { return col_t(255, 0, 0, alpha); }
		static col_t green(int alpha = 255) { return col_t(0, 255, 0, alpha); }
		static col_t blue(int alpha = 255) { return col_t(0, 0, 255, alpha); }

		static col_t white(int alpha = 255) { return col_t(255, 255, 255, alpha); }
		static col_t black(int alpha = 255) { return col_t(13, 13, 13, alpha); }
		static col_t grey(int alpha = 255) { return col_t(60, 60, 60, alpha); }

		static col_t light_black(int alpha = 255) { return col_t(32, 32, 32, alpha); }
		static col_t light_grey(int alpha = 255) { return col_t(77, 77, 77, alpha); }
		static col_t light_red(int alpha = 255) { return col_t(236, 98, 95, alpha); }
		static col_t light_blue(int alpha = 255) { return col_t(0, 102, 204, alpha); }

		static col_t dark_grey(int alpha = 255) { return col_t(44, 44, 44, alpha); }

		static col_t purple(int alpha = 255) { return col_t(220, 0, 220, alpha); }
		static col_t pink(int alpha = 255) { return col_t(255, 105, 180, alpha); }
	};
};

struct matrix3x4_t {
	matrix3x4_t() = default;
	matrix3x4_t(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23) {
		m_matrix[0][0] = m00;
		m_matrix[0][1] = m01;
		m_matrix[0][2] = m02;
		m_matrix[0][3] = m03;
		m_matrix[1][0] = m10;
		m_matrix[1][1] = m11;
		m_matrix[1][2] = m12;
		m_matrix[1][3] = m13;
		m_matrix[2][0] = m20;
		m_matrix[2][1] = m21;
		m_matrix[2][2] = m22;
		m_matrix[2][3] = m23;
	}
	matrix3x4_t(const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& vec_origin) { init(x_axis, y_axis, z_axis, vec_origin); }

	void init(const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& vec_origin) {
		m_matrix[0][0] = x_axis.x;
		m_matrix[0][1] = y_axis.x;
		m_matrix[0][2] = z_axis.x;
		m_matrix[0][3] = vec_origin.x;
		m_matrix[1][0] = x_axis.y;
		m_matrix[1][1] = y_axis.y;
		m_matrix[1][2] = z_axis.y;
		m_matrix[1][3] = vec_origin.y;
		m_matrix[2][0] = x_axis.z;
		m_matrix[2][1] = y_axis.z;
		m_matrix[2][2] = z_axis.z;
		m_matrix[2][3] = vec_origin.z;
	}

	float* operator[](int i) {
		return m_matrix[i];
	}

	const float* operator[](int i) const {
		return m_matrix[i];
	}

	float m_matrix[3][4] = {};
};

struct v_matrix {
	float* operator[](int i) {
		return m_matrix[i];
	}

	const float* operator[](int i) const {
		return m_matrix[i];
	}

	float m_matrix[4][4] = {};
};
