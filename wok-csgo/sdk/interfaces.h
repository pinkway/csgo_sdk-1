#pragma once
#include "sdk.h"

namespace interfaces {
	void init();

	extern i_base_client_dll*		m_client_dll;
	extern i_engine_client*			m_engine;
	extern i_client_entity_list*	m_entity_list;
	extern i_client_mode*			m_client_mode;
	extern i_global_vars*			m_global_vars;
	extern i_client_state*			m_client_state;
	extern i_mem_alloc*				m_mem_alloc;
	extern i_model_info*			m_model_info;
	extern i_surface*				m_surface;
	extern i_input*					m_input;
	extern i_move_helper*			m_move_helper;
	extern i_prediction*			m_prediction;
	extern i_game_movement*			m_game_movement;
	extern i_debug_overlay*			m_debug_overlay;
	extern i_cvar_system*			m_cvar_system;
	extern i_game_event_manager*	m_event_manager;
	extern i_engine_trace*			m_trace_system;
	extern i_surface_data*			m_surface_data;
	extern i_input_system*			m_input_system;
	extern i_game_rules**			m_game_rules;
	extern i_game_types*			m_game_types;
	extern i_mdl_cache*				m_model_cache;
	extern i_model_render*			m_model_render;
	extern i_material_system*		m_material_system;
	extern i_glow_object_manager*	m_glow_manager;
	extern i_view_render_beams*		m_beams;
	extern i_weapon_system*			m_weapon_system;
	extern i_cs_player_resource**	m_player_resource;
	extern i_localize*				m_localize;
	extern i_panel*					m_panel;
	extern i_render_view*			m_render_view;

	extern IDirect3DDevice9*		m_d3d_device;
}