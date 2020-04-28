#pragma once

enum font_flags {
	FONT_CENTERED_X = (1 << 1),
	FONT_CENTERED_Y = (1 << 2),
	FONT_CENTERED = FONT_CENTERED_X | FONT_CENTERED_Y,
	FONT_DROPSHADOW = (1 << 3),
	FONT_OUTLINE = (1 << 4),
};

class c_font {
private:
	LPD3DXFONT m_font;
	IDirect3DDevice9* m_device;

	bool m_anti_aliased;
	int m_height, m_weight;
	std::string m_font_name;
public:
	c_font(std::string font_name, int height, int weight, bool anti_aliased = false);
	~c_font() = default;

	void paint(const vec2_t& pos, std::wstring txt, const col_t& clr);

	void invalidate();
	void restore(IDirect3DDevice9* device);

	const vec2_t get_size(std::wstring txt) const;
};