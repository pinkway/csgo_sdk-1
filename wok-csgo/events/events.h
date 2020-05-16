#pragma once
#include "../globals.h"
#include "../features/features.h"

namespace events {
	struct callback_t {
		callback_t(uint32_t hash, std::function<void(i_game_event*)> fn) {
			m_hash = hash;
			m_fn = fn;
		}

		uint32_t m_hash;
		std::function<void(i_game_event*)> m_fn;
	};

	class listener : public i_game_event_listener {
	public:
		listener() = default;
		~listener() = default;

		void fire_game_event(i_game_event* event);
		int get_event_debug_id() { return EVENT_DEBUG_ID_INIT; }
	};

	void init();

	void undo();

	void player_hurt(i_game_event* event);

	extern listener m_listener;
	extern std::vector<callback_t> m_callbacks;
}