#include "../features.h"

void c_movement::on_create_move(bool a1) {
	if (!g::local->is_alive())
		return;

	if (a1) {
		rotate(m_view_angles);

		return compute_buttons();
	}


	/* call all movement related stuff such as auto_strafe etc... here.... */

	compute_buttons();
}

void c_movement::rotate(const qangle_t& wish_angles) {
	if (g::cmd->m_view_angles.z != 0.f
		&& !g::local->get_flags().has(FL_ONGROUND)) {
		g::cmd->m_move.y = 0.f;
	}

	auto move_2d = vec2_t(g::cmd->m_move.x, g::cmd->m_move.y);

	if (const auto speed_2d = move_2d.length()) {
		const auto delta = g::cmd->m_view_angles.y - wish_angles.y;

		vec2_t v1;

		math::sin_cos(
			math::deg_to_rad(
				remainderf(math::rad_to_deg(math::atan2(move_2d.y / speed_2d, move_2d.x / speed_2d)) + delta, 360.f)
			), v1.x, v1.y
		);

		const auto cos_x = math::cos(
			math::deg_to_rad(remainderf(math::rad_to_deg(math::atan2(0.f, speed_2d)), 360.f))
		);

		move_2d.x = cos_x * v1.y * speed_2d;
		move_2d.y = cos_x * v1.x * speed_2d;

		if (g::local->get_move_type() == MOVE_TYPE_LADDER) {
			if (wish_angles.x < 45.f
				&& std::fabsf(delta) <= 65.f
				&& g::cmd->m_view_angles.x >= 45.f) {
				move_2d.x *= -1.f;
			}
		}
		else if (std::fabsf(g::cmd->m_view_angles.x) > 90.f) {
			move_2d.x *= -1.f;
		}
	}

	static const auto cl_forwardspeed = interfaces::cvar_system->find_var(_("cl_forwardspeed"));
	static const auto cl_sidespeed = interfaces::cvar_system->find_var(_("cl_sidespeed"));
	static const auto cl_upspeed = interfaces::cvar_system->find_var(_("cl_upspeed"));

	const auto max_forward_speed = cl_forwardspeed->get_float();
	const auto max_side_speed = cl_sidespeed->get_float();
	const auto max_up_speed = cl_upspeed->get_float();

	g::cmd->m_move = vec3_t(
		math::clamp(move_2d.x, -max_forward_speed, max_forward_speed),
		math::clamp(move_2d.y, -max_side_speed, max_side_speed),
		math::clamp(g::cmd->m_move.z, -max_up_speed, max_up_speed)
	);
}

void c_movement::compute_buttons() {
	g::cmd->m_buttons.remove(IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT);

	if (g::local->get_move_type() == MOVE_TYPE_LADDER) {
		if (std::fabsf(g::cmd->m_move.x) > 200.f) {
			g::cmd->m_buttons.add(g::cmd->m_move.x > 0.f ? IN_FORWARD : IN_BACK);
		}

		if (std::fabsf(g::cmd->m_move.y) > 200.f) {
			g::cmd->m_buttons.add(g::cmd->m_move.y > 0.f ? IN_MOVERIGHT : IN_MOVELEFT);
		}

		return;
	}

	if (g::cmd->m_move.x != 0.f) {
		g::cmd->m_buttons.add(g::cmd->m_move.x > 0.f ? IN_FORWARD : IN_BACK);
	}

	if (g::cmd->m_move.y != 0.f) {
		g::cmd->m_buttons.add(g::cmd->m_move.y > 0.f ? IN_MOVERIGHT : IN_MOVELEFT);
	}
}