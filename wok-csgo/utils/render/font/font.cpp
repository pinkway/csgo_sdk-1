#include "../../utils.h"

c_font::c_font(std::string font_name, int height, int weight, bool anti_aliased) {
	m_device = render::m_device;
	m_font_name = font_name;
	m_height = height;
	m_weight = weight;
	m_anti_aliased = anti_aliased;

	D3DXCreateFontA(m_device, m_height, 0, m_weight, 0, 0, ANSI_CHARSET, OUT_RASTER_PRECIS, m_anti_aliased ? CLEARTYPE_NATURAL_QUALITY : NONANTIALIASED_QUALITY, DEFAULT_PITCH, m_font_name.c_str(), &m_font);
}

void c_font::paint(const vec2_t& pos, std::wstring txt, const col_t& clr) {
	RECT r;
	SetRect(&r, pos.x, pos.y, pos.x, pos.y);
	m_font->DrawTextW(0, txt.data(), -1, &r, DT_LEFT | DT_NOCLIP, clr.direct());
}

void c_font::invalidate() {
	m_device = nullptr;
	m_font->OnLostDevice();
}

void c_font::restore(IDirect3DDevice9* device) {
	m_device = device;
	m_font->OnResetDevice();
}

const vec2_t c_font::get_size(std::wstring txt) const {
	RECT r;
	m_font->DrawTextW(0, txt.data(), txt.size(), &r, DT_CALCRECT, D3DCOLOR_ARGB(0, 0, 0, 0));
	return vec2_t(r.right - r.left, r.bottom - r.top);
}