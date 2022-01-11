#pragma once

namespace csgo::hooks {
    LRESULT __stdcall wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
    inline decltype( &wnd_proc ) o_wnd_proc{};

    long D3DAPI dx9_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params );
    inline decltype( &dx9_reset ) o_dx9_reset{};

    long D3DAPI dx9_present( IDirect3DDevice9* device,
        RECT* src_rect, RECT* dst_rect, HWND dst_wnd_override, RGNDATA* dirty_region
    );
    inline decltype( &dx9_present ) o_dx9_present{};

    void __fastcall lock_cursor( const std::uintptr_t ecx, const std::uintptr_t edx );
    inline decltype( &lock_cursor ) o_lock_cursor{};

    void __stdcall create_move_proxy( int seq_number, float input_sample_frame_time, bool active );
    void __stdcall create_move( int seq_number, float input_sample_frame_time, bool active, bool& send_packet );

    using o_create_move_t = void( __thiscall* )( valve::c_client* const, int, float, bool );
    inline o_create_move_t o_create_move{};
}