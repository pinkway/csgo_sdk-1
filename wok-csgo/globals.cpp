#include "globals.h"

namespace g {
	HMODULE			module = nullptr;
	c_local_player	local;
	c_user_cmd*		cmd = nullptr;
	bool			send_packet = true;
}