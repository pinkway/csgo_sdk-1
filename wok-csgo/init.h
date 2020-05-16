#pragma once
#include "hooks/hooks.h"
#include "events/events.h"

namespace wok {
	void init() {
		while (!memory::get_module_handle(fnv1a("serverbrowser.dll")))
			std::this_thread::sleep_for(std::chrono::milliseconds(200));

		g::local = *reinterpret_cast<c_local_player*>(SIG("client_panorama.dll", "8B 0D ? ? ? ? 83 FF FF 74 07") + 0x2);

		input::init();

		interfaces::init();

		render::init();

		netvars::init();

		hooks::init();

		events::init();
	}

	void unload() {
		hooks::undo();

		input::undo();

		events::undo();
	}
}