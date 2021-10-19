#include "common.hpp"
#include "features.hpp"
#include "script.hpp"
#include "natives.hpp"
#include "gui/base_tab.h"
#include "playerlist.hpp"
#include "pointers.hpp"
#include "gta/player.hpp"
#include "fiber_pool.hpp"
#include "imgui.h"
#include "misc.h"
#include "gta/Weapons.h"
#include "script_local.hpp"
#include "hooking.hpp"
#include "script_global.hpp"
#include "random.h"
#include "gta_util.hpp"
#include "gta/joaat.hpp"


/*
*
* 	natives
*	SET_BLIP_SPRITE
*	
*	
*
*/


namespace big::features
{

	namespace online {
		void features::online::draw_extra_info()
		{
			if (CNetGamePlayer* net_player = g_pointers->m_GetNetPlayer(online::selectedPlayer))
			{
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

				if (ImGui::BeginChild("Player Menu", ImVec2(700, 700), true, window_flags))
				{
					if (ImGui::TreeNodeEx("Player Info", ImGuiWindowFlags_NoCollapse))
					{

						//Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(ped);
						//Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, false);
						const char* name = g_player[features::online::selectedPlayer].m_name.c_str();
						const char* rid = g_player[features::online::selectedPlayer].m_str_rockstar_id.c_str();
						//const char* numberplate = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer)));
						//const int ammoinweapon = WEAPON::GET_AMMO_IN_PED_WEAPON(ped, WEAPON::GET_SELECTED_PED_WEAPON(ped));
						//static bool godmode = PLAYER::GET_PLAYER_INVINCIBLE(online::selectedPlayer);
						//const int pedmoney = PED::GET_PED_MONEY(ped);
						//const int interior = INTERIOR::GET_INTERIOR_AT_COORDS(coords.x, coords.y, coords.z);
						//
						//ImGui::Checkbox("Godmode?", &godmode);
						//{
						//	godmode = PLAYER::GET_PLAYER_INVINCIBLE(online::selectedPlayer);
						//}
						//ImGui::SameLine();
						ImGui::Text("Username: %s | RID: %s", name, rid);
						//ImGui::Text("Number Plate: %s | Ammo in Weapon: %s | Money %s | Interior ID %s", pedmoney);
					}

					ImGui::Separator();

					static bool spectate = false;
					if (ImGui::Checkbox("Spectate", &spectate))
					{
							NETWORK::NETWORK_SET_IN_SPECTATOR_MODE_EXTENDED(spectate, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer), 1);
							HUD::SET_MINIMAP_IN_SPECTATOR_MODE(spectate, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));

					}

					if (ImGui::Button("Give all weapons"))
					{
						g_fiber_pool->queue_job([]
						{
							features::weapons::give_all_weps(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
						});
					}

					ImGui::SameLine();

					if (ImGui::Button("Give all MK2 weapons"))
					{
						g_fiber_pool->queue_job([]
						{
							features::weapons::give_all_mk2(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
						});
					}

					if (ImGui::Button("Give Stun Gun"))
					{
						g_fiber_pool->queue_job([]
						{
							features::weapons::give_stun_gun(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
						});
					}

					ImGui::SameLine();

					if (ImGui::Button("Give Scanner"))
					{
						g_fiber_pool->queue_job([]
						{
							features::weapons::give_scanner(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
						});
					}
					

					if (ImGui::Button("Off Radar"))
					{
						*script_global(2426865).at(online::selectedPlayer, 449).at(209).as<int*>() = 1;
						*script_global(2441237).at(70).as<int*>() = NETWORK::GET_NETWORK_TIME();
						*script_global(2544210).at(4628).as<int*>() = 4;
					}
					ImGui::Separator();

					if (ImGui::Button("Teleport"))
					{
						features::online::teleport_to_player(online::selectedPlayer);
					}

					ImGui::SameLine();

					if (ImGui::Button("Teleport To Player Vehicle"))
					{
						g_fiber_pool->queue_job([]
						{
							Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
							PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -2);
						});
					}

					ImGui::SameLine();

					if (ImGui::Button("Parachute To Player"))
					{
						auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer), true);
						ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), pos.x, pos.y, pos.z + 1000, false, false, false, false);
					}

					ImGui::SameLine();

					if (ImGui::Button("Cage"))
					{
						features::online::cage(online::selectedPlayer);
					}
					
					/*if (ImGui::Button("CRASH"))
					{
						gta_util::execute_as_script(RAGE_JOAAT("freemode"), [] 
						{
							
						});
					}*/
					
					
					ImGui::Checkbox("Freeze", &features::online::Freeze_player_bool);
					
					ImGui::SameLine();
					if (ImGui::Button("Steal Outfit"))
					{
						g_fiber_pool->queue_job([]
						{
							Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer);
							Ped player_ped = PLAYER::PLAYER_PED_ID();
							PED::SET_PED_COMPONENT_VARIATION(player_ped, 1, PED::GET_PED_DRAWABLE_VARIATION(ped, 1), PED::GET_PED_TEXTURE_VARIATION(ped, 1), 0);
							//PED::SET_PED_COMPONENT_VARIATION(player_ped, 3, PED::GET_PED_DRAWABLE_VARIATION(ped, 3), 1, 0);
							PED::SET_PED_COMPONENT_VARIATION(player_ped, 4, PED::GET_PED_DRAWABLE_VARIATION(ped, 4), PED::GET_PED_TEXTURE_VARIATION(ped, 4), 0);
							//PED::SET_PED_COMPONENT_VARIATION(player_ped, 5, PED::GET_PED_DRAWABLE_VARIATION(ped, 5), 1, 0);
							PED::SET_PED_COMPONENT_VARIATION(player_ped, 6, PED::GET_PED_DRAWABLE_VARIATION(ped, 6), PED::GET_PED_TEXTURE_VARIATION(ped, 6), 0);
							PED::SET_PED_COMPONENT_VARIATION(player_ped, 7, PED::GET_PED_DRAWABLE_VARIATION(ped, 7), PED::GET_PED_TEXTURE_VARIATION(ped, 7), 0);
							PED::SET_PED_COMPONENT_VARIATION(player_ped, 8, PED::GET_PED_DRAWABLE_VARIATION(ped, 8), PED::GET_PED_TEXTURE_VARIATION(ped, 8), 0);
							PED::SET_PED_COMPONENT_VARIATION(player_ped, 9, PED::GET_PED_DRAWABLE_VARIATION(ped, 9), PED::GET_PED_TEXTURE_VARIATION(ped, 9), 0);
							PED::SET_PED_COMPONENT_VARIATION(player_ped, 10, PED::GET_PED_DRAWABLE_VARIATION(ped, 10), PED::GET_PED_TEXTURE_VARIATION(ped, 10), 0);
							PED::SET_PED_COMPONENT_VARIATION(player_ped, 11, PED::GET_PED_DRAWABLE_VARIATION(ped, 11), PED::GET_PED_TEXTURE_VARIATION(ped, 11), 0);
							PED::SET_PED_PROP_INDEX(player_ped, 0, PED::GET_PED_PROP_INDEX(ped, 0), PED::GET_PED_PROP_TEXTURE_INDEX(ped, 0), true);
							PED::SET_PED_PROP_INDEX(player_ped, 1, PED::GET_PED_PROP_INDEX(ped, 1), PED::GET_PED_PROP_TEXTURE_INDEX(ped, 1), true);
						});
					}
					if (ImGui::Button("Steal Outfit22"))
					{
						Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer);
						
						LOG(INFO) << PED::GET_PED_DRAWABLE_VARIATION(ped, 11);


					}
					if (ImGui::Button("Spawn Grifer Gesus"))
					{
						
						features::objects::spawnGriferPed("u_m_m_jesus_01", online::selectedPlayer);
					} 
					
					ImGui::SameLine();

					if (ImGui::Button("Spawn Extreme Grifer Gesus"))
					{

						features::objects::spawnExtimeGriferPed("u_m_m_jesus_01", online::selectedPlayer);
					}


					if (ImGui::Button("Spawn H Jet"))
					{
						features::objects::spawnGriferJet("u_m_m_jesus_01", "lazer", online::selectedPlayer, false);
					}

					if (ImGui::Button("Spawn E UFO"))
					{
						features::objects::spawnGriferJet("u_m_m_jesus_01", "lazer", online::selectedPlayer, true);
					}

					if (ImGui::Button("Spawn S Pyr"))
					{
						features::objects::spawnGriferJet("u_m_m_jesus_01", "pyro", online::selectedPlayer, false);
					}

					if (ImGui::Button("Spawn G Bus"))
					{
						features::objects::spawnGriferVehicle("u_m_m_jesus_01", "apc", online::selectedPlayer);
					}
					

					ImGui::SameLine();

					if (ImGui::Button("Fix Vehicle"))
					{
						g_fiber_pool->queue_job([]
						{
							NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));

							while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false)))
								script::get_current()->yield(5ms);
							*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
							VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer)));
							*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
						});
					}

					ImGui::SameLine();
					//if (ImGui::Button("Copy"))
					//{
					//	PED::CLONE_PED_TO_TARGET(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer), PLAYER::PLAYER_PED_ID());
					//}

					//ImGui::SameLine();

					if (ImGui::Button("ragdoll"))
					{
						g_fiber_pool->queue_job([]
						{
							TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
							auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer), true);
							FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 13, 1, false, true, 0, false);
						});
					}

					if (ImGui::Button("Full Acceleration"))
					{
						g_fiber_pool->queue_job([]
						{
							Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
							NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);

							while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
								script::get_current()->yield(5ms);
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, VEHICLE::GET_VEHICLE_MODEL_ESTIMATED_MAX_SPEED(ENTITY::GET_ENTITY_MODEL(veh)) * 2);
						});
					}


					static char Plate_Text[255];
					ImGui::InputText("##Lisence_Plate", Plate_Text, 255);

					ImGui::SameLine();

					if (ImGui::Button("Plate Cange"))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
						features::vehicles::set_plate(veh, Plate_Text);
					}

					if (ImGui::Button("Clear tasks"))
					{
						g_fiber_pool->queue_job([]
						{
							TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
						});
					}


					static char* exlosions[] = {
	"GRENADE",
	"GRENADELAUNCHER",
	"STICKYBOMB",
	"MOLOTOV",
	"ROCKET",
	"TANKSHELL",
	"HI_OCTANE",
	"CAR",
	"PLANE",
	"PETROL_PUMP",
	"BIKE",
	"DIR_STEAM",
	"DIR_FLAME",
	"DIR_WATER_HYDRANT",
	"DIR_GAS_CANISTER",
	"BOAT",
	"SHIP_DESTROY",
	"TRUCK",
	"BULLET",
	"SMOKEGRENADELAUNCHER",
	"SMOKEGRENADE",
	"BZGAS",
	"FLARE",
	"GAS_CANISTER",
	"EXTINGUISHER",
	"_0x988620B8",
	"EXP_TAG_TRAIN",
	"EXP_TAG_BARREL",
	"EXP_TAG_PROPANE",
	"EXP_TAG_BLIMP",
	"EXP_TAG_DIR_FLAME_EXPLODE",
	"EXP_TAG_TANKER",
	"PLANE_ROCKET",
	"EXP_TAG_VEHICLE_BULLET",
	"EXP_TAG_GAS_TANK",
	"EXP_TAG_BIRD_CRAP",
	"EXP_TAG_RAILGUN",
	"EXP_TAG_BLIMP2",
	"EXP_TAG_FIREWORK",
	"EXP_TAG_SNOWBALL",
	"EXP_TAG_PROXMINE",
	"EXP_TAG_VALKYRIE_CANNON",
	"EXP_TAG_AIR_DEFENCE",
	"EXP_TAG_PIPEBOMB",
	"EXP_TAG_VEHICLEMINE",
	"EXP_TAG_EXPLOSIVEAMMO",
	"EXP_TAG_APCSHELL",
	"EXP_TAG_BOMB_CLUSTER",
	"EXP_TAG_BOMB_GAS",
	"EXP_TAG_BOMB_INCENDIARY",
	"EXP_TAG_BOMB_STANDARD",
	"EXP_TAG_TORPEDO",
	"EXP_TAG_TORPEDO_UNDERWATER",
	"EXP_TAG_BOMBUSHKA_CANNON",
	"EXP_TAG_BOMB_CLUSTER_SECONDARY",
	"EXP_TAG_HUNTER_BARRAGE",
	"EXP_TAG_HUNTER_CANNON",
	"EXP_TAG_ROGUE_CANNON",
	"EXP_TAG_MINE_UNDERWATER",
	"EXP_TAG_ORBITAL_CANNON",
	"EXP_TAG_BOMB_STANDARD_WIDE",
	"EXP_TAG_EXPLOSIVEAMMO_SHOTGUN",
	"EXP_TAG_OPPRESSOR2_CANNON",
	"EXP_TAG_MORTAR_KINETIC",
	"EXP_TAG_VEHICLEMINE_KINETIC",
	"EXP_TAG_VEHICLEMINE_EMP",
	"EXP_TAG_VEHICLEMINE_SPIKE",
	"EXP_TAG_VEHICLEMINE_SLICK",
	"EXP_TAG_VEHICLEMINE_TAR",
	"EXP_TAG_SCRIPT_DRONE",
	"EXP_TAG_RAYGUN",
	"EXP_TAG_BURIEDMINE",
	"EXP_TAG_SCRIPT_MISSILE",
	"EXP_TAG_RCTANK_ROCKET",
	"EXP_TAG_BOMB_WATER",
	"EXP_TAG_BOMB_WATER_SECONDARY",
	"_0xF728C4A9",
	"_0xBAEC056F",
	"EXP_TAG_FLASHGRENADE",
	"EXP_TAG_STUNGRENADE",
	"_0x763D3B3B",
	"EXP_TAG_SCRIPT_MISSILE_LARGE",
	"EXP_TAG_SUBMARINE_BIG",	};

					static int expl_selected = 0;

					ImGui::Separator();

					ImGui::Combo("##type", &expl_selected, exlosions, sizeof(exlosions) / sizeof(*exlosions));

					static bool isAudible = false;
					static bool isInvisible = false;
					static bool Enable_damage = false;
					static float cameraShake = 0.f;

					if (ImGui::Button("Spawn explosion"))
					{
						g_fiber_pool->queue_job([]
						{
							auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer), true);
							FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, expl_selected, 1, isAudible, isInvisible, cameraShake, Enable_damage);
						});
					}
					ImGui::SameLine();
					ImGui::Checkbox("isAudible", &isAudible);
					ImGui::SameLine();
					ImGui::Checkbox("isInvisible", &isInvisible);
					ImGui::SameLine();
					ImGui::Checkbox("Enable_damage", &Enable_damage);
					ImGui::SliderFloat("cameraShake", &cameraShake, 0.f, 100.f);

					if (ImGui::Button("Launch Vehicle Up"))
					{
						g_fiber_pool->queue_job([]
						{

							Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
							NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);

							while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
								script::get_current()->yield(5ms);

							Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(veh);
							

							ENTITY::SET_ENTITY_VELOCITY(veh, vel.x, vel.y, 100.f);
						});
					}

					if (ImGui::TreeNode("Weapons"))
					{
						static char weapon_name[255];
						ImGui::InputText("Weapon name", weapon_name, 255);

						ImGui::Separator();

						if (ImGui::Button("Give Weapon"))
						{
							Hash hash = MISC::GET_HASH_KEY(weapon_name);
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer), hash, 1000, 1);
						}

						ImGui::SameLine();

						if (ImGui::Button("Remove Weapon"))
						{
							Hash hash = MISC::GET_HASH_KEY(weapon_name);
							WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer), hash);
						}
						if (ImGui::Button("Remove Corret Weapon"))
						{
							Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer);
							Hash hash = WEAPON::GET_SELECTED_PED_WEAPON(ped);
							WEAPON::REMOVE_WEAPON_FROM_PED(ped, hash);
						}
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Objects"))
					{
						static char* objects[] = { "prop_bskball_01", "PROP_MP_RAMP_03", "PROP_MP_RAMP_02", "PROP_MP_RAMP_01", "PROP_JETSKI_RAMP_01", "PROP_WATER_RAMP_03", "PROP_VEND_SNAK_01", "PROP_TRI_START_BANNER", "PROP_TRI_FINISH_BANNER", "PROP_TEMP_BLOCK_BLOCKER", "PROP_SLUICEGATEL", "PROP_SKIP_08A", "PROP_SAM_01", "PROP_RUB_CONT_01B", "PROP_ROADCONE01A", "PROP_MP_ARROW_BARRIER_01", "PROP_HOTEL_CLOCK_01", "PROP_LIFEBLURB_02", "PROP_COFFIN_02B", "PROP_MP_NUM_1", "PROP_MP_NUM_2", "PROP_MP_NUM_3", "PROP_MP_NUM_4", "PROP_MP_NUM_5", "PROP_MP_NUM_6", "PROP_MP_NUM_7", "PROP_MP_NUM_8", "PROP_MP_NUM_9", "prop_xmas_tree_int", "prop_bumper_car_01", "prop_beer_neon_01", "prop_space_rifle", "prop_dummy_01", "prop_rub_trolley01a", "prop_wheelchair_01_s", "PROP_CS_KATANA_01", "PROP_CS_DILDO_01", "prop_armchair_01", "prop_bin_04a", "prop_chair_01a", "prop_dog_cage_01", "prop_dummy_plane", "prop_golf_bag_01", "prop_arcade_01", "hei_prop_heist_emp", "prop_alien_egg_01", "prop_air_towbar_01", "hei_prop_heist_tug", "prop_air_luggtrolley", "PROP_CUP_SAUCER_01", "prop_wheelchair_01", "prop_ld_toilet_01", "prop_acc_guitar_01", "prop_bank_vaultdoor", "p_v_43_safe_s", "p_spinning_anus_s", "prop_can_canoe", "prop_air_woodsteps", "Prop_weed_01", "prop_a_trailer_door_01", "prop_apple_box_01", "prop_air_fueltrail1", "prop_barrel_02a", "prop_barrel_float_1", "prop_barrier_wat_03b", "prop_air_fueltrail2", "prop_air_propeller01", "prop_windmill_01", "prop_Ld_ferris_wheel", "p_tram_crash_s", "p_oil_slick_01", "p_ld_stinger_s", "p_ld_soc_ball_01", "prop_juicestand", "p_oil_pjack_01_s", "prop_barbell_01", "prop_barbell_100kg", "p_parachute1_s", "p_cablecar_s", "prop_beach_fire", "prop_lev_des_barge_02", "prop_lev_des_barge_01", "prop_a_base_bars_01", "prop_beach_bars_01", "prop_air_bigradar", "prop_weed_pallet", "prop_artifact_01", "prop_attache_case_01", "prop_large_gold", "prop_roller_car_01", "prop_water_corpse_01", "prop_water_corpse_02", "prop_dummy_01", "prop_atm_01", "hei_prop_carrier_docklight_01", "hei_prop_carrier_liferafts", "hei_prop_carrier_ord_03", "hei_prop_carrier_defense_02", "hei_prop_carrier_defense_01", "hei_prop_carrier_radar_1", "hei_prop_carrier_radar_2", "hei_prop_hei_bust_01", "hei_prop_wall_alarm_on", "hei_prop_wall_light_10a_cr", "prop_afsign_amun", "prop_afsign_vbike", "prop_aircon_l_01", "prop_aircon_l_02", "prop_aircon_l_03", "prop_aircon_l_04", "prop_airhockey_01", "prop_air_bagloader", "prop_air_blastfence_01", "prop_air_blastfence_02", "prop_air_cargo_01a", "prop_air_chock_01", "prop_air_chock_03", "prop_air_gasbogey_01", "prop_air_generator_03", "prop_air_stair_02", "prop_amb_40oz_02", "prop_amb_40oz_03", "prop_amb_beer_bottle", "prop_amb_donut", "prop_amb_handbag_01", "prop_amp_01", "prop_anim_cash_pile_02", "prop_asteroid_01", "prop_arm_wrestle_01", "prop_ballistic_shield", "prop_bank_shutter", "prop_barier_conc_02b", "prop_barier_conc_05a", "prop_barrel_01a", "prop_bar_stool_01", "prop_basejump_target_01" };

						static int combo_pos = 0;

						ImGui::Separator();

						ImGui::SliderFloat("x_rot", &objects::x_rot, 0.f, 360.f);
						ImGui::SliderFloat("y_rot", &objects::y_rot, 0.f, 360.f);
						ImGui::SliderFloat("z_rot", &objects::z_rot, 0.f, 360.f);
						ImGui::SliderFloat("x_pos", &objects::x_pos, -10.f, 10.f);
						ImGui::SliderFloat("y_pos", &objects::y_pos, -10.f, 10.f);
						ImGui::SliderFloat("z_pos", &objects::z_pos, -10.f, 10.f);

						ImGui::Combo("Combo", &combo_pos, objects, sizeof(objects) / sizeof(*objects));

						ImGui::SameLine();

						if (ImGui::Button("Attach"))
						{
							g_fiber_pool->queue_job([]
							{
								features::objects::attach_obj_to_player(objects[combo_pos], online::selectedPlayer);
							});
						}

						if (ImGui::Button("Attach All"))
						{
							g_fiber_pool->queue_job([]
							{
								features::objects::attach_obj_to_player(objects[combo_pos], online::selectedPlayer);
							});
						}

						ImGui::SameLine();

						if (ImGui::Button("Detach all objects"))
						{
							g_fiber_pool->queue_job([]
							{
								features::objects::detach_all(online::selectedPlayer);
							});
						}
						ImGui::TreePop();
					}

					/*
					if (ImGui::TreeNode("Info"))
					{
						if (ImGui::Button("Update"))
						{
							g_fiber_pool->queue_job([]
							{
								Ped ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer);
								Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(ped);
								Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, false);
								const int interior = INTERIOR::GET_INTERIOR_AT_COORDS(coords.x, coords.y, coords.z);
								static bool godmode = PLAYER::GET_PLAYER_INVINCIBLE(online::selectedPlayer);
								const int ammoinweapon = WEAPON::GET_AMMO_IN_CLIP(ped, WEAPON::GET_SELECTED_PED_WEAPON(ped));
								LOG(INFO) << interior;
								LOG(INFO) << godmode;
								LOG(INFO) << ammoinweapon;

							});
						}
						ImGui::TreePop();
					}
					*/
				}
				ImGui::EndChild();
			}
		}

		//_GET_ALL_VEHICLES
		//void features::online::show_aircraft()
		//{
		//	
		//}

		void features::online::show_players()
		{

			auto wSize = ImGui::GetWindowSize();

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar;
			float x = (wSize.x / 2) - 300;
			float y = wSize.y - 100;
			if (ImGui::BeginChild("Player List", ImVec2(x, y), true, window_flags))
			{
				for (Player i = 0; i < gta::num_players; i++)
				{
					if (CNetGamePlayer* net_player = g_pointers->m_GetNetPlayer(i))
					{
						if (ImGui::Selectable(g_player[i].m_name.c_str(), false))
						{
							online::selectedPlayer = i;
						}
					}
				}
			}
			ImGui::EndChild();

			ImGui::SameLine();
		}

		int features::online::selectedPlayer = 0;

		void features::online::teleport_to_player(Player player) {
			Entity handle;
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), false);
			PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : handle = PLAYER::PLAYER_PED_ID();
			ENTITY::SET_ENTITY_COORDS(handle, coords.x, coords.y, coords.z, false, false, false, false);
		}
		void features::online::cage(Player player)
		{
			Vector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0);
			Object obj = OBJECT::CREATE_OBJECT(features::basic::$("prop_gold_cont_01"), remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
		}
		bool features::online::block_afk_bool = false;
		bool features::online::Freeze_player_bool = false;

		void features::online::block_afk()
		{
			if (block_afk_bool)
			{
				misc::set_global(2550148 + 296, -1000000);
				misc::set_global(1377236 + 1149, -1000000);
			}
			else
			{
				misc::set_global(2550148 + 296, 0);
				misc::set_global(1377236 + 1149, 0);
			}
		}
		void features::online::Freeze_player()
		{
			if (Freeze_player_bool)
			{
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer));
			}
		}
		void features::online::change_clothe(int componentId, int drawableId, int textureId, int paletteId)
		{
			Player ped = PLAYER::PLAYER_PED_ID();
			PED::SET_PED_COMPONENT_VARIATION(ped, componentId, drawableId, textureId, paletteId);
		}
		void features::online::disable_phone(bool phone_disabled)
		{
			if (phone_disabled)
			{
				misc::set_global(19664 + 1, 1);
			}
			else
			{
				misc::set_global(19664 + 1, 0);
			}
		}
		void features::online::bst()
		{
			misc::set_global(2440049 + 4006, 3);
		}
		void features::online::bribe()
		{
			misc::set_global(2540384 + 4024, 5);
			misc::set_global(2540384 + 4620, 1);
			misc::set_global(2537071 + 4622, static_cast<int64_t>(NETWORK::GET_NETWORK_TIME()) + 720000);
		}
		void features::online::no_orb_cooldown()
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ORBITAL_CANNON_COOLDOWN"), 0, 0);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ORBITAL_CANNON_COOLDOWN"), 0, 0);
		}
		void features::online::ewo_cooldown_remove()
		{
			misc::set_global(2540384 + 6672, 0);
		}

		void features::online::grinding_mode()
		{
			misc::set_global(2451787 + 742, 1);
		}

		void features::online::block_vote()
		{
			//Global_1388057 = func_16450(joaat("MPPLY_BECAME_CHEATER_NUM"));
			//Global_1388059 = func_16450(joaat("MPPLY_OFFENSIVE_LANGUAGE"));
			//Global_1388060 = func_16450(joaat("MPPLY_GRIEFING"));
			//Global_1388062 = func_16450(joaat("MPPLY_OFFENSIVE_TAGPLATE"));
			//Global_1388063 = func_16450(joaat("MPPLY_OFFENSIVE_UGC"));

			misc::set_global(1388057, 0);
			misc::set_global(1388059, 0);
			misc::set_global(1388060, 0);
			misc::set_global(1388062, 0);
			misc::set_global(1388063, 0);
		}

/*		void features::online::AirstrikePlayer() {
			Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer);
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
			Hash airStrike = MISC::GET_HASH_KEY("WEAPON_AIRSTRIKE_ROCKET");
			WEAPON::REQUEST_WEAPON_ASSET(airStrike, 31, false);
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords.x, coords.y, coords.z + 50.f, coords.x, coords.y, coords.z, 250, 1, airStrike, playerPed, 1, 0, -1.0);
			*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
		}
		*/
	}

	namespace basic {
		float features::basic::deg_to_rad(float degs)
		{
			return degs * 3.141592653589793f / 180.f;
		}
		Hash features::basic::load(char* name)
		{
			Hash hash = MISC::GET_HASH_KEY(name);

			STREAMING::REQUEST_MODEL(hash);
			while (!STREAMING::HAS_MODEL_LOADED(hash))
			{
				script::get_current()->yield();
			}
			return hash;
		}
		Hash features::basic::$(std::string str) {
			return MISC::GET_HASH_KEY(&str[0u]);
		}
	}

	namespace recovery {


		//Fast Spinning
		//void features::recovery::sameValue() // only loop it
		//{
		//	auto CASINO_SLOTS = find_script_thread(RAGE_JOAAT("casino_slots"));
		//	auto checker = *script_local::script_local(CASINO_SLOTS, 3401).at(PLAYER::PLAYER_ID(), 11).at(10).as<int*>();
		//	if (checker == 3)
		//	{
		//		MISC::SET_BIT(checker, 2);
		//	}
		//}

		void features::recovery::packed_bool(int loop, int looptwo, char* boolname)
		{
			if ((boolname != NULL) && (boolname[0] == '\0')) {
				for (int i = loop; i <= looptwo; i++)
				{
					STATS::STAT_SET_BOOL_MASKED(STATS::_GET_NGSTAT_BOOL_HASH((i - loop), 0, 1, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), boolname), TRUE, ((i - loop) - STATS::_0xF4D8E7AC2A27758C((i - loop)) * 64), 1);
				}
			}
		}

		void features::recovery::reset_reports()
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_FRIENDLY"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_HELPFUL"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_GRIEFING"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_VC_ANNOYINGME"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_VC_HATE"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_TAGPLATE"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_NAME"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_MOTTO"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_STATUS"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_EMBLEM"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_EXPLOITS"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_ISPUNISHED"), 0, 1);
		}

		void features::recovery::remove_bad_sport()
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_BAD_SPORT_BITSET"), 0, 0);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_WAS_I_BAD_SPORT"), 0, 0);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_OVERALL_BADSPORT"), 0, 0);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_CHAR_IS_BADSPORT"), 0, 0);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_BECAME_BADSPORT_NUM"), 0, 0);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MPPLY_DESTROYED_PVEHICLES"), 0, 0);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_REPORT_STRENGTH"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_COMMEND_STRENGTH"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_FRIENDLY"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_HELPFUL"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_GRIEFING"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_VC_ANNOYINGME"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_VC_HATE"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_LANGUAGE"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_TAGPLATE"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_OFFENSIVE_UGC"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_NAME"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_MOTTO"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_STATUS"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_BAD_CREW_EMBLEM"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_GAME_EXPLOITS"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_EXPLOITS"), 0, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_ISPUNISHED"), 0, 1);
		}

		void features::recovery::unlock_fast_run()
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CHAR_ABILITY_1_UNLCK"), -1, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CHAR_ABILITY_2_UNLCK"), -1, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CHAR_ABILITY_3_UNLCK"), -1, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CHAR_FM_ABILITY_1_UNLCK"), -1, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CHAR_FM_ABILITY_2_UNLCK"), -1, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CHAR_FM_ABILITY_3_UNLCK"), -1, 1);
		}

		void features::recovery::unlock_wireframe_suits()
		{
			misc::set_global(262145 + 22912, 1);
			misc::set_global(262145 + 22913, 1);
			misc::set_global(262145 + 22914, 1);
			misc::set_global(262145 + 22915, 1);
			misc::set_global(262145 + 22916, 1);
		}

		void features::recovery::unlock_raygun_paint()
		{
			misc::set_global(101851, 90);
		}

		void features::recovery::unlock_character_stats()
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STAM"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_SHO"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STRN"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STL"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_FLY"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_DRIV"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SCRIPT_INCREASE_LUNG"), 100, 1);

			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STAM"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_SHO"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STRN"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STL"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_FLY"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_DRIV"), 100, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SCRIPT_INCREASE_LUNG"), 100, 1);
		}

		void features::recovery::unlock_weapon_colors()
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MOLOTOV_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CMBTPISTOL_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_PISTOL50_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_APPISTOL_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MICROSMG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SMG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ASLTSMG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ASLTRIFLE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CRBNRIFLE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ADVRIFLE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ASLTMG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_PUMP_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SAWNOFF_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_BULLPUP_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ASLTSHTGN_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SNIPERRFL_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GRNLAUNCH_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_RPG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MINIGUNS_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_GRENADE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_SMKGRENADE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_STKYBMB_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_MOLOTOV_ENEMY_KILLS"), 600, 1);

			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MOLOTOV_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CMBTPISTOL_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_PISTOL50_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_APPISTOL_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MICROSMG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SMG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ASLTSMG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ASLTRIFLE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_CRBNRIFLE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ADVRIFLE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ASLTMG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_PUMP_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SAWNOFF_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_BULLPUP_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ASLTSHTGN_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SNIPERRFL_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GRNLAUNCH_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_RPG_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MINIGUNS_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_GRENADE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_SMKGRENADE_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_STKYBMB_ENEMY_KILLS"), 600, 1);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_MOLOTOV_ENEMY_KILLS"), 600, 1);
		}

		void features::recovery::change_kd(const int choice, const int kills, const int deaths)
		{
			switch (choice) {
			case 1:
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_KILLS_PLAYERS"), kills, 1);
				break;
			case 2:
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MPPLY_DEATHS_PLAYER"), deaths, 1);
				break;
			default:
				break;
			}
		}

		void cayo_stat(char* stat, const int id, const int save)
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY(stat), id, save);
		}

		void features::recovery::setup_cayo(const int choice)
		{
			static int characterid = 0;

			switch (choice) {
			case 1: { // Solo
				static int solo_cayo_array_ids[31] = { 131071, 63, 63, 5, 3, 3, 3, 5, 2, -1, 0, 0, 0, 0, 0, 0, -1, -1, -1, 126823, 0, 0, 0, 0, 0, 0, -1, -1, -1, 65535, 10 };
				static char* solo_cayo_array_stats[31] = { "MPx_H4CNF_BS_GEN", "MPx_H4CNF_BS_ENTR", "MPx_H4CNF_BS_ABIL", "MPx_H4CNF_WEAPONS", "MPx_H4CNF_WEP_DISRP", "MPx_H4CNF_ARM_DISRP", "MPx_H4CNF_HEL_DISRP",
					"MPx_H4CNF_TARGET", "MPx_H4CNF_TROJAN", "MPx_H4CNF_APPROACH", "MPx_H4LOOT_CASH_I", "MPx_H4LOOT_CASH_C", "MPx_H4LOOT_WEED_I", "MPx_H4LOOT_WEED_C", "MPx_H4LOOT_COKE_I", "MPx_H4LOOT_COKE_C", "MPx_H4LOOT_GOLD_I",
					"MPx_H4LOOT_GOLD_C", "MPx_H4LOOT_PAINT", "MPx_H4_PROGRESS", "MPx_H4LOOT_CASH_I_SCOPED", "MPx_H4LOOT_CASH_C_SCOPED", "MPx_H4LOOT_WEED_I_SCOPED", "MPx_H4LOOT_WEED_C_SCOPED", "MPx_H4LOOT_COKE_I_SCOPED", "MPx_H4LOOT_COKE_C_SCOPED",
					"MPx_H4LOOT_GOLD_I_SCOPED" , "MPx_H4LOOT_GOLD_C_SCOPED" , "MPx_H4LOOT_PAINT_SCOPED" , "MPx_H4_MISSIONS" , "MPx_H4_PLAYTHROUGH_STATUS" };

				for (int i = 0; i <= 31; i++)
				{
					std::string currentstr = solo_cayo_array_stats[i];
					currentstr.erase(0, 3);

					currentstr = "MP" + characterid + currentstr;

					cayo_stat((char*)currentstr.c_str(), solo_cayo_array_ids[i], 1);
				}

				characterid = 1;

				for (int i = 0; i <= 31; i++)
				{
					std::string currentstr = solo_cayo_array_stats[i];
					currentstr.erase(0, 3);

					currentstr = "MP" + characterid + currentstr;

					cayo_stat((char*)currentstr.c_str(), solo_cayo_array_ids[i], 1);
				}
			}
			case 2: { // Duo+
				static int duoplus_cayo_array_ids[36] = { 131071, 63, 63, 2, 3, 3, 3, 5, 4424, 5256, 5156, 1, -1, 1185000, 1185000, 0, 0, 0, 0, -1, -1, 0, 0, -1, 126823, 0, 0, 0, 0, -1, -1, 0, 0, -1, 65535, 40000 };
				static char* duoplus_cayo_array_stats[36] = { "MPx_H4CNF_BS_GEN", "MPx_H4CNF_BS_ENTR", "MPx_H4CNF_BS_ABIL", "MPx_H4CNF_WEAPONS", "MPx_H4CNF_WEP_DISRP", "MPx_H4CNF_ARM_DISRP", "MPx_H4CNF_HEL_DISRP",
					"MPx_H4CNF_TARGET", "MP0_H4CNF_BOLTCUT", "MP0_H4CNF_UNIFORM", "MP0_H4CNF_GRAPPEL", "MP0_H4CNF_TROJAN", "MP0_H4CNF_APPROACH", "MP0_H4LOOT_COKE_V", "MP0_H4LOOT_PAINT_V", "MP0_H4LOOT_CASH_I", "MP0_H4LOOT_CASH_C",
					"MP0_H4LOOT_WEED_I", "MP0_H4LOOT_WEED_C", "MP0_H4LOOT_COKE_I", "MP0_H4LOOT_COKE_C", "MP0_H4LOOT_GOLD_I", "MP0_H4LOOT_GOLD_C", "MP0_H4LOOT_PAINT", "MP0_H4_PROGRESS", "MP0_H4LOOT_CASH_I_SCOPED",
					"MP0_H4LOOT_CASH_C_SCOPED" , "MP0_H4LOOT_WEED_I_SCOPED" , "MP0_H4LOOT_WEED_C_SCOPED" , "MP0_H4LOOT_COKE_I_SCOPED" , "MP0_H4LOOT_COKE_C_SCOPED", "MP0_H4LOOT_GOLD_I_SCOPED", "MP0_H4LOOT_GOLD_C_SCOPED",
					"MP0_H4LOOT_PAINT_SCOPED", "MP1_H4_MISSIONS", "MP1_H4_PLAYTHROUGH_STATUS" };

				for (int i = 0; i <= 36; i++)
				{
					std::string currentstr = duoplus_cayo_array_stats[i];
					currentstr.erase(0, 3);

					currentstr = "MP" + characterid + currentstr;

					cayo_stat((char*)currentstr.c_str(), duoplus_cayo_array_ids[i], 1);
				}

				characterid = 1;

				for (int i = 0; i <= 36; i++)
				{
					std::string currentstr = duoplus_cayo_array_stats[i];
					currentstr.erase(0, 3);

					currentstr = "MP" + characterid + currentstr;

					cayo_stat((char*)currentstr.c_str(), duoplus_cayo_array_ids[i], 1);
				}
			}
			}

		}
	}

	namespace weapons {
		bool features::weapons::unlimited_ammo_bool = false;
		bool features::weapons::unlimited_clip_bool = false;
		bool features::weapons::WeaponDeleteGun_bool = false;

		void features::weapons::unlimited_ammo(Ped ped)
		{

			if (features::weapons::unlimited_ammo_bool)
			{
				WEAPON::SET_PED_INFINITE_AMMO(ped, true, 0);
			}
			else
			{
				WEAPON::SET_PED_INFINITE_AMMO(ped, false, 0);
			}

		}

		void features::weapons::unlimited_clip(Ped ped)
		{

			if (features::weapons::unlimited_clip_bool)
			{
				WEAPON::SET_PED_INFINITE_AMMO_CLIP(ped, true);
			}
			else
			{
				WEAPON::SET_PED_INFINITE_AMMO_CLIP(ped, false);
			}

		}

		void features::weapons::give_all_weps(Ped ped)
		{
			int Weapons[] = { WEAPON_ADVANCEDRIFLE, WEAPON_APPISTOL, WEAPON_ASSAULTRIFLE, WEAPON_ASSAULTRIFLE_MK2, WEAPON_ASSAULTSHOTGUN, WEAPON_ASSAULTSMG, WEAPON_BALL, WEAPON_BAT, WEAPON_BATTLEAXE, WEAPON_BOTTLE, WEAPON_BULLPUPRIFLE, WEAPON_BULLPUPRIFLE_MK2, WEAPON_BULLPUPSHOTGUN, WEAPON_BZGAS, WEAPON_CARBINERIFLE, WEAPON_CARBINERIFLE_MK2, WEAPON_COMBATMG, WEAPON_COMBATMG_MK2, WEAPON_COMBATPDW, WEAPON_COMBATPISTOL, WEAPON_COMPACTLAUNCHER, WEAPON_COMPACTRIFLE, WEAPON_CROWBAR, WEAPON_DAGGER, WEAPON_DOUBLEACTION, WEAPON_FIREEXTINGUISHER, WEAPON_FIREWORK, WEAPON_FLARE, WEAPON_FLAREGUN, WEAPON_FLASHLIGHT, WEAPON_GOLFCLUB, WEAPON_GRENADE, WEAPON_GRENADELAUNCHER, WEAPON_GRENADELAUNCHER_SMOKE, WEAPON_GUSENBERG, WEAPON_HAMMER, WEAPON_HATCHET, WEAPON_HEAVYPISTOL, WEAPON_HEAVYSHOTGUN, WEAPON_HEAVYSNIPER, WEAPON_HEAVYSNIPER_MK2, WEAPON_HOMINGLAUNCHER, WEAPON_KNIFE, WEAPON_KNUCKLE, WEAPON_MACHETE, WEAPON_MACHINEPISTOL, WEAPON_MARKSMANPISTOL, WEAPON_MARKSMANRIFLE, WEAPON_MARKSMANRIFLE_MK2, WEAPON_MG, WEAPON_MICROSMG, WEAPON_MINIGUN, WEAPON_MINISMG, WEAPON_MOLOTOV, WEAPON_MUSKET, WEAPON_NIGHTSTICK, WEAPON_PETROLCAN, WEAPON_PIPEBOMB, WEAPON_PISTOL50, WEAPON_PISTOL, WEAPON_PISTOL_MK2, WEAPON_POOLCUE, WEAPON_PROXMINE, WEAPON_PUMPSHOTGUN, WEAPON_PUMPSHOTGUN_MK2, WEAPON_RAILGUN, WEAPON_RAYCARBINE, WEAPON_RAYPISTOL, WEAPON_RAYMINIGUN, WEAPON_REVOLVER, WEAPON_REVOLVER_MK2, WEAPON_RPG, WEAPON_SAWNOFFSHOTGUN, WEAPON_SMG, WEAPON_SMG_MK2, WEAPON_SMOKEGRENADE, WEAPON_SNIPERRIFLE, WEAPON_SNOWBALL, WEAPON_SNSPISTOL, WEAPON_SNSPISTOL_MK2, WEAPON_SPECIALCARBINE, WEAPON_SPECIALCARBINE_MK2, WEAPON_STICKYBOMB, WEAPON_STINGER, WEAPON_STONE_HATCHET, WEAPON_STUNGUN, WEAPON_SWITCHBLADE, WEAPON_UNARMED, WEAPON_VINTAGEPISTOL, WEAPON_WRENCH };
			for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
				if (!WEAPON::HAS_PED_GOT_WEAPON(ped, Weapons[i], false))
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, Weapons[i], 9999, 1);
			}
		}

		void features::weapons::give_all_mk2(Ped ped) {
			int Weapons[] = { WEAPON_ASSAULTRIFLE_MK2, WEAPON_BULLPUPRIFLE_MK2, WEAPON_CARBINERIFLE_MK2, WEAPON_COMBATMG_MK2, WEAPON_HEAVYSNIPER_MK2, WEAPON_MARKSMANRIFLE_MK2, WEAPON_PISTOL_MK2, WEAPON_PUMPSHOTGUN_MK2, WEAPON_REVOLVER_MK2, WEAPON_SMG_MK2, WEAPON_SNSPISTOL_MK2, WEAPON_SPECIALCARBINE_MK2 };
			for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, Weapons[i], 9999, 1);
			}
		}

		void features::weapons::give_stun_gun(Ped ped) {
			int Weapons[] = { 0x3656C8C1, 911657153 };
			for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, Weapons[i], 9999, 1);
			}
		}

		void features::weapons::give_scanner(Ped ped) {
			int Weapons[] = { 0xFDBADCED, 4256881901 };
			for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, Weapons[i], 9999, 1);
			}
		}

		void features::weapons::refill_ammo(Ped ped)
		{
			Hash weaponhash;
			WEAPON::GET_CURRENT_PED_WEAPON(ped, &weaponhash, 1);
			WEAPON::SET_PED_AMMO(ped, weaponhash, 9999, 0);
		}

		void features::weapons::WeaponDeleteGun() {
			if (WeaponDeleteGun_bool)
			{
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && PED::IS_PED_SHOOTING_IN_AREA)
				{
					Vector3 iCoord;
					if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &iCoord))
					{
						MISC::_CLEAR_AREA_OF_EVERYTHING(iCoord.x, iCoord.y, iCoord.z, 2, 0, 0, 0, 0);
					}
				}
			}
		}
	}

	namespace vehicles {
		bool features::vehicles::enableflares_bool = false;


		void features::vehicles::max_vehicle()
		{
			Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
			for (int i = 0; i < 50; i++)
			{
				VEHICLE::SET_VEHICLE_MOD(vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) - 1, false);
			}
			features::vehicles::set_plate(vehicle, "Wurst");
		}


		void features::vehicles::enableflares()
		{
			if (enableflares_bool)
			{
				if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
				{
					Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
					Hash hash = MISC::GET_HASH_KEY("weapon_flaregun");
					STREAMING::REQUEST_MODEL(hash);
					WEAPON::REQUEST_WEAPON_ASSET(hash, 31, 26);
					if (PAD::IS_CONTROL_JUST_RELEASED(0, 355))
					{

						auto offset1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, -6.0, -4.0, -0.2);
						auto offset2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, -3.0, -4.0, -0.2);
						auto offset3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, 6.0, -4.0, -0.2);
						auto offset4 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(vehicle, 3.0, -4.0, -0.2);
						AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "flares_released", vehicle, "DLC_SM_Countermeasures_Sounds", true, 0);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset1.x, offset1.y, offset1.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset2.x, offset2.y, offset2.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset3.x, offset3.y, offset3.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY_NEW(pos.x, pos.y, pos.z, offset4.x, offset4.y, offset4.z, 0, true, hash, PLAYER::PLAYER_PED_ID(), true, true, -4.f, vehicle, false, false, false, true, true, false);
					}
				}
			}
		}

		void features::vehicles::set_plate(Vehicle veh, char* platename)
		{
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, platename);
		}
	}

	namespace objects {
		void features::objects::request_control_id(int network_id)
		{
			int tick = 0;

			while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(network_id) && tick <= 25)
			{
				NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(network_id);
				tick++;
			}
		}

		void features::objects::request_control_ent(Entity entity)
		{
			int tick = 0;
			while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
			{
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);

				tick++;
			}
			if (g_pointers->m_is_session_started)
			{
				int network_id = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
				features::objects::request_control_id(network_id);
				NETWORK::SET_NETWORK_ID_CAN_MIGRATE(network_id, 1);
			}
		}
		void features::objects::attach_obj_to_player(char* object, Player player)
		{
			Entity t = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
			Vector3 pos = { 0.f, -.26f, .1f };
			Vector3 rot = { 0.f, 0.f, 0.f };
			Hash hash = MISC::GET_HASH_KEY(object);
			const auto ped_position = ENTITY::GET_ENTITY_COORDS(t, true);
			Vector3 empty;
			auto objectToAttach = OBJECT::CREATE_OBJECT(hash, ped_position.x, ped_position.y, ped_position.z, true, false, false);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(objectToAttach, t, SKEL_Spine0, x_pos, y_pos, z_pos, x_rot, y_rot, z_rot, 1, 1, false, true, 2, 1);
			return;
		}

		void features::objects::spawnPed(const char* pedName, Player player)
		{
			g_fiber_pool->queue_job([=] {
				Entity t = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
				const auto ped_position = ENTITY::GET_ENTITY_COORDS(t, true);

				Hash pedHash = MISC::GET_HASH_KEY(pedName);
				if (STREAMING::IS_MODEL_VALID(pedHash)
					&& STREAMING::IS_MODEL_IN_CDIMAGE(pedHash))
				{
					STREAMING::REQUEST_MODEL(pedHash);
					while (!STREAMING::HAS_MODEL_LOADED(pedHash))
						script::get_current()->yield();
					int pedtype = PED::GET_PED_TYPE(pedHash);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
					Ped ped = PED::CREATE_PED(pedtype, pedHash, ped_position.x, ped_position.y, ped_position.z, 1.f, true, true);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

				}
				else
					LOG(INFO) << "Ped not found";
			});
		}


		void features::objects::spawnGriferPed(const char* pedName, Player player)
		{
			g_fiber_pool->queue_job([=] {
				Entity t = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
				const auto ped_position = ENTITY::GET_ENTITY_COORDS(t, true);

				Hash pedHash = MISC::GET_HASH_KEY(pedName);
				if (STREAMING::IS_MODEL_VALID(pedHash)
					&& STREAMING::IS_MODEL_IN_CDIMAGE(pedHash))
				{
					STREAMING::REQUEST_MODEL(pedHash);
					while (!STREAMING::HAS_MODEL_LOADED(pedHash))
						script::get_current()->yield();
					static const Hash playerGroup = MISC::GET_HASH_KEY("PLAYER");
					static const Hash civGroup = MISC::GET_HASH_KEY("CIVMALE");
					static const Hash femCivGroup = MISC::GET_HASH_KEY("CIVFEMALE");

					Hash relationshipGroup;
					PED::ADD_RELATIONSHIP_GROUP("_HOSTILE_JESUS", &relationshipGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

					int pedtype = PED::GET_PED_TYPE(pedHash);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
					Ped ped = PED::CREATE_PED(pedtype, pedHash, ped_position.x, ped_position.y, ped_position.z, 1.f, true, true);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

					PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
					PED::SET_PED_HEARING_RANGE(ped, 9999.f);
					PED::SET_PED_CONFIG_FLAG(ped, 281, true);

					ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

					PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
					PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
					PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
					PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

					WEAPON::GIVE_WEAPON_TO_PED(ped, MISC::GET_HASH_KEY("WEAPON_RAILGUN"), 9999, true, true);
					TASK::TASK_COMBAT_PED(ped, t, 0, 16);

					PED::SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);
				}
				else
					LOG(INFO) << "Ped not found";
			});
		}

		void features::objects::spawnExtimeGriferPed(const char* pedName, Player player)
		{
			g_fiber_pool->queue_job([=] {
				Entity t = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
				const auto pos = ENTITY::GET_ENTITY_COORDS(t, true);
				Hash hash = MISC::GET_HASH_KEY("OPPRESSOR2");
				while (!STREAMING::HAS_MODEL_LOADED(hash))
				{
					STREAMING::REQUEST_MODEL(hash);
					script::get_current()->yield();
				}

				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
				Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, 0.f, TRUE, FALSE, FALSE);
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574; //By writing the "old" bypass to the function, running CREATE_VEHICLE, then restoring it, the anti-cheat does not have enough time to catch the function in a dirty state.
				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

				for (int i = 0; i < 50; i++)
				{
					int max = VEHICLE::GET_NUM_VEHICLE_MODS(veh, i);
					VEHICLE::SET_VEHICLE_MOD(veh, i, max > 0 ? max - 1 : 0, false);
				}

				ENTITY::SET_ENTITY_PROOFS(veh, false, true, true, false, false, false, false, false);
				script::get_current()->yield();
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
				if (*g_pointers->m_is_session_started)
				{
					DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
					ENTITY::_SET_ENTITY_SOMETHING(veh, TRUE); //True means it can be deleted by the engine when switching lobbies/missions/etc, false means the script is expected to clean it up.
					auto networkId = NETWORK::VEH_TO_NET(veh);
					if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
						NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
					VEHICLE::SET_VEHICLE_IS_STOLEN(veh, FALSE);
				}
				Hash pedHash = MISC::GET_HASH_KEY(pedName);
				if (STREAMING::IS_MODEL_VALID(pedHash)
					&& STREAMING::IS_MODEL_IN_CDIMAGE(pedHash))
				{
					STREAMING::REQUEST_MODEL(pedHash);
					while (!STREAMING::HAS_MODEL_LOADED(pedHash))
						script::get_current()->yield();
					static const Hash playerGroup = MISC::GET_HASH_KEY("PLAYER");
					static const Hash civGroup = MISC::GET_HASH_KEY("CIVMALE");
					static const Hash femCivGroup = MISC::GET_HASH_KEY("CIVFEMALE");

					Hash relationshipGroup;
					PED::ADD_RELATIONSHIP_GROUP("_HOSTILE_JESUS", &relationshipGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

					int pedtype = PED::GET_PED_TYPE(pedHash);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
					Ped ped = PED::CREATE_PED_INSIDE_VEHICLE(veh, pedtype, pedHash, -1, true, true);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

					PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
					PED::SET_PED_HEARING_RANGE(ped, 9999.f);
					PED::SET_PED_CONFIG_FLAG(ped, 281, true);

					ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

					PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
					PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
					PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

					PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 1);
					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
					PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
					PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

					WEAPON::GIVE_WEAPON_TO_PED(ped, MISC::GET_HASH_KEY("WEAPON_RAILGUN"), 9999, true, true);
					TASK::TASK_COMBAT_PED(ped, t, 0, 16);

					PED::SET_PED_FIRING_PATTERN(ped, 0x914E786F);
				}
			});

		}

		void features::objects::spawnGriferJet(const char* pedName, const char* vehicle, Player player, bool isUFO)
		{
			g_fiber_pool->queue_job([=] {
				Entity t = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
				const auto pos = ENTITY::GET_ENTITY_COORDS(t, true);
				Hash hash = MISC::GET_HASH_KEY(vehicle);
				while (!STREAMING::HAS_MODEL_LOADED(hash))
				{
					STREAMING::REQUEST_MODEL(hash);
					script::get_current()->yield();
				}

				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
				Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z + 250, 0.f, TRUE, FALSE, FALSE);
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574; //By writing the "old" bypass to the function, running CREATE_VEHICLE, then restoring it, the anti-cheat does not have enough time to catch the function in a dirty state.
				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
				VEHICLE::CONTROL_LANDING_GEAR(veh, 3);

				script::get_current()->yield();
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
				if (*g_pointers->m_is_session_started)
				{
					DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
					ENTITY::_SET_ENTITY_SOMETHING(veh, TRUE); //True means it can be deleted by the engine when switching lobbies/missions/etc, false means the script is expected to clean it up.
					auto networkId = NETWORK::VEH_TO_NET(veh);
					if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
						NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
					VEHICLE::SET_VEHICLE_IS_STOLEN(veh, FALSE);
				}
				Hash pedHash = MISC::GET_HASH_KEY(pedName);
				if (STREAMING::IS_MODEL_VALID(pedHash)
					&& STREAMING::IS_MODEL_IN_CDIMAGE(pedHash))
				{
					STREAMING::REQUEST_MODEL(pedHash);
					while (!STREAMING::HAS_MODEL_LOADED(pedHash))
						script::get_current()->yield();

					int pedtype = PED::GET_PED_TYPE(pedHash);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
					Ped ped = PED::CREATE_PED_INSIDE_VEHICLE(veh, pedtype, pedHash, -1, true, true);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

					TASK::TASK_PLANE_MISSION(ped, veh, 0, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(online::selectedPlayer), 0, 0, 0, 6, 0.0, 0.0, 0.0, 2500.0, -1500.0, 0);
					Blip blip = HUD::ADD_BLIP_FOR_ENTITY(ped);

					PED::SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);

					if (isUFO)
					{
						Hash hash = MISC::GET_HASH_KEY("p_spinning_anus_s");
						const auto ped_position = ENTITY::GET_ENTITY_COORDS(ped, true);
						Vector3 empty;
						auto objectToAttach = OBJECT::CREATE_OBJECT(hash, ped_position.x, ped_position.y, ped_position.z, true, false, false);
						ENTITY::ATTACH_ENTITY_TO_ENTITY(objectToAttach, ped, SKEL_Spine0, x_pos, y_pos, z_pos, x_rot, y_rot, z_rot, 1, 1, false, true, 2, 1);
						return;
					}
				}
			});

		}

		void features::objects::spawnGriferVehicle(const char* pedName, const char* vehName, Player player)
		{
			g_fiber_pool->queue_job([=] {
				Entity t = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
				const auto pos = ENTITY::GET_ENTITY_COORDS(t, true);
				Hash hash = MISC::GET_HASH_KEY(vehName);
				while (!STREAMING::HAS_MODEL_LOADED(hash))
				{
					STREAMING::REQUEST_MODEL(hash);
					script::get_current()->yield();
				}


				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
				Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z + 30, 0.f, TRUE, FALSE, FALSE);
				*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574; //By writing the "old" bypass to the function, running CREATE_VEHICLE, then restoring it, the anti-cheat does not have enough time to catch the function in a dirty state.
				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
				VEHICLE::CONTROL_LANDING_GEAR(veh, 3);

				script::get_current()->yield();
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
				if (*g_pointers->m_is_session_started)
				{
					DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
					ENTITY::_SET_ENTITY_SOMETHING(veh, TRUE); //True means it can be deleted by the engine when switching lobbies/missions/etc, false means the script is expected to clean it up.
					auto networkId = NETWORK::VEH_TO_NET(veh);
					if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
						NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
					VEHICLE::SET_VEHICLE_IS_STOLEN(veh, FALSE);
				}
				Hash pedHash = MISC::GET_HASH_KEY(pedName);
				if (STREAMING::IS_MODEL_VALID(pedHash)
					&& STREAMING::IS_MODEL_IN_CDIMAGE(pedHash))
				{
					static const Hash playerGroup = MISC::GET_HASH_KEY("PLAYER");
					static const Hash civGroup = MISC::GET_HASH_KEY("CIVMALE");
					static const Hash femCivGroup = MISC::GET_HASH_KEY("CIVFEMALE");

					Hash relationshipGroup;
					PED::ADD_RELATIONSHIP_GROUP("_HOSTILE_JESUS", &relationshipGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
					PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

					STREAMING::REQUEST_MODEL(pedHash);
					while (!STREAMING::HAS_MODEL_LOADED(pedHash))
						script::get_current()->yield();

					int pedtype = PED::GET_PED_TYPE(pedHash);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
					Ped ped = PED::CREATE_PED_INSIDE_VEHICLE(veh, pedtype, pedHash, -1, true, true);
					*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

					Blip blip = HUD::ADD_BLIP_FOR_ENTITY(ped);

					PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
					PED::SET_PED_HEARING_RANGE(ped, 9999.f);
					PED::SET_PED_CONFIG_FLAG(ped, 281, true);

					ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

					PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
					PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
					PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

					PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 1);
					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
					PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
					PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

					WEAPON::GIVE_WEAPON_TO_PED(ped, MISC::GET_HASH_KEY("WEAPON_RAILGUN"), 9999, true, true);
					TASK::TASK_COMBAT_PED(ped, t, 0, 16);

					PED::SET_PED_FIRING_PATTERN(ped, 0xC6EE6B4C);



				}
			});

		}


		void features::objects::detach_all(Player player)
		{
			static char* objects[] = { "prop_bskball_01", "PROP_MP_RAMP_03", "PROP_MP_RAMP_02", "PROP_MP_RAMP_01", "PROP_JETSKI_RAMP_01", "PROP_WATER_RAMP_03", "PROP_VEND_SNAK_01", "PROP_TRI_START_BANNER", "PROP_TRI_FINISH_BANNER", "PROP_TEMP_BLOCK_BLOCKER", "PROP_SLUICEGATEL", "PROP_SKIP_08A", "PROP_SAM_01", "PROP_RUB_CONT_01B", "PROP_ROADCONE01A", "PROP_MP_ARROW_BARRIER_01", "PROP_HOTEL_CLOCK_01", "PROP_LIFEBLURB_02", "PROP_COFFIN_02B", "PROP_MP_NUM_1", "PROP_MP_NUM_2", "PROP_MP_NUM_3", "PROP_MP_NUM_4", "PROP_MP_NUM_5", "PROP_MP_NUM_6", "PROP_MP_NUM_7", "PROP_MP_NUM_8", "PROP_MP_NUM_9", "prop_xmas_tree_int", "prop_bumper_car_01", "prop_beer_neon_01", "prop_space_rifle", "prop_dummy_01", "prop_rub_trolley01a", "prop_wheelchair_01_s", "PROP_CS_KATANA_01", "PROP_CS_DILDO_01", "prop_armchair_01", "prop_bin_04a", "prop_chair_01a", "prop_dog_cage_01", "prop_dummy_plane", "prop_golf_bag_01", "prop_arcade_01", "hei_prop_heist_emp", "prop_alien_egg_01", "prop_air_towbar_01", "hei_prop_heist_tug", "prop_air_luggtrolley", "PROP_CUP_SAUCER_01", "prop_wheelchair_01", "prop_ld_toilet_01", "prop_acc_guitar_01", "prop_bank_vaultdoor", "p_v_43_safe_s", "p_spinning_anus_s", "prop_can_canoe", "prop_air_woodsteps", "Prop_weed_01", "prop_a_trailer_door_01", "prop_apple_box_01", "prop_air_fueltrail1", "prop_barrel_02a", "prop_barrel_float_1", "prop_barrier_wat_03b", "prop_air_fueltrail2", "prop_air_propeller01", "prop_windmill_01", "prop_Ld_ferris_wheel", "p_tram_crash_s", "p_oil_slick_01", "p_ld_stinger_s", "p_ld_soc_ball_01", "prop_juicestand", "p_oil_pjack_01_s", "prop_barbell_01", "prop_barbell_100kg", "p_parachute1_s", "p_cablecar_s", "prop_beach_fire", "prop_lev_des_barge_02", "prop_lev_des_barge_01", "prop_a_base_bars_01", "prop_beach_bars_01", "prop_air_bigradar", "prop_weed_pallet", "prop_artifact_01", "prop_attache_case_01", "prop_large_gold", "prop_roller_car_01", "prop_water_corpse_01", "prop_water_corpse_02", "prop_dummy_01", "prop_atm_01", "hei_prop_carrier_docklight_01", "hei_prop_carrier_liferafts", "hei_prop_carrier_ord_03", "hei_prop_carrier_defense_02", "hei_prop_carrier_defense_01", "hei_prop_carrier_radar_1", "hei_prop_carrier_radar_2", "hei_prop_hei_bust_01", "hei_prop_wall_alarm_on", "hei_prop_wall_light_10a_cr", "prop_afsign_amun", "prop_afsign_vbike", "prop_aircon_l_01", "prop_aircon_l_02", "prop_aircon_l_03", "prop_aircon_l_04", "prop_airhockey_01", "prop_air_bagloader", "prop_air_blastfence_01", "prop_air_blastfence_02", "prop_air_cargo_01a", "prop_air_chock_01", "prop_air_chock_03", "prop_air_gasbogey_01", "prop_air_generator_03", "prop_air_stair_02", "prop_amb_40oz_02", "prop_amb_40oz_03", "prop_amb_beer_bottle", "prop_amb_donut", "prop_amb_handbag_01", "prop_amp_01", "prop_anim_cash_pile_02", "prop_asteroid_01", "prop_arm_wrestle_01", "prop_ballistic_shield", "prop_bank_shutter", "prop_barier_conc_02b", "prop_barier_conc_05a", "prop_barrel_01a", "prop_bar_stool_01", "prop_basejump_target_01" };

			for (int i = 0; i < 5; i++) {
				Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 1);
				MISC::CLEAR_AREA_OF_PEDS(coords.x, coords.y, coords.z, 2, 0);
				MISC::CLEAR_AREA_OF_OBJECTS(coords.x, coords.y, coords.z, 2, 0);
				MISC::CLEAR_AREA_OF_VEHICLES(coords.x, coords.y, coords.z, 2, 0, 0, 0, 0, 0, 0);
				for (int i = 0; i < 136; i++) {
					Object obj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(coords.x, coords.y, coords.z, 4.0, MISC::GET_HASH_KEY(objects[i]), 0, 0, 1);

					if (ENTITY::DOES_ENTITY_EXIST(obj) && ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(obj, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player))) {
						features::objects::request_control_ent(obj);
						int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(obj);
						NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
						features::objects::request_control_id(netID);
						ENTITY::DETACH_ENTITY(obj, 1, 1);
					}
				}
			}
		}
	}

	namespace self {
		bool features::self::super_jump_bool = false;
		bool features::self::no_clip_bool = false;
		bool features::self::god_mode_bool = false;
		bool features::self::never_wanted_bool = false;
		bool features::self::buff_hp_regen_bool = false;
		bool features::self::light_test_bool = false;
		bool features::self::no_spam_bool = true;

		Vector3 features::self::get_blip_marker()
		{
			static Vector3 zero;
			Vector3 coords;

			bool blipFound = false;
			// search for marker blip
			int blipIterator = HUD::_GET_WAYPOINT_BLIP_SPRITE();
			for (Blip i = HUD::GET_FIRST_BLIP_INFO_ID(blipIterator); HUD::DOES_BLIP_EXIST(i) != 0; i = HUD::GET_NEXT_BLIP_INFO_ID(blipIterator))
			{
				if (HUD::GET_BLIP_INFO_ID_TYPE(i) == 4)
				{
					coords = HUD::GET_BLIP_INFO_ID_COORD(i);
					blipFound = true;
					break;
				}
			}
			if (blipFound)
			{
				return coords;
			}

			return zero;
		}

		Vector3 features::self::get_mission_objective()
		{
			Vector3 blipCoords;

			for (Blip x = HUD::GET_FIRST_BLIP_INFO_ID(1); HUD::DOES_BLIP_EXIST(x) != 0; x = HUD::GET_NEXT_BLIP_INFO_ID(1))
			{
				if (HUD::GET_BLIP_COLOUR(x) == 0x42 || HUD::GET_BLIP_COLOUR(x) == 0x5 || HUD::GET_BLIP_COLOUR(x) == 0x3C || HUD::GET_BLIP_COLOUR(x) == 0x2)
				{
					blipCoords = HUD::GET_BLIP_COORDS(x);
					break;
				}
			}
			for (Blip y = HUD::GET_FIRST_BLIP_INFO_ID(38); HUD::DOES_BLIP_EXIST(y) != 0; y = HUD::GET_NEXT_BLIP_INFO_ID(38))
			{
				if (HUD::GET_BLIP_COLOUR(y) == 0x0)
				{
					blipCoords = HUD::GET_BLIP_COORDS(y);
					break;
				}
			}
			for (Blip z = HUD::GET_FIRST_BLIP_INFO_ID(431); HUD::DOES_BLIP_EXIST(z) != 0; z = HUD::GET_NEXT_BLIP_INFO_ID(431))
			{
				if (HUD::GET_BLIP_COLOUR(z) == 0x3C)
				{
					blipCoords = HUD::GET_BLIP_COORDS(z);
					break;
				}
			}

			return blipCoords;
		}

		void features::self::tp_to_destination(Entity e, Vector3 coords)
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
		}

		void features::self::tp_to_waypoint()
		{
			Vector3 coords = get_blip_marker();

			if (coords.x == 0 && coords.y == 0)
			{
				return;
			}

			// get entity to teleport
			Entity entity = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_IN_ANY_VEHICLE(entity, 0))
			{
				entity = PED::GET_VEHICLE_PED_IS_USING(entity);
			}
			bool worldLevel = false;
			static float groundCheckHeight[] =
			{ 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
			for (int i = 0; i < 800 / sizeof(float); i++)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);

				if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, 0, 0))
				{
					worldLevel = true;
					coords.z += 3.0;
					break;
				}
			}
			if (!worldLevel)
			{
				coords.z = 1000.0;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
			}
			features::self::tp_to_destination(entity, coords);

		}

		void features::self::tp_to_objective()
		{
			Vector3 coords = features::self::get_mission_objective();

			if (coords.x == 0 && coords.y == 0)
			{
				return;
			}

			// get entity to teleport
			Entity entity = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_IN_ANY_VEHICLE(entity, 0))
			{
				entity = PED::GET_VEHICLE_PED_IS_USING(entity);
			}
			bool worldLevel = false;
			static float groundCheckHeight[] =
			{ 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
			for (int i = 0; i < 800 / sizeof(float); i++)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entity, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);

				if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, 0, 0))
				{
					worldLevel = true;
					coords.z += 3.0;
					break;
				}
			}
			if (!worldLevel)
			{
				coords.z = 1000.0;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
			}

			features::self::tp_to_destination(PLAYER::PLAYER_PED_ID(), coords);
		}
	}

	void features::self::god_mode(Player player)
	{
		if (features::self::god_mode_bool)
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), features::self::god_mode_bool);
	}

	void features::self::never_wanted(Player player)
	{

		if (features::self::never_wanted_bool)
		{
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
			PLAYER::SET_MAX_WANTED_LEVEL(0);
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, true);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, true);
		}
		else
		{
			PLAYER::SET_MAX_WANTED_LEVEL(5);
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, false);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, false);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, false);
		}

	}

	void features::self::buff_hp_regen(Player player)
	{
		if (features::self::buff_hp_regen_bool)
		{
			PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(player, 1.5);
		}
		else
		{
			PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(player, 1.0);
		}
	}

	void features::self::super_jump()
	{
		if (features::self::super_jump_bool)
		{
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_PED_ID());
		}
	}

	void features::self::light_test()
	{
		if (features::self::light_test_bool)
		{
			auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			GRAPHICS::DRAW_LIGHT_WITH_RANGE(pos.x, pos.y, pos.z, (int)(light_color[0] * 255), (int)(light_color[1] * 255), (int)(light_color[2] * 255), 4, 1);
		}
	}

	void features::self::no_spam()
	{
		if (features::self::no_spam_bool)
		{
			HUD::HIDE_HELP_TEXT_THIS_FRAME();
		}
	}

	void features::self::no_clip()
	{

		if (features::self::no_clip_bool) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, false);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			if (GetAsyncKeyState(0x57) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 268)) {
				float fivef = 2.5f;
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				float xVec = fivef * sin(features::basic::deg_to_rad(heading)) * -1.0f;
				float yVec = fivef * cos(features::basic::deg_to_rad(heading));
				ENTITY::SET_ENTITY_HEADING(playerPed, heading);

				pos.x -= xVec, pos.y -= yVec;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			}
			if (GetAsyncKeyState(0x53) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 269)) {
				float fivef = 2.5f;
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				float xVec = fivef * sin(features::basic::deg_to_rad(heading)) * -1.0f;
				float yVec = fivef * cos(features::basic::deg_to_rad(heading));
				ENTITY::SET_ENTITY_HEADING(playerPed, heading);

				pos.x += xVec, pos.y += yVec;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			}
			if (GetAsyncKeyState(0x41) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 266)) {
				float fivef = .5f;
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				ENTITY::SET_ENTITY_HEADING(playerPed, heading + 3.5f);
			}
			if (GetAsyncKeyState(0x44) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 271)) {
				float fivef = .5f;
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				ENTITY::SET_ENTITY_HEADING(playerPed, heading - 3.5f);
			}
			if (GetAsyncKeyState(VK_LBUTTON)) {
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				ENTITY::SET_ENTITY_HEADING(playerPed, heading);

				pos.z += 2.5;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			}
			if (GetAsyncKeyState(VK_RBUTTON)) {
				float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
				ENTITY::SET_ENTITY_HEADING(playerPed, heading);

				pos.z -= 2.5;
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
			}
		}
	}

	void features::self::give_needs(Ped ped)
	{
		ENTITY::SET_ENTITY_HEALTH(ped, 328, 0);
		PED::ADD_ARMOUR_TO_PED(ped, 50);
		PED::CLEAR_PED_BLOOD_DAMAGE(ped);
	}

	void features::self::player_alpha(int alpha)
	{
		ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), alpha, 0);
	}

	void features::self::suicide(Ped ped)
	{
		ENTITY::SET_ENTITY_HEALTH(ped, 0, 0);
	}

	void features::self::skip_cutscene()
	{
		if (CUTSCENE::HAS_CUTSCENE_LOADED()) {
			CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
		}
		else
		{
			return;
		}
	}

	void features::run_tick()
	{
		if (GetAsyncKeyState(VK_F12)) {
			features::self::tp_to_waypoint();
		}


		features::vehicles::enableflares();
		features::weapons::WeaponDeleteGun();
		features::self::god_mode(PLAYER::PLAYER_ID());
		features::self::super_jump();
		features::self::light_test();
		features::self::no_spam();
		features::self::no_clip();
		features::online::block_afk();
		features::online::Freeze_player();

		UpdatePlayer(); // Player List
	}



	void features::script_func()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				run_tick();
			}
				EXCEPT_CLAUSE
				script::get_current()->yield();
		}
	}
}
