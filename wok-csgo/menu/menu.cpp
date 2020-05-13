#include "menu.h"

void c_menu::on_paint() {
	if (!(input::m_blocked = input::get_key(VK_INSERT, TOGGLE)))
		return;

	ImGui::ShowDemoWindow();
}