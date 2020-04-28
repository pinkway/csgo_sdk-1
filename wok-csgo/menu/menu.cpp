#include "menu.h"

void c_menu::on_paint() {
	render::text(fonts::m_tahoma12, render::m_screen_size / 2, (int)interfaces::global_vars->m_realtime % 2 ? _("wok sdk v2 :sunglasses:") : _("-_____________-"), FONT_CENTERED);
}