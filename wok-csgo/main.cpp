#include "common_includes.h"
#include "globals.h"
#include "init.h"

void __stdcall init_wok(const HMODULE module) {
	g::module = module;

	wok::init();

	while (!input::get_key(VK_DELETE, ON_KEY))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	wok::unload();

	FreeLibraryAndExitThread(module, 0);
}

int __stdcall DllMain(const HMODULE module, const unsigned long reason_for_call, const void* reserved) {
	if (reason_for_call != DLL_PROCESS_ATTACH)
		return 0;

	module ? DisableThreadLibraryCalls(module) : 0;

	if (auto thread = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(init_wok), module, 0, 0))
		CloseHandle(thread);

	return 1;
}