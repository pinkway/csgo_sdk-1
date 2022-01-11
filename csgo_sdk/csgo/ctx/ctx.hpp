#pragma once

namespace csgo {
    namespace valve {
        struct cvar_t;
    }

    class c_ctx {
    private:
        struct {
            sdk::address_t  m_local_player{},
                            m_weapon_system{},
                            m_user_cmd_checksum{};

            struct {
                sdk::address_t m_reset{}, m_update{};
            }               m_anim_state{};

            struct {
                std::uint32_t m_bone_cache{}, m_mdl_bone_cnt{};
            }               m_renderable{};

            struct {
                std::uint32_t   m_health{}, m_team_num{},
                                m_sim_time{}, m_flags{},
                                m_origin{}, m_velocity{},
                                m_abs_origin{}, m_abs_velocity{},
                                m_abs_rotation{}, m_move_type{},
                                m_mins{}, m_maxs{};
            }               m_base_entity{};

            struct {
                std::uint32_t   m_sequence{},
                                m_hitbox_set_index{}, m_studio_hdr{},
                                m_pose_params{}, m_anim_layers{};
            }               m_base_animating{};

            struct {
                std::uint32_t   m_pin_pulled{},
                                m_throw_time{}, m_throw_strength{};
            }               m_base_grenade{};

            struct {
                std::uint32_t   m_item_index{};
            }               m_base_attributable_item{};

            struct {
                std::uint32_t   m_clip1{}, m_primary_reserve_ammo_count{},
                                m_next_primary_attack{}, m_next_secondary_attack{};
            }               m_base_weapon{};

            struct {
                std::uint32_t   m_burst_mode{},
                                m_last_shot_time{}, m_recoil_index{},
                                m_postpone_fire_ready_time{};
            }               m_weapon_cs_base{};

            struct {
                std::uint32_t   m_zoom_lvl{},
                                m_burst_shots_remaining{}, m_next_burst_shot{};
            }               m_weapon_cs_base_gun{};

            struct {
                std::uint32_t   m_weapon_handle{}, m_next_attack{};
            }               m_base_combat_character{};

            struct {
                std::uint32_t   m_tick_base{}, m_life_state{},
                                m_duck_amt{}, m_duck_speed{},
                                m_spawn_time{}, m_aim_punch{},
                                m_view_punch{};
            }               m_base_player{};

            struct {
                std::uint32_t   m_lby{}, m_eye_angles{},
#ifndef CSGO2018
                                m_survival_team{},
#endif
                                m_anim_state{}, m_defusing{};
            }               m_cs_player{};

            struct {
                std::uint32_t   m_warmup_period{}, m_freeze_period{},
                                m_valve_ds{}, m_bomb_planted{};
            }               m_game_rules{};
        } m_offsets{};

        struct {
            valve::cvar_t*  m_cl_forwardspeed{},
                            *m_cl_sidespeed{}, *m_cl_upspeed{},
                            *m_cl_pitchdown{}, *m_cl_pitchup{},
                            *m_mp_teammates_are_enemies{};
        } m_cvars{};
    public:
        void init( );

        ALWAYS_INLINE const auto& offsets( ) const;

        ALWAYS_INLINE const auto& cvars( ) const;
    };

    inline const auto g_ctx = std::make_unique< c_ctx >( );
}

#include "impl/ctx.inl"