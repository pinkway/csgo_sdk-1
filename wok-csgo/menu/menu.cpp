#include "menu.h"

void c_menu::on_paint() {
	if (!(input::m_blocked = input::get_key<TOGGLE>(VK_INSERT)))
		return;

	const auto screen_size = ImGui::GetIO().DisplaySize;
	
	ImGui::SetNextWindowPos(ImVec2(screen_size.x * 0.5f, screen_size.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));

	ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_Once);

	if (ImGui::Begin(_("wok sdk v2"), 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) {

	}
	ImGui::End();
}