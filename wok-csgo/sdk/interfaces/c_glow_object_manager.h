#pragma once

class c_glow_object_manager {
public:
	class glow_object_t {
	public:
		void set(col_t clr, int style) {
			m_glow_color = vec3_t(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f);
			m_alpha = clr.a() / 255.f;
			m_render_when_occluded = true;
			m_render_when_unoccluded = false;
			m_glow_style = style;
			m_bloom_amount = 0.8f;
		}
	public:
		c_base_entity*		m_entity;
		vec3_t				m_glow_color;
		float				m_alpha;

		char				pad0[8];
		float				m_bloom_amount;
		char				pad1[4];
		bool				m_render_when_occluded;
		bool				m_render_when_unoccluded;
		bool				m_full_bloom_render;
		char				pad2[1];
		int					m_full_bloom_stencil_test_value;
		int					m_glow_style;
		int					m_split_screen_slot;
		int					m_next_free_slot;

		bool				IsUnused() const { return m_next_free_slot != glow_object_t::ENTRY_IN_USE; }

		static const int	END_OF_FREE_LIST = -1;
		static const int	ENTRY_IN_USE = -2;
	};

	c_utl_vector<glow_object_t> m_GlowObjectDefinitions;
	int							m_first_free_slot;
};