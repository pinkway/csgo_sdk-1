#include "../hooks.h"

bool __stdcall hooks::create_move::fn(float input_sample_time, c_user_cmd* cmd) {
	static const auto original = client_mode->get_original<t>(index);

	if (!cmd 
		|| !cmd->m_commandnumber)
		return original(input_sample_time, cmd);

	g::cmd = cmd;
	g::send_packet = true;

	c_prediction::get().pre_start();

	c_prediction::get().start(g::local, cmd); {

	}
	c_prediction::get().end(g::local, cmd);

	math::normalize_angles(cmd->m_viewangles);

	cmd->m_move.x = std::clamp(cmd->m_move.x, -450.f, 450.f);
	cmd->m_move.y = std::clamp(cmd->m_move.y, -450.f, 450.f);
	cmd->m_move.z = std::clamp(cmd->m_move.z, -320.f, 320.f);

	*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14) = g::send_packet;

	return false;
}