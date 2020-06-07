#include "../hooks.h"

long __stdcall hooks::reset::fn(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params) {
	static const auto original = m_d3d_device->get_original<T>(index);

	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto ret = original(device, present_params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}