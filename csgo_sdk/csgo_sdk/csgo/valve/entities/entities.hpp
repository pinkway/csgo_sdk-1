#pragma once

namespace csgo::valve {
    struct renderable_t {

    };

    struct networkable_t {

    };

    struct base_entity_t {

    };

    struct base_attributable_item_t : public base_entity_t {

    };

    struct base_weapon_t : public base_attributable_item_t {

    };

    struct base_combat_character_t : public base_entity_t {

    };

    struct base_player_t : public base_combat_character_t {

    };

    struct base_animating_t : public base_player_t {

    };

    struct cs_player_t : public base_animating_t {

    };
}

#include "impl/entities.inl"