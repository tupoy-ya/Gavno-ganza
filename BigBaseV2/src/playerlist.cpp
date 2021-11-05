#include "common.hpp"
#include "playerlist.hpp"
#include "pointers.hpp"
#include "features.hpp"
#include "gta/natives.hpp"
#include "natives.hpp"

namespace big
{
    void UpdatePlayer()
    {
        for (int32_t i = 0; i < gta::num_players; i++)
        {
            if (auto net_player = g_pointers->m_GetNetPlayer(i))
            {
                g_player[i].m_net_player = net_player;
                g_player[i].m_name = PLAYER::GET_PLAYER_NAME(i);
                const char* RockstarIDBuffer;
                int NETWORK_HANDLE[76];
                NETWORK::NETWORK_HANDLE_FROM_PLAYER(i, &NETWORK_HANDLE[0], 13);
                if (NETWORK::NETWORK_IS_HANDLE_VALID(&NETWORK_HANDLE[0], 13))
                {
                    RockstarIDBuffer = NETWORK::NETWORK_MEMBER_ID_FROM_GAMER_HANDLE(&NETWORK_HANDLE[0]);
                }
                else { RockstarIDBuffer = "0"; }
                g_player[i].m_str_rockstar_id = RockstarIDBuffer;

            }
        }
    }
}