#include "hooks.h"

namespace hooks {
	void init() {
		// // // // // // // // // // // // // // // // // // // // // // //

		d3d_device = std::make_unique<memory::hook_t>(interfaces::d3d_device);

		d3d_device->hook(reset::index, reset::fn);
		d3d_device->hook(present::index, present::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		client_dll = std::make_unique<memory::hook_t>(interfaces::client_dll);

		client_dll->hook(frame_stage_notify::index, frame_stage_notify::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		client_mode = std::make_unique<memory::hook_t>(interfaces::client_mode);

		client_mode->hook(create_move::index, create_move::fn);
		client_mode->hook(override_view::index, override_view::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		model_render = std::make_unique<memory::hook_t>(interfaces::model_render);

		model_render->hook(draw_model_execute::index, draw_model_execute::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		panel = std::make_unique<memory::hook_t>(interfaces::panel);

		panel->hook(paint_traverse::index, paint_traverse::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		surface = std::make_unique<memory::hook_t>(interfaces::surface);

		surface->hook(lock_cursor::index, lock_cursor::fn);

		// // // // // // // // // // // // // // // // // // // // // // //

		static const auto addr = SIG("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C");

		static const auto c_cs_player_vtable = reinterpret_cast<uintptr_t*>(addr + 0x47);
		static const auto i_client_renderable_vtable = reinterpret_cast<uintptr_t*>(addr + 0x4E);

		c_cs_player_ = std::make_unique<memory::hook_t>(c_cs_player_vtable);

		// // // // // // // // // // // // // // // // // // // // // // //

		i_client_renderable_ = std::make_unique<memory::hook_t>(i_client_renderable_vtable);

		// // // // // // // // // // // // // // // // // // // // // // //
	}

	void undo() {
		d3d_device->unhook();
		client_dll->unhook();
		client_mode->unhook();
		c_cs_player_->unhook();
		i_client_renderable_->unhook();
	}

	std::unique_ptr<memory::hook_t> d3d_device = nullptr;
	std::unique_ptr<memory::hook_t> client_dll = nullptr;
	std::unique_ptr<memory::hook_t> client_mode = nullptr;
	std::unique_ptr<memory::hook_t> model_render = nullptr;
	std::unique_ptr<memory::hook_t> panel = nullptr;
	std::unique_ptr<memory::hook_t> surface = nullptr;
	std::unique_ptr<memory::hook_t> c_cs_player_ = nullptr;
	std::unique_ptr<memory::hook_t> i_client_renderable_ = nullptr;
}