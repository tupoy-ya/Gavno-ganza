#pragma once

#include "common.hpp"
#include "natives.hpp"

namespace big
{
	class base_tab
	
	{
	public:
		static void render_base_tab();
		static void render_players_tab();
		static void render_outfit_tab();
	};
	inline std::uint32_t g_SelectedPlayer{};
}