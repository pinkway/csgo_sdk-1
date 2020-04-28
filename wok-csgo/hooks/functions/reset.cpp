#include "../hooks.h"

long __stdcall hooks::reset::fn(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params) {
	static const auto original = d3d_device->get_original<t>(index);

	render::invalidate();
	auto ret = original(device, present_params);
	render::restore(device);

	return ret;
}