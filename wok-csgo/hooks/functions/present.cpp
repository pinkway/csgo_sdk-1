#include "../hooks.h"
#include "../../menu/menu.h"

long __stdcall hooks::present::fn(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region) {
	static const auto original = d3d_device->get_original<t>(index);

	IDirect3DStateBlock9* block;
	if (device->CreateStateBlock(D3DSBT_PIXELSTATE, &block))
		return original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);

	IDirect3DVertexDeclaration9* vert_dec;
	if (device->GetVertexDeclaration(&vert_dec))
		return original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);

	IDirect3DVertexShader9* vert_shader;
	if (device->GetVertexShader(&vert_shader))
		return original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);

	render::init(device);
	render::set_render_states(); {
		c_menu::get().on_paint();
	}
	block->Apply();
	block->Release();

	device->SetVertexShader(vert_shader);
	device->SetVertexDeclaration(vert_dec);

	return original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);
}