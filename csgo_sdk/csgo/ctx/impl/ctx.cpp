#include "../../csgo.hpp"

int __stdcall DllMain( _In_ HINSTANCE instance, _In_ DWORD reason, _In_ LPVOID reserved ) {
    if ( reason != DLL_PROCESS_ATTACH )
        return 0;

    DisableThreadLibraryCalls( instance );

    std::jthread{ [ ]( ) {
        csgo::g_ctx->init( );
    } }.detach( );

    return 1;
}

#ifdef _DEBUG
#define THROW_IF_DBG( exception ) throw std::runtime_error{ exception }
#else
#define THROW_IF_DBG( exception ) return
#endif

#define HOOK( target, hook, original ) \
    if ( MH_CreateHook( sdk::address_t{ target }.as< LPVOID >( ), \
        reinterpret_cast< LPVOID >( &hook ), reinterpret_cast< LPVOID* >( &original ) ) != MH_OK ) \
        THROW_IF_DBG( "can't hook " #hook "." ) \

#define HOOK_VFUNC( vft, index, hook, original ) \
    if ( MH_CreateHook( ( *sdk::address_t{ vft }.as< LPVOID** >( ) )[ index ], \
        reinterpret_cast< LPVOID >( &hook ), reinterpret_cast< LPVOID* >( &original ) ) != MH_OK ) \
        THROW_IF_DBG( "can't hook " #hook "." ) \

namespace csgo {
    void c_ctx::init( ) {
        std::unordered_map< sdk::hash_t, sdk::x86_pe_image_t* > modules{};

        do {
            sdk::peb( )->for_each_ldr_data_table_entry( [ & ] ( sdk::ldr_data_table_entry_t* const entry ) {
                modules.insert_or_assign(
                    sdk::hash( entry->m_base_dll_name.m_buffer, entry->m_base_dll_name.m_len / sizeof( wchar_t ) ),
                    entry->m_dll_base.as< sdk::x86_pe_image_t* >( )
                );

                return false;
            }, sdk::e_ldr_data_table::in_load_order );

            std::this_thread::sleep_for( std::chrono::milliseconds{ 200u } );
        } while ( modules.find( HASH( "serverbrowser.dll" ) ) == modules.end( ) );

        if ( MH_Initialize( ) != MH_OK )
            THROW_IF_DBG( "can't initialize minhook." );

        struct code_section_t {
            ALWAYS_INLINE constexpr code_section_t( ) = default;

            /*ALWAYS_INLINE*/ code_section_t( const sdk::x86_pe_image_t* const image ) {
                if ( image->m_dos_hdr.e_magic != sdk::k_dos_hdr_magic )
                    THROW_IF_DBG( "invalid dos hdr." );

                const auto nt_hdrs = image->nt_hdrs( );
                if ( nt_hdrs->m_sig != sdk::k_nt_hdrs_magic )
                    THROW_IF_DBG( "invalid nt hdrs." );

                m_start = image;
                m_start.self_offset( nt_hdrs->m_opt_hdr.m_code_base );

                m_end = m_start.offset( nt_hdrs->m_opt_hdr.m_code_size );
            }

            sdk::address_t m_start{}, m_end{};
        };

        /* initialize imgui */
        {
            const code_section_t shaderapidx9{ modules.at( HASH( "shaderapidx9.dll" ) ) };

            const auto device = **BYTESEQ( "A1 ? ? ? ? 50 8B 08 FF 51 0C" ).search(
                shaderapidx9.m_start, shaderapidx9.m_end, false
            ).self_offset( 0x1 ).as< IDirect3DDevice9*** >( );

            D3DDEVICE_CREATION_PARAMETERS params{};
            if ( device->GetCreationParameters( &params ) != D3D_OK )
                THROW_IF_DBG( "can't get creation params." );

            ImGui::CreateContext( );
            ImGui::StyleColorsClassic( );

            ImGui::GetStyle( ).WindowMinSize = { 450, 350 };

            auto& io = ImGui::GetIO( );

            io.IniFilename = io.LogFilename = nullptr;

            ImGui_ImplWin32_Init( params.hFocusWindow );
            ImGui_ImplDX9_Init( device );

            const code_section_t inputsystem{ modules.at( HASH( "inputsystem.dll" ) ) };

            HOOK( BYTESEQ( "55 8B EC 83 EC 0C 80 3D" ).search(
                inputsystem.m_start, inputsystem.m_end, false
            ), hooks::wnd_proc, hooks::o_wnd_proc );

            HOOK_VFUNC( device, 16u, hooks::dx9_reset, hooks::o_dx9_reset );
            HOOK_VFUNC( device, 17u, hooks::dx9_present, hooks::o_dx9_present );
        }

        const code_section_t client{ modules.at( HASH( "client.dll" ) ) };

        /* initialize valve interfaces */
        {
            std::unordered_map< sdk::hash_t, sdk::address_t > interfaces{};

            const auto parse_module = [ & ] ( const sdk::hash_t hash ) {
                const auto module = modules.at( hash );

                sdk::address_t list{};

                module->for_each_export( module, [ & ] ( const char* name, const sdk::address_t addr ) {
                    if ( sdk::hash( name ) != HASH( "CreateInterface" ) )
                        return false;

                    list = addr;

                    return true;
                } );

                if ( !list )
                    return;

                if ( *list.offset( 0x4 ).as< std::uint8_t* >( ) == 0xe9u
                    && *list.self_rel( 0x5, true ).offset( 0x5 ).as< std::uint8_t* >( ) == 0x35u )
                    list.self_offset( 0x6 ).self_deref( 2u );
                else if ( *list.offset( 0x2 ).as< std::uint8_t* >( ) == 0x35 )
                    list.self_offset( 0x3 ).self_deref( 2u );
                else
                    return;

                struct interface_entry_t {
                    using create_t = std::uintptr_t*( __cdecl* )( );

                    create_t            m_create_fn{};
                    const char*         m_name{};
                    interface_entry_t*  m_next{};
                };

                for ( auto entry = list.as< interface_entry_t* >( ); entry; entry = entry->m_next )
                    if ( entry->m_name )
                        interfaces.insert_or_assign( sdk::hash( entry->m_name ), entry->m_create_fn( ) );             
            };

            parse_module( HASH( "client.dll" ) );
            parse_module( HASH( "engine.dll" ) );
            parse_module( HASH( "vstdlib.dll" ) );
            parse_module( HASH( "vphysics.dll" ) );
            parse_module( HASH( "matchmaking.dll" ) );

            valve::g_client = interfaces.at( HASH( "VClient018" ) ).as< valve::c_client* >( );
            valve::g_engine = interfaces.at( HASH( "VEngineClient014" ) ).as< valve::c_engine* >( );
            valve::g_entity_list = interfaces.at( HASH( "VClientEntityList003" ) ).as< valve::c_entity_list* >( );

            valve::g_global_vars = **reinterpret_cast< valve::global_vars_base_t*** >(
                ( *reinterpret_cast< std::uintptr_t** >( valve::g_client ) )[ 11u ] + 0xau
            );
            valve::g_client_state = **reinterpret_cast< valve::client_state_t*** >(
                ( *reinterpret_cast< std::uintptr_t** >( valve::g_engine ) )[ 12u ] + 0x10u
            );

            valve::g_input = *BYTESEQ( "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85" ).search(
                client.m_start, client.m_end, false
            ).self_offset( 0x1 ).as< valve::input_t** >( );

            valve::g_cvar = interfaces.at( HASH( "VEngineCvar007" ) ).as< valve::c_cvar* >( );

            valve::g_move_helper = **BYTESEQ( "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01" ).search(
                client.m_start, client.m_end, false
            ).self_offset( 0x2 ).as< valve::c_move_helper*** >( );

            valve::g_prediction = interfaces.at( HASH( "VClientPrediction001" ) ).as< valve::prediction_t* >( );
            valve::g_movement = interfaces.at( HASH( "GameMovement001" ) ).as< valve::c_movement* >( );

            valve::g_engine_trace = interfaces.at( HASH( "EngineTraceClient004" ) ).as< valve::c_engine_trace* >( );
            valve::g_surface_data = interfaces.at( HASH( "VPhysicsSurfaceProps001" ) ).as< valve::c_surface_data* >( );

            valve::g_game_rules = *BYTESEQ( "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 74 7A" ).search(
                client.m_start, client.m_end, false
            ).self_offset( 0x1 ).as< valve::game_rules_t*** >( );
            valve::g_game_types = interfaces.at( HASH( "VENGINE_GAMETYPES_VERSION002" ) ).as< valve::c_game_types* >( );
        }

        /* initialize offsets */
        {
            m_offsets.m_local_player = BYTESEQ( "8B 0D ? ? ? ? 83 FF FF 74 07" ).search( client.m_start, client.m_end ).self_offset( 0x2 ).self_deref( 1u );

            m_offsets.m_user_cmd_checksum = BYTESEQ( "53 8B D9 83 C8" ).search( client.m_start, client.m_end );

            m_offsets.m_anim_state.m_reset = BYTESEQ( "56 6A 01 68 ? ? ? ? 8B F1" ).search( client.m_start, client.m_end );
            m_offsets.m_anim_state.m_update = BYTESEQ( "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" ).search( client.m_start, client.m_end );

            m_offsets.m_renderable.m_bone_cache = *BYTESEQ( "FF B7 ? ? ? ? 52" ).search( client.m_start, client.m_end ).self_offset( 0x2 ).as< std::uint32_t* >( );
            m_offsets.m_renderable.m_mdl_bone_cnt = *BYTESEQ( "EB 05 F3 0F 10 45 ? 8B 87 ? ? ? ?" ).search(
                client.m_start, client.m_end
            ).self_offset( 0x9 ).as< std::uint32_t* >( );

            struct ent_offset_t {
                valve::recv_prop_t* m_prop{};
                std::uint32_t       m_offset{};
            };

            std::unordered_map< sdk::hash_t, ent_offset_t > ent_offsets{};

            ent_offsets.reserve( 40000u );

            const auto parse_recv_table = [ & ]( const auto& self, const char* name, valve::recv_table_t* const table, const std::uint32_t offset = 0u ) -> void {
                std::string concated{};

                concated.reserve( 128u );

                for ( int i{}; i < table->m_props_count; ++i ) {
                    const auto prop = &table->m_props[ i ];

                    const auto child = prop->m_data_table;
                    if ( child
                        && child->m_props_count > 0 )
                        self( self, name, child, prop->m_offset + offset );

                    concated = name;
                    concated += "->";
                    concated += prop->m_var_name;

                    ent_offsets.insert_or_assign(
                        sdk::hash( concated.data( ), concated.size( ) ),
                        ent_offset_t{ prop, prop->m_offset + offset }
                    );
                }
            };

            for ( auto client_class = valve::g_client->all_classes( ); client_class; client_class = client_class->m_next )
                if ( client_class->m_recv_table )
                    parse_recv_table( parse_recv_table, client_class->m_network_name, client_class->m_recv_table );

            {
                std::string concated{};

                concated.reserve( 128u );

                const auto mov_data_map = BYTESEQ( "C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C3 CC" );

                for ( auto start = client.m_start; ; start.self_offset( 0x1 ) ) {
                    start = mov_data_map.search( start, client.m_end );
                    if ( start == client.m_end )
                        break;

                    const auto data_map = start.offset( 0x2 ).deref( 1u ).offset( -0x4 ).as< valve::data_map_t* >( );
                    if ( !data_map
                        || data_map->m_size <= 0
                        || data_map->m_size >= 200
                        || !data_map->m_name
                        || !data_map->m_description )
                        continue;

                    for ( int i{}; i < data_map->m_size; ++i ) {
                        const auto& description = data_map->m_description[ i ];
                        if ( !description.m_name )
                            continue;

                        concated = data_map->m_name;
                        concated.erase( std::remove( concated.begin( ), concated.end( ), '_' ), concated.end( ) );

                        concated += "->";
                        concated += description.m_name;

                        ent_offsets.insert_or_assign(
                            sdk::hash( concated.data( ), concated.size( ) ),
                            ent_offset_t{ nullptr, description.m_offset }
                        );
                    }
                }
            }

            m_offsets.m_base_entity.m_health = ent_offsets.at( HASH( "CBaseEntity->m_iHealth" ) ).m_offset;
            m_offsets.m_base_entity.m_team_num = ent_offsets.at( HASH( "CBaseEntity->m_iTeamNum" ) ).m_offset;
            m_offsets.m_base_entity.m_sim_time = ent_offsets.at( HASH( "CBaseEntity->m_flSimulationTime" ) ).m_offset;
            m_offsets.m_base_entity.m_flags = ent_offsets.at( HASH( "CBaseEntity->m_fFlags" ) ).m_offset;
            m_offsets.m_base_entity.m_origin = ent_offsets.at( HASH( "CBaseEntity->m_vecOrigin" ) ).m_offset;
            m_offsets.m_base_entity.m_velocity = ent_offsets.at( HASH( "CBaseEntity->m_vecVelocity" ) ).m_offset;
            m_offsets.m_base_entity.m_abs_origin = ent_offsets.at( HASH( "CBaseEntity->m_vecAbsOrigin" ) ).m_offset;
            m_offsets.m_base_entity.m_abs_velocity = ent_offsets.at( HASH( "CBaseEntity->m_vecAbsVelocity" ) ).m_offset;
            m_offsets.m_base_entity.m_abs_rotation = ent_offsets.at( HASH( "CBaseEntity->m_angAbsRotation" ) ).m_offset;
            m_offsets.m_base_entity.m_move_type = ent_offsets.at( HASH( "CBaseEntity->m_MoveType" ) ).m_offset;
            m_offsets.m_base_entity.m_mins = ent_offsets.at( HASH( "CBaseEntity->m_vecMins" ) ).m_offset;
            m_offsets.m_base_entity.m_maxs = ent_offsets.at( HASH( "CBaseEntity->m_vecMaxs" ) ).m_offset;

            m_offsets.m_base_animating.m_sequence = ent_offsets.at( HASH( "CBaseAnimating->m_nSequence" ) ).m_offset;
            m_offsets.m_base_animating.m_hitbox_set_index = ent_offsets.at( HASH( "CBaseAnimating->m_nHitboxSet" ) ).m_offset;
            m_offsets.m_base_animating.m_studio_hdr = *BYTESEQ( "75 19 8B 40 04" ).search(
                client.m_start, client.m_end
            ).self_rel( 0x1, false ).self_offset( 0x2 ).as< std::uint32_t* >( );
            m_offsets.m_base_animating.m_pose_params = ent_offsets.at( HASH( "CBaseAnimating->m_flPoseParameter" ) ).m_offset;
            m_offsets.m_base_animating.m_anim_layers = *BYTESEQ( "8B 80 ? ? ? ? 8D 34 C8" ).search(
                client.m_start, client.m_end
            ).self_offset( 0x2 ).as< std::uint32_t* >( );

            m_offsets.m_base_grenade.m_pin_pulled = ent_offsets.at( HASH( "CBaseCSGrenade->m_bPinPulled" ) ).m_offset;
            m_offsets.m_base_grenade.m_throw_time = ent_offsets.at( HASH( "CBaseCSGrenade->m_fThrowTime" ) ).m_offset;
            m_offsets.m_base_grenade.m_throw_strength = ent_offsets.at( HASH( "CBaseCSGrenade->m_flThrowStrength" ) ).m_offset;

            m_offsets.m_base_attributable_item.m_item_index = ent_offsets.at( HASH( "CBaseAttributableItem->m_iItemDefinitionIndex" ) ).m_offset;

            m_offsets.m_base_weapon.m_get_info = BYTESEQ( "55 8B EC 81 EC ?? ?? ?? ?? 53 8B D9 56 57 8D 8B" ).search( client.m_start, client.m_end );
            m_offsets.m_base_weapon.m_clip1 = ent_offsets.at( HASH( "CBaseCombatWeapon->m_iClip1" ) ).m_offset;
            m_offsets.m_base_weapon.m_primary_reserve_ammo_count = ent_offsets.at( HASH( "CBaseCombatWeapon->m_iPrimaryReserveAmmoCount" ) ).m_offset;
            m_offsets.m_base_weapon.m_next_primary_attack = ent_offsets.at( HASH( "CBaseCombatWeapon->m_flNextPrimaryAttack" ) ).m_offset;
            m_offsets.m_base_weapon.m_next_secondary_attack = ent_offsets.at( HASH( "CBaseCombatWeapon->m_flNextSecondaryAttack" ) ).m_offset;

            m_offsets.m_weapon_cs_base.m_burst_mode = ent_offsets.at( HASH( "CWeaponCSBase->m_bBurstMode" ) ).m_offset;
            m_offsets.m_weapon_cs_base.m_last_shot_time = ent_offsets.at( HASH( "CWeaponCSBase->m_fLastShotTime" ) ).m_offset;
            m_offsets.m_weapon_cs_base.m_recoil_index = ent_offsets.at( HASH( "CWeaponCSBase->m_flRecoilIndex" ) ).m_offset;
            m_offsets.m_weapon_cs_base.m_postpone_fire_ready_time = ent_offsets.at( HASH( "CWeaponCSBase->m_flPostponeFireReadyTime" ) ).m_offset;

            m_offsets.m_weapon_cs_base_gun.m_zoom_lvl = ent_offsets.at( HASH( "CWeaponCSBaseGun->m_zoomLevel" ) ).m_offset;
            m_offsets.m_weapon_cs_base_gun.m_burst_shots_remaining = ent_offsets.at( HASH( "CWeaponCSBaseGun->m_iBurstShotsRemaining" ) ).m_offset;
            m_offsets.m_weapon_cs_base_gun.m_next_burst_shot = ent_offsets.at( HASH( "CWeaponCSBaseGun->m_fNextBurstShot" ) ).m_offset;

            m_offsets.m_base_combat_character.m_weapon_handle = ent_offsets.at( HASH( "CBaseCombatCharacter->m_hActiveWeapon" ) ).m_offset;
            m_offsets.m_base_combat_character.m_next_attack = ent_offsets.at( HASH( "CBaseCombatCharacter->m_flNextAttack" ) ).m_offset;

            m_offsets.m_base_player.m_tick_base = ent_offsets.at( HASH( "CBasePlayer->m_nTickBase" ) ).m_offset;
            m_offsets.m_base_player.m_life_state = ent_offsets.at( HASH( "CBasePlayer->m_lifeState" ) ).m_offset;
            m_offsets.m_base_player.m_duck_amt = ent_offsets.at( HASH( "CBasePlayer->m_flDuckAmount" ) ).m_offset;
            m_offsets.m_base_player.m_duck_speed = ent_offsets.at( HASH( "CBasePlayer->m_flDuckSpeed" ) ).m_offset;
            m_offsets.m_base_player.m_spawn_time = *BYTESEQ( "89 86 ? ? ? ? E8 ? ? ? ? 80 BE" ).search(
                client.m_start, client.m_end
            ).self_offset( 0x2 ).as< std::uint32_t* >( );
            m_offsets.m_base_player.m_aim_punch = ent_offsets.at( HASH( "CBasePlayer->m_aimPunchAngle" ) ).m_offset;
            m_offsets.m_base_player.m_view_punch = ent_offsets.at( HASH( "CBasePlayer->m_viewPunchAngle" ) ).m_offset;

            m_offsets.m_cs_player.m_lby = ent_offsets.at( HASH( "CCSPlayer->m_flLowerBodyYawTarget" ) ).m_offset;
            m_offsets.m_cs_player.m_eye_angles = ent_offsets.at( HASH( "CCSPlayer->m_angEyeAngles" ) ).m_offset;
#ifndef CSGO2018
            m_offsets.m_cs_player.m_survival_team = ent_offsets.at( HASH( "CCSPlayer->m_nSurvivalTeam" ) ).m_offset;
#endif
            m_offsets.m_cs_player.m_anim_state = *BYTESEQ( "8B 8E ? ? ? ? 85 C9 74 3E" ).search(
                client.m_start, client.m_end
            ).self_offset( 0x2 ).as< std::uint32_t* >( );
            m_offsets.m_cs_player.m_defusing = ent_offsets.at( HASH( "CCSPlayer->m_bIsDefusing" ) ).m_offset;

            m_offsets.m_game_rules.m_warmup_period = ent_offsets.at( HASH( "CCSGameRulesProxy->m_bWarmupPeriod" ) ).m_offset;
            m_offsets.m_game_rules.m_freeze_period = ent_offsets.at( HASH( "CCSGameRulesProxy->m_bFreezePeriod" ) ).m_offset;
            m_offsets.m_game_rules.m_valve_ds = ent_offsets.at( HASH( "CCSGameRulesProxy->m_bIsValveDS" ) ).m_offset;
            m_offsets.m_game_rules.m_bomb_planted = ent_offsets.at( HASH( "CCSGameRulesProxy->m_bBombPlanted" ) ).m_offset;
        }

        /* initialize cvars */
        {
            m_cvars.m_cl_forwardspeed = valve::g_cvar->find_var( "cl_forwardspeed" );
            m_cvars.m_cl_sidespeed = valve::g_cvar->find_var( "cl_sidespeed" );
            m_cvars.m_cl_upspeed = valve::g_cvar->find_var( "cl_upspeed" );

            m_cvars.m_cl_pitchdown = valve::g_cvar->find_var( "cl_pitchdown" );
            m_cvars.m_cl_pitchup = valve::g_cvar->find_var( "cl_pitchup" );

            m_cvars.m_mp_teammates_are_enemies = valve::g_cvar->find_var( "mp_teammates_are_enemies" );
        }

        const code_section_t vguimatsurface{ modules.at( HASH( "vguimatsurface.dll" ) ) };

        HOOK( BYTESEQ( "80 3D ? ? ? ? ? 8B 91 ? ? ? ? 8B 0D ? ? ? ? C6 05 ? ? ? ? 01" ).search(
            vguimatsurface.m_start, vguimatsurface.m_end, false
        ), hooks::lock_cursor, hooks::o_lock_cursor );

        HOOK_VFUNC( valve::g_client, 22u, hooks::create_move_proxy, hooks::o_create_move );

        if ( MH_EnableHook( MH_ALL_HOOKS ) != MH_OK )
            THROW_IF_DBG( "can't enable all hooks." );

    }
}

#undef DETOUR
#undef DETOUR_VFUNC

#undef THROW_IF_DBG