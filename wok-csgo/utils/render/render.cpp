#include "../utils.h"

namespace render {
	void init(IDirect3DDevice9* device) {
		if (m_init)
			return;

		restore(device);

		m_fonts.emplace_back(fonts::m_tahoma12 = std::make_shared<c_font>(_("Tahoma"), 12, FW_REGULAR));

		m_init = true;
	}

	void restore(IDirect3DDevice9* device) {
		m_device = device;

		for (auto& font : m_fonts)
			font->restore(m_device);
	}

	void invalidate() {
		m_device = nullptr;

		for (auto& font : m_fonts)
			font->invalidate();
	}

	void line(const vec2_t& from, const vec2_t& to, const col_t& clr) {
		auto new_clr = clr.direct();

		vertex_t vert[2] = {
			{ from.x, from.y, 0.f, 1.f, new_clr },
			{ to.x, to.y, 0.f, 1.f, new_clr }
		};

		m_device->SetTexture(0, nullptr);
		m_device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &vert, sizeof(vertex_t));
	}

	void rect(const vec2_t& pos, const vec2_t& size, const col_t& clr) {
		auto new_clr = clr.direct();

		vertex_t vert[5] = {
			{ pos.x, pos.y, 1.f, 1.f, new_clr },
			{ pos.x + size.x, pos.y, 1.f, 1.f, new_clr },
			{ pos.x + size.x, pos.y + size.y, 1.f, 1.f, new_clr },
			{ pos.x, pos.y + size.y, 1.f, 1.f, new_clr },
			{ pos.x, pos.y, 1.f, 1.f, new_clr }
		};

		m_device->SetTexture(0, nullptr);
		m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &vert, sizeof(vertex_t));
	}

	void filled_rect(const vec2_t& pos, const vec2_t& size, const col_t& clr) {
		auto new_clr = clr.direct();

		vertex_t vert[4] = {
			{ pos.x, pos.y, 1.f, 1.f, new_clr },
			{ pos.x + size.x, pos.y, 1.f, 1.f, new_clr },
			{ pos.x, pos.y + size.y, 1.f, 1.f, new_clr },
			{ pos.x + size.x, pos.y + size.y, 1.f, 1.f, new_clr },
		};

		m_device->SetTexture(0, nullptr);
		m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vert, sizeof(vertex_t));
	}

	void gradient_rect(const vec2_t& pos, const vec2_t& size, const col_t& clr, const col_t& clr2, bool horizontal) {
		auto new_clr = clr.direct();
		auto new_clr2 = clr2.direct();

		if (horizontal) {
			auto top_left = new_clr, top_right = new_clr2, bottom_left = new_clr, bottom_right = new_clr2;

			vertex_t vert[5] = {
				{ pos.x, pos.y, 1.f, 1.f, top_left },
				{ pos.x + size.x, pos.y, 1.f, 1.f, top_right },
				{ pos.x + size.x, pos.y + size.y, 1.f, 1.f, bottom_right },
				{ pos.x, pos.y + size.y, 1.f, 1.f, bottom_left },
				{ pos.x, pos.y, 1.f, 1.f, top_left }
			};

			m_device->SetTexture(0, nullptr);
			m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &vert, sizeof(vertex_t));
		}
		else {
			auto top_left = new_clr, top_right = new_clr, bottom_left = new_clr2, bottom_right = new_clr2;

			vertex_t vert[5] = {
				{ pos.x, pos.y, 1.f, 1.f, top_left },
				{ pos.x + size.x, pos.y, 1.f, 1.f, top_right },
				{ pos.x + size.x, pos.y + size.y, 1.f, 1.f, bottom_right },
				{ pos.x, pos.y + size.y, 1.f, 1.f, bottom_left },
				{ pos.x, pos.y, 1.f, 1.f, top_left }
			};

			m_device->SetTexture(0, nullptr);
			m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &vert, sizeof(vertex_t));
		}
	}

	void gradient_filled_rect(const vec2_t& pos, const vec2_t& size, const col_t& clr, const col_t& clr2, bool horizontal) {
		auto new_clr = clr.direct();
		auto new_clr2 = clr2.direct();

		if (horizontal) {
			auto top_left = new_clr, top_right = new_clr2, bottom_left = new_clr, bottom_right = new_clr2;

			vertex_t vert[4] = {
				 { pos.x, pos.y, 0.f, 1.f, top_left },
				 { pos.x + size.x, pos.y, 0.f, 1.f, top_right },
				 { pos.x, pos.y + size.y, 0.f, 1.f, bottom_left },
				 { pos.x + size.x, pos.y + size.y, 0.f, 1.f, bottom_right }
			};

			m_device->SetTexture(0, nullptr);
			m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vert, sizeof(vertex_t));
		}
		else {
			auto top_left = new_clr, top_right = new_clr, bottom_left = new_clr2, bottom_right = new_clr2;

			vertex_t vert[4] = {
				 { pos.x, pos.y, 0.f, 1.f, top_left },
				 { pos.x + size.x, pos.y, 0.f, 1.f, top_right },
				 { pos.x, pos.y + size.y, 0.f, 1.f, bottom_left },
				 { pos.x + size.x, pos.y + size.y, 0.f, 1.f, bottom_right }
			};

			m_device->SetTexture(0, nullptr);
			m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vert, sizeof(vertex_t));
		}
	}
	void text(std::shared_ptr<c_font>& font, const vec2_t& pos, std::wstring txt, int flags, const col_t& clr) {
		if (txt.empty())
			return;

		auto new_pos = pos;

		if (flags) {
			if (flags & FONT_CENTERED_X 
				|| flags & FONT_CENTERED_Y) {
				auto size = font->get_size(txt);

				flags & FONT_CENTERED_X ? new_pos.x -= size.x * 0.5f : 0;

				flags & FONT_CENTERED_Y ? new_pos.y -= size.y * 0.5f : 0;
			}

			if (flags & FONT_OUTLINE
				|| flags & FONT_DROPSHADOW) {
				auto new_clr = col_t(max(0, clr.a() - 100));

				if (flags & FONT_OUTLINE) {
					font->paint(new_pos - vec2_t(0, 1), txt, new_clr);
					font->paint(new_pos + vec2_t(0, 1), txt, new_clr);
					font->paint(new_pos - vec2_t(1, 0), txt, new_clr);
					font->paint(new_pos + vec2_t(1, 0), txt, new_clr);
				}

				flags & FONT_DROPSHADOW ? font->paint(new_pos + 1, txt, new_clr) : 0;
			}
		}

		font->paint(new_pos, txt, clr);
	}

	void text(std::shared_ptr<c_font>& font, const vec2_t& pos, std::string txt, int flags, const col_t& clr) {
		if (txt.empty())
			return;

		wchar_t buf[1024];
		if (!MultiByteToWideChar(CP_UTF8, 0, txt.data(), -1, buf, 1024))
			return;

		text(font, pos, buf, flags, clr);
	}

	void filled_triangle(vec2_t pos, vec2_t pos2, vec2_t pos3, const col_t& clr) {
		auto new_clr = clr.direct();

		vertex_t vert[3] = {
			{ pos.x, pos.y, 0.f, 1.f, new_clr },
			{ pos2.x, pos2.y, 0.f, 1.f, new_clr },
			{ pos3.x, pos3.y, 0.f, 1.f, new_clr }
		};

		m_device->SetTexture(0, nullptr);
		m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, &vert, sizeof(vertex_t));
	}

	bool world_to_screen(const vec3_t& in, vec2_t& out) {
		auto screen_transform = [](const vec3_t& in, vec2_t& out) -> bool {
			static const auto& matrix = *reinterpret_cast<v_matrix*>(*reinterpret_cast<uintptr_t*>(SIG("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9") + 0x3) + 0xB0);

			out.x = matrix[0][0] * in.x + matrix[0][1] * in.y + matrix[0][2] * in.z + matrix[0][3];
			out.y = matrix[1][0] * in.x + matrix[1][1] * in.y + matrix[1][2] * in.z + matrix[1][3];

			auto w = matrix[3][0] * in.x + matrix[3][1] * in.y + matrix[3][2] * in.z + matrix[3][3];

			if (w < 0.001f) {
				out.x *= 100000;
				out.y *= 100000;
				return true;
			}

			auto inv_w = 1.f / w;
			out.x *= inv_w;
			out.y *= inv_w;

			return false;
		};

		if (!screen_transform(in, out)) {
			out.x = (m_screen_size.x * 0.5f) + (out.x * m_screen_size.x) * 0.5f;
			out.y = (m_screen_size.y * 0.5f) - (out.y * m_screen_size.y) * 0.5f;
			return true;
		}

		return false;
	}

	void set_render_states() {
		D3DVIEWPORT9 viewport;
		m_device->GetViewport(&viewport);
		m_screen_size = vec2_t(viewport.Width, viewport.Height);

		m_device->SetVertexShader(nullptr);
		m_device->SetPixelShader(nullptr);
		m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		m_device->SetRenderState(D3DRS_LIGHTING, false);
		m_device->SetRenderState(D3DRS_FOGENABLE, false);
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
		m_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
		m_device->SetRenderState(D3DRS_STENCILENABLE, false);

		m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		m_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
		m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

		m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
		m_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

		m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	bool m_init = false;
	IDirect3DDevice9* m_device = nullptr;

	vec2_t m_screen_size;
	std::vector<std::shared_ptr<c_font>> m_fonts;
}

namespace fonts {
	std::shared_ptr<c_font> m_tahoma12;
}