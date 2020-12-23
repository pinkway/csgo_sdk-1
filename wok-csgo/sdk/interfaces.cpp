#include "interfaces.h"

namespace interfaces {
	void init() {
		INTERFACE_EXPORT(m_client_dll, i_base_client_dll, "client.dll", "VClient018")
		INTERFACE_EXPORT(m_engine, i_engine_client, "engine.dll", "VEngineClient014")
		INTERFACE_EXPORT(m_entity_list, i_client_entity_list, "client.dll", "VClientEntityList003")

		INTERFACE_OFFSET(m_client_mode, i_client_mode, m_client_dll, 10, 0x5)
		INTERFACE_OFFSET(m_global_vars, i_global_vars, m_client_dll, 11, 0xA)
		INTERFACE_OFFSET(m_client_state, i_client_state, m_engine, 12, 0x10)

		m_mem_alloc = *EXPORT("tier0.dll", "g_pMemAlloc").cast<i_mem_alloc**>();

		INTERFACE_EXPORT(m_model_info, i_model_info, "engine.dll", "VModelInfoClient004")
		INTERFACE_EXPORT(m_surface, i_surface, "vguimatsurface.dll", "VGUI_Surface031")

		INTERFACE_SIG(m_input, i_input, "client.dll", "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85", 0x1)

		PINTERFACE_SIG(m_move_helper, i_move_helper, "client.dll", "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01", 0x2)

		INTERFACE_EXPORT(m_prediction, i_prediction, "client.dll", "VClientPrediction001")
		INTERFACE_EXPORT(m_game_movement, i_game_movement, "client.dll", "GameMovement001")
		INTERFACE_EXPORT(m_debug_overlay, i_debug_overlay, "engine.dll", "VDebugOverlay004")
		INTERFACE_EXPORT(m_cvar_system, i_cvar_system, "vstdlib.dll", "VEngineCvar007")
		INTERFACE_EXPORT(m_event_manager, i_game_event_manager, "engine.dll", "GAMEEVENTSMANAGER002")
		INTERFACE_EXPORT(m_trace_system, i_engine_trace, "engine.dll", "EngineTraceClient004")
		INTERFACE_EXPORT(m_surface_data, i_surface_data, "vphysics.dll", "VPhysicsSurfaceProps001")
		INTERFACE_EXPORT(m_input_system, i_input_system, "inputsystem.dll", "InputSystemVersion001")

		INTERFACE_SIG(m_game_rules, i_game_rules*, "client.dll", "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A", 0x1)

		INTERFACE_EXPORT(m_game_types, i_game_types, "matchmaking.dll", "VENGINE_GAMETYPES_VERSION002")
		INTERFACE_EXPORT(m_model_cache, i_mdl_cache, "datacache.dll", "MDLCache004")
		INTERFACE_EXPORT(m_model_render, i_model_render, "engine.dll", "VEngineModel016")
		INTERFACE_EXPORT(m_material_system, i_material_system, "materialsystem.dll", "VMaterialSystem080")

		INTERFACE_SIG(m_glow_manager, i_glow_object_manager, "client.dll", "0F 11 05 ? ? ? ? 83 C8 01", 0x3)
		INTERFACE_SIG(m_beams, i_view_render_beams, "client.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9", 0x1)
		INTERFACE_SIG(m_weapon_system, i_weapon_system, "client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0", 0x2)
		INTERFACE_SIG(m_player_resource, i_cs_player_resource*, "client.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7", 0x2)

		INTERFACE_EXPORT(m_localize, i_localize, "localize.dll", "Localize_001")
		INTERFACE_EXPORT(m_panel, i_panel, "vgui2.dll", "VGUI_Panel009")
		INTERFACE_EXPORT(m_render_view, i_render_view, "engine.dll", "VEngineRenderView014")

		uintptr_t** shader_device = nullptr;
		INTERFACE_EXPORT(shader_device, uintptr_t*, "shaderapidx9.dll", "ShaderDevice001")
		if (const auto device_table = shader_device[0]) {
			if (const auto shutdown_device = device_table[37]) {
				m_d3d_device = **reinterpret_cast<IDirect3DDevice9***>(shutdown_device + 0x2);
			}
		}
	}

	i_base_client_dll*		m_client_dll = nullptr;
	i_engine_client*		m_engine = nullptr;
	i_client_entity_list*	m_entity_list = nullptr;
	i_client_mode*			m_client_mode = nullptr;
	i_global_vars*			m_global_vars = nullptr;
	i_client_state*			m_client_state = nullptr;
	i_mem_alloc*			m_mem_alloc = nullptr;
	i_model_info*			m_model_info = nullptr;
	i_surface*				m_surface = nullptr;
	i_input*				m_input = nullptr;
	i_move_helper*			m_move_helper = nullptr;
	i_prediction*			m_prediction = nullptr;
	i_game_movement*		m_game_movement = nullptr;
	i_debug_overlay*		m_debug_overlay = nullptr;
	i_cvar_system*			m_cvar_system = nullptr;
	i_game_event_manager*	m_event_manager = nullptr;
	i_engine_trace*			m_trace_system = nullptr;
	i_surface_data*			m_surface_data = nullptr;
	i_input_system*			m_input_system = nullptr;
	i_game_rules**			m_game_rules = nullptr;
	i_game_types*			m_game_types = nullptr;
	i_mdl_cache*			m_model_cache = nullptr;
	i_model_render*			m_model_render = nullptr;
	i_material_system*		m_material_system = nullptr;
	i_glow_object_manager*	m_glow_manager = nullptr;
	i_view_render_beams*	m_beams = nullptr;
	i_weapon_system*		m_weapon_system = nullptr;
	i_cs_player_resource**	m_player_resource = nullptr;
	i_localize*				m_localize = nullptr;
	i_panel*				m_panel = nullptr;
	i_render_view*			m_render_view = nullptr;

	IDirect3DDevice9*		m_d3d_device = nullptr;
}
