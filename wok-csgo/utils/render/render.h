#pragma once

enum e_font_flags {
	FONT_NONE = 0,
	FONT_CENTERED_X = (1 << 0),
	FONT_CENTERED_Y = (1 << 1),
	FONT_CENTERED = FONT_CENTERED_X | FONT_CENTERED_Y,
	FONT_DROP_SHADOW = (1 << 2),
	FONT_OUTLINE = (1 << 3)
};

namespace render {
	void init();

	void add_to_draw_list();

	void begin();

	void text(const std::string& txt, vec2_t pos, float size, const col_t& clr, ImFont* font, int flags = FONT_NONE);

	void line(const vec2_t& from, const vec2_t& to, const col_t& clr);

	void rect(const vec2_t& pos, const vec2_t& size, const col_t& clr);

	void rect_filled(const vec2_t& pos, const vec2_t& size, const col_t& clr);

	void multi_rect(const std::vector<vec2_t> points, const col_t& clr);

	void multi_rect_filled(const std::vector<vec2_t> points, const col_t& clr);
	
	bool world_to_screen(const vec3_t& in, vec2_t& out);

	extern std::mutex m_mutex;

	extern vec2_t m_screen_size;

	extern std::shared_ptr<ImDrawList> m_draw_list;
	extern std::shared_ptr<ImDrawList> m_temp_draw_list;
}

namespace fonts {
	extern ImFont* m_tahoma14;
}