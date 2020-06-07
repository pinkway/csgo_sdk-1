#pragma once
#include "hooks/hooks.h"
#include "events/events.h"

namespace wok {
	void init() {
		while (!memory::get_module_handle(fnv1a("serverbrowser.dll")))
			std::this_thread::sleep_for(std::chrono::milliseconds(200));

		g::local = *SIG("client.dll", "8B 0D ? ? ? ? 83 FF FF 74 07").self_offset(0x2).cast<c_local_player*>();

		input::init();

		interfaces::init();

		render::init();

		netvars::init();

		cfg::init();

		hooks::init();

		events::init();
	}

	void unload() {
		hooks::undo();

		input::undo();

		events::undo();
	}
}