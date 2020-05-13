#pragma once
#include "../globals.h"
#include "../features/features.h"

namespace hooks {
	void init();
	void undo();

	extern std::unique_ptr<memory::hook_t> d3d_device;
	extern std::unique_ptr<memory::hook_t> client_dll;
	extern std::unique_ptr<memory::hook_t> client_mode;
	extern std::unique_ptr<memory::hook_t> panel;
	extern std::unique_ptr<memory::hook_t> c_cs_player_;
	extern std::unique_ptr<memory::hook_t> i_client_renderable_;

	namespace reset {
		static const auto index = 16;
		typedef long(__stdcall* t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall fn(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params);
	}

	namespace present {
		static const auto index = 17;
		typedef long(__stdcall* t)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
		long __stdcall fn(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region);
	}

	namespace frame_stage_notify {
		static const auto index = 37;
		typedef void(__stdcall* t)(client_frame_stage_t);
		void __stdcall fn(client_frame_stage_t stage);
	}

	namespace create_move {
		static const auto index = 24;
		typedef bool(__stdcall* t)(float, c_user_cmd*);
		bool __stdcall fn(float input_sample_time, c_user_cmd* cmd);
	}

	namespace override_view {
		static const auto index = 18;
		typedef void(__stdcall* t) (c_view_setup*);
		void __stdcall fn(c_view_setup* setup);
	}

	namespace paint_traverse {
		static const auto index = 41;
		typedef void(__thiscall* t)(void*, uint32_t, bool, bool);
		void __fastcall fn(void* ecx, void* edx, uint32_t id, bool force_repaint, bool allow_force);
	}
}