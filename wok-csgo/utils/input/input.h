#pragma once

enum {
	ON_KEY,
	TOGGLE,
	AWAY_KEY,
};

namespace input {
	void init();
	void undo();

	long __stdcall wnd_proc(HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param);

	bool get_key(uint32_t key, int mode);

	extern bool m_blocked;

	extern HWND m_hwnd;
	extern WNDPROC m_original_wnd_proc;
};