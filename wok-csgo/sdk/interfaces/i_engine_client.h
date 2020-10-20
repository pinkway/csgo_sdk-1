#pragma once

class i_engine_client {
public:
	VFUNC(get_player_info(int index, player_info_t* info), 8, void(__thiscall*)(void*, int, player_info_t*), index, info)
	VFUNC(get_player_for_user_id(int id), 9, int(__thiscall*)(void*, int), id)
	VFUNC(get_local_player(), 12, int(__thiscall*)(void*))
	VFUNC(get_last_time_stamp(), 14, float(__thiscall*)(void*))
	VFUNC(get_view_angles(qangle_t& view_angles), 18, void(__thiscall*)(void*, qangle_t&), view_angles)
	VFUNC(set_view_angles(qangle_t& view_angles), 19, void(__thiscall*)(void*, qangle_t&), view_angles)
	VFUNC(is_in_game(), 26, bool(__thiscall*)(void*))
	VFUNC(is_connected(), 27, bool(__thiscall*)(void*))
	VFUNC(get_bsp_tree_query(), 43, uintptr_t*(__thiscall*)(void*))
	VFUNC(fire_events(), 59, void(__thiscall*)(void*))
	VFUNC(get_net_channel_info(), 78, i_net_channel_info*(__thiscall*)(void*))
	VFUNC(is_paused(), 90, bool(__thiscall*)(void*))
	VFUNC(client_cmd(const char* cmd), 108, void(__thiscall*)(void*, const char*), cmd)
	VFUNC(client_cmd_unrestricted(const char* cmd, bool unk = false), 114, void(__thiscall*)(void*, const char*, bool), cmd, unk)
};