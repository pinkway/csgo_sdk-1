#include "hooks.h"

namespace hooks {
	void init() {
		// // // // // // // // // // // // // // // // // // // // // // //

		m_d3d_device = std::make_unique<memory::hook_t>(interfaces::d3d_device);

		m_d3d_device->hook(reset::index, reset::fn);
		m_d3d_device->hook(present::index, present::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_client_dll = std::make_unique<memory::hook_t>(interfaces::client_dll);

		m_client_dll->hook(frame_stage_notify::index, frame_stage_notify::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_client_mode = std::make_unique<memory::hook_t>(interfaces::client_mode);

		m_client_mode->hook(create_move::index, create_move::fn);
		m_client_mode->hook(override_view::index, override_view::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_model_render = std::make_unique<memory::hook_t>(interfaces::model_render);

		m_model_render->hook(draw_model_execute::index, draw_model_execute::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_panel = std::make_unique<memory::hook_t>(interfaces::panel);

		m_panel->hook(paint_traverse::index, paint_traverse::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_surface = std::make_unique<memory::hook_t>(interfaces::surface);

		m_surface->hook(lock_cursor::index, lock_cursor::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		static const auto addr = SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C");

		static const auto c_cs_player_vtable = reinterpret_cast<uintptr_t*>(addr + 0x47);
		static const auto i_client_renderable_vtable = reinterpret_cast<uintptr_t*>(addr + 0x4E);

		m_player = std::make_unique<memory::hook_t>(c_cs_player_vtable);

		m_player->hook(eye_angles::index, eye_angles::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		m_renderable = std::make_unique<memory::hook_t>(i_client_renderable_vtable);

		// // // // // // // // // // // // // // // // // // // // // // //
	}

	void undo() {
		m_d3d_device->unhook();
		m_client_dll->unhook();
		m_client_mode->unhook();
		m_player->unhook();
		m_renderable->unhook();
	}

	std::unique_ptr<memory::hook_t> m_d3d_device = nullptr;
	std::unique_ptr<memory::hook_t> m_client_dll = nullptr;
	std::unique_ptr<memory::hook_t> m_client_mode = nullptr;
	std::unique_ptr<memory::hook_t> m_model_render = nullptr;
	std::unique_ptr<memory::hook_t> m_panel = nullptr;
	std::unique_ptr<memory::hook_t> m_surface = nullptr;
	std::unique_ptr<memory::hook_t> m_player = nullptr;
	std::unique_ptr<memory::hook_t> m_renderable = nullptr;
}