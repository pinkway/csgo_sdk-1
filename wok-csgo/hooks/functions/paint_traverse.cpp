#include "../hooks.h"

void __fastcall hooks::paint_traverse::fn(void* ecx, void* edx, uint32_t id, bool force_repaint, bool allow_force) {
	static const auto original = m_panel->get_original<t>(index);

	original(ecx, id, force_repaint, allow_force);

	{
		static auto draw_panel_id = 0;

		if (!draw_panel_id) {
			if (!strcmp(interfaces::panel->get_name(id), _("FocusOverlayPanel"))) {
				draw_panel_id = id;
			}
			else {
				return;
			}
		}

		if (id != draw_panel_id)
			return;

		static auto should_skip = false;

		if (should_skip = !should_skip)
			return;
	}

	render::begin();
}