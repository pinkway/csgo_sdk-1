#pragma once

struct vertex_t {
	float x, y, z, rhw;
	uint32_t clr;
};

namespace render {
	void init(IDirect3DDevice9* device);

	void restore(IDirect3DDevice9* device);
	void invalidate();

	void set_render_states();

	void line(int x, int y, int x1, int y1, const col_t& clr);
	void line(const vec2_t& from, const vec2_t& to, const col_t& clr);

	void rect(int x, int y, int w, int h, const col_t& clr);
	void rect(const vec2_t& pos, const vec2_t& size, const col_t& clr);

	void filled_rect(int x, int y, int w, int h, const col_t& clr);
	void filled_rect(const vec2_t& pos, const vec2_t& size, const col_t& clr);

	void gradient_rect(int x, int y, int w, int h, const col_t& clr, const col_t& clr2, bool horizontal = false);
	void gradient_rect(const vec2_t& pos, const vec2_t& size, const col_t& clr, const col_t& clr2, bool horizontal = false);

	void gradient_filled_rect(int x, int y, int w, int h, const col_t& clr, const col_t& clr2, bool horizontal = false);
	void gradient_filled_rect(const vec2_t& pos, const vec2_t& size, const col_t& clr, const col_t& clr2, bool horizontal = false);

	void text(std::shared_ptr<c_font>& font, const vec2_t& pos, std::wstring txt, int flags, const col_t& clr = col_t::palette_t::white());
	void text(std::shared_ptr<c_font>& font, const vec2_t& pos, std::string txt, int flags, const col_t& clr = col_t::palette_t::white());

	void filled_triangle(POINT pos1, POINT pos2, POINT pos3, const col_t& clr);

	bool world_to_screen(const vec3_t& in, vec2_t& out);

	extern bool m_init;
	extern IDirect3DDevice9* m_device;

	extern vec2_t m_screen_size;
	extern std::vector<std::shared_ptr<c_font>> m_fonts;
}

namespace fonts {
	extern std::shared_ptr<c_font> m_tahoma12;
}