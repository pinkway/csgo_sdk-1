#pragma once

enum {
	ON_KEY,
	TOGGLE,
	AWAY_KEY
};

namespace input {
	void init();
	void undo();

	long __stdcall wnd_proc(HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param);

	bool get_key(uint32_t key, int mode);
	bool is_mouse_in_region(vec2_t pos, vec2_t size);
	bool is_mouse_in_region(int x, int y, int w, int h);

	extern float m_scroll;
	extern bool m_blocked;
	extern vec2_t m_mouse_pos;

	extern HWND m_hwnd;
	extern WNDPROC m_original_wnd_proc;
};