#pragma once

class i_engine_client {
public:
	VFUNC(get_player_info(int index, player_info_t* info), 8, void(__thiscall*)(void*, int, player_info_t*), index, info)
	VFUNC(get_view_angles(qangle_t& viewangles), 18, void(__thiscall*)(void*, qangle_t&), viewangles)
	VFUNC(is_in_game(), 26, bool(__thiscall*)(void*))
	VFUNC(is_connected(), 27, bool(__thiscall*)(void*))
	VFUNC(client_cmd(const char* cmd), 108, void(__thiscall*)(void*, const char*), cmd)
	VFUNC(client_cmd_unrestricted(const char* cmd, const char* flag = 0), 114, void(__thiscall*)(void*, const char*, bool), cmd, flag)
};