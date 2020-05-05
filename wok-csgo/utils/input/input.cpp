#include "../utils.h"

namespace input {
	void init() {
		while (!m_hwnd)
			m_hwnd = FindWindowA(_("Valve001"), 0);

		m_original_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongA(m_hwnd, GWL_WNDPROC, reinterpret_cast<long>(wnd_proc)));
	}

	void undo() {
		SetWindowLongPtrA(FindWindowA(_("Valve001"), 0), GWL_WNDPROC, reinterpret_cast<long>(m_original_wnd_proc));
	}

	long __stdcall wnd_proc(HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param) {
		switch (msg) {
		case WM_MOUSEMOVE:
			m_mouse_pos = vec2_t(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));
			break;
		case WM_MOUSEWHEEL:
			m_scroll = HIWORD(w_param);
			break;
		}

		if (m_blocked)
			return true;

		return CallWindowProcA(m_original_wnd_proc, hwnd, msg, w_param, l_param);
	}

	bool get_key(uint32_t key, int mode) {
		switch (mode) {
		case ON_KEY:
			return HIWORD(GetKeyState(key));
			break;
		case TOGGLE:
			return LOWORD(GetKeyState(key));
			break;
		case AWAY_KEY:
			return !HIWORD(GetKeyState(key));
			break;
		}

		return false;
	}

	bool is_mouse_in_region(vec2_t pos, vec2_t size) {
		return m_mouse_pos.x > pos.x && m_mouse_pos.y > pos.y && m_mouse_pos.x < pos.x + size.x && m_mouse_pos.y < pos.y + size.y;
	}
	
	bool is_mouse_in_region(int x, int y, int w, int h) {
		return m_mouse_pos.x > x && m_mouse_pos.y > y && m_mouse_pos.x < x + w && m_mouse_pos.y < y + h;
	}

	float m_scroll;
	bool m_blocked;
	vec2_t m_mouse_pos;

	HWND m_hwnd;
	WNDPROC m_original_wnd_proc;
}