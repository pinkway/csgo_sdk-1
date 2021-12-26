#pragma once

namespace csgo::valve {
    struct global_vars_base_t {
        float           m_real_time{};
        int             m_frame_count{};

        float           m_abs_frame_time{},
                        m_abs_frame_start_time_std_dev{},
                        m_cur_time{},
                        m_frame_time{};

        int             m_max_clients{}, m_tick_count{};
        float           m_interval_per_tick{}, m_interp_amt{};
        int             m_sim_ticks_this_frame{}, m_network_protocol{};

        sdk::address_t  m_save_data{};
        bool            m_client{}, m_remote_client{};
    } inline* g_global_vars{};
}