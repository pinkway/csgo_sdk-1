#include "../../csgo.hpp"

inline auto g_menu_opened = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

namespace csgo::hooks {
	LRESULT __stdcall wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
		if ( msg == WM_KEYUP
			&& wparam == VK_INSERT )
			g_menu_opened ^= 1;

		if ( g_menu_opened ) {
			ImGui_ImplWin32_WndProcHandler( wnd, msg, wparam, lparam );

			return 1;
		}

		return o_wnd_proc( wnd, msg, wparam, lparam );
	}

	long D3DAPI	dx9_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params ) {
		ImGui_ImplDX9_InvalidateDeviceObjects( );

		const auto ret = o_dx9_reset( device, params );

		ImGui_ImplDX9_CreateDeviceObjects( );

		return ret;
	}

	long D3DAPI dx9_present( IDirect3DDevice9* device,
		RECT* src_rect, RECT* dst_rect, HWND dst_wnd_override, RGNDATA* dirty_region
	) {
		ImGui_ImplDX9_NewFrame( );
		ImGui_ImplWin32_NewFrame( );

		ImGui::NewFrame( ); 
		
		if ( g_menu_opened ) {
			ImGui::Begin( "wok sdk", &g_menu_opened );

			ImGui::End( );
		}

		ImGui::EndFrame( );

		ImGui::Render( );

		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

		return o_dx9_present( device, src_rect, dst_rect, dst_wnd_override, dirty_region );
	}

	void __fastcall lock_cursor( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		using unlock_cursor_t = void( __thiscall* )( const std::uintptr_t );
		if ( g_menu_opened )
			return ( *sdk::address_t{ ecx }.as< unlock_cursor_t** >( ) )[ 66u ]( ecx );

		o_lock_cursor( ecx, edx );
	}

	NAKED void __stdcall create_move_proxy( int seq_number, float input_sample_frame_time, bool active ) {
		__asm {
			push ebp
			mov ebp, esp

			push ebx
			push esp
			push dword ptr[ ebp + 16 ]
			push dword ptr[ ebp + 12 ]
			push dword ptr[ ebp + 8 ]

			call create_move

			pop ebx

			pop ebp
			retn 12
		}
	}

	void __stdcall create_move( int seq_number, float input_sample_frame_time, bool active, bool& send_packet ) {
		o_create_move( valve::g_client, seq_number, input_sample_frame_time, active );

		if ( !valve::g_engine->in_game( )
			|| !g_local_player->self( )
			|| !g_local_player->self( )->alive( ) )
			return;

		const auto slot = seq_number % valve::k_mp_backup;

		g_local_player->create_move( send_packet,
			valve::g_input->m_cmds[ slot ], valve::g_input->m_vfyd_cmds[ slot ]
		);
	}
}