#include "common.hpp"
#include "base_tab.h"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "ImGuiBitfield.h"
#include "features.hpp"
#include "script_global.hpp"

namespace big
{
	void base_tab::render_players_tab()
	{

		if (ImGui::BeginTabItem("Players"))
		{
			if (*g_pointers->m_is_session_started)
			{
				features::online::show_players();

				features::online::draw_extra_info();

				ImGui::NewLine();
			}

			ImGui::EndTabItem();
		}
	}

	void base_tab::render_outfit_tab()
	{
		if (ImGui::BeginTabItem("Outfit Editor"))
		{
			static int selected_component = 0;
			const char* const player_components[]
			{
				"Head",
				"Masks",
				"Hair Styles",
				"Torsos",
				"Legs",
				"Bags and Parachutes",
				"Shoes",
				"Accessories",
				"Undershirts",
				"Body Armors",
				"Decals",
				"Tops"
			};
			ImGui::Combo("##player_components", &selected_component, player_components, IM_ARRAYSIZE(player_components));
			static int drawableId_int = 0;
			static int textureId_int = 0;
			ImGui::InputInt("drawableId", &drawableId_int);
			ImGui::InputInt("textureId", &textureId_int);
			if (ImGui::Button("set"))
			{
				features::online::change_clothe(selected_component, drawableId_int, textureId_int, 0);
			}
		}
	}

	void base_tab::render_base_tab()
	{
		if (ImGui::BeginTabItem("Test"))
		{
			if (ImGui::TreeNode("test stuff"))
			{
				//
				if (ImGui::Button("END_CREDITS_SAVE_MICHAEL_TREVOR"))
				{
					g_fiber_pool->queue_job([]
					{
						AUDIO::SET_CUSTOM_RADIO_TRACK_LIST("RADIO_01_CLASS_ROCK", "END_CREDITS_SAVE_MICHAEL_TREVOR", 1);
						AUDIO::SET_RADIO_TRACK("RADIO_01_CLASS_ROCK", "END_CREDITS_SAVE_MICHAEL_TREVOR");
					});
				}
				if (ImGui::Button("Remote_Ring"))
				{
					g_fiber_pool->queue_job([]
					{
						AUDIO::PLAY_PED_RINGTONE("Remote_Ring", PLAYER::PLAYER_PED_ID(), 1);
					});
				}
				ImGui::SameLine();
				if (ImGui::Button("EPSILONISM_04_SOUNDSET"))
				{
					g_fiber_pool->queue_job([]
					{
						AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "IDLE_BEEP", PLAYER::PLAYER_PED_ID(), "EPSILONISM_04_SOUNDSET", true, 0);
					});
				}
				ImGui::SameLine();
				if (ImGui::Button("submarine_sunk"))
				{
					g_fiber_pool->queue_job([]
					{
						AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "sonar_pulse", PLAYER::PLAYER_PED_ID(), "docks_heist_finale_2a_sounds", true, 0);
					});
				}
				ImGui::SameLine();
				if (ImGui::Button("MissionFailedSounds"))
				{
					g_fiber_pool->queue_job([]
					{
						if (AUDIO::REQUEST_SCRIPT_AUDIO_BANK("generic_failed", false, -1))
						AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "ScreenFlash", PLAYER::PLAYER_PED_ID(), "MissionFailedSounds", true, 0);
					});
				}
				if (ImGui::Button("TextHit"))
				{
					g_fiber_pool->queue_job([]
					{
						AUDIO::TRIGGER_MUSIC_EVENT("EXL3_MISSION_FAIL");
					});
				}
				ImGui::SameLine();
				if (ImGui::Button("Security_Door_Bomb_Bleeps"))
				{
					g_fiber_pool->queue_job([]
					{
						AUDIO::REQUEST_SCRIPT_AUDIO_BANK("Prologue_Sounds", 1, 0);
						AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "Security_Door_Bomb_Bleeps", PLAYER::PLAYER_PED_ID(), "Prologue_Sounds", true, 0);
					});
				}
				ImGui::TreePop();
			}
			/*
			const char* const demo_combo[]
			{
				"One",
				"Two",
				"Three"
			};
			const double min = 0., max = 10.;

			if (ImGui::TreeNode("Capture/Logging"))
			{
				//If you want to add a new option, you have to first declare it in settings.h's default_options, otherwise, this code will crash when trying to access an option that does not exist in memory.
			
					g_settings.save();
				if (ImGui::SliderInt("Int", (PINT)g_settings.options["demo int"].get<int64_t*>(), 0, 10))
					g_settings.save();
				if (ImGui::SliderScalar("Double", ImGuiDataType_Double, g_settings.options["demo double"].get<double*>(), &min, &max)) //JSON does not describe rational numbers as integer/float/double/etc types, it is just "number". See: https://nlohmann.github.io/json/features/types/
					g_settings.save();
				if (ImGui::Combo("Combo", (PINT)g_settings.options["demo combo"].get<int64_t*>(), demo_combo, sizeof(demo_combo) / sizeof(*demo_combo)))
					g_settings.save();
				ImGui::TreePop();
			}
			*/

			if (ImGui::TreeNode("Player"))
			{
				static int wanted_level = 0;
				if (ImGui::SliderInt("wanted level", &wanted_level, 0, 5))
				{
					g_fiber_pool->queue_job([]
					{
						//LOG(INFO) << wanted_level;
						PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), wanted_level, false);
					});
				}

				static bool neverwanted = false;

				if (ImGui::Checkbox("never wanted", &features::self::never_wanted_bool))

				ImGui::SameLine();

				if (ImGui::Button("ragdoll"))
				{
					g_fiber_pool->queue_job([]
					{
						TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
						PED::SET_PED_TO_RAGDOLL(PLAYER::PLAYER_PED_ID(), 10000, 10000, 0, true, true, false);
					});
				}

				static int selected_seat = 0;
				const char* const vehicle_seats[]
				{
					"Passenger ",
					"Left Rear",
					"RightRear",
				};

				ImGui::Combo("##vehicle_seat", &selected_seat, vehicle_seats, IM_ARRAYSIZE(vehicle_seats));

				ImGui::SameLine();

				if (ImGui::Button("teleport to nearest vehicle"))
				{
					g_fiber_pool->queue_job([]
					{
						LOG(INFO) << selected_seat;
						auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
						PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), VEHICLE::GET_CLOSEST_VEHICLE(pos.x, pos.y, pos.z, 1000, 0, 70), selected_seat);
					});
				}

				if (ImGui::Button("Bribe Authorities "))
				{
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						*script_global(2544210 + 4622).as<int*>() = 5;
					} QUEUE_JOB_END_CLAUSE
				}

				ImGui::SameLine();

				if (ImGui::Button("Go Into Personal Vehicle"))
				{
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						*script_global(2409299).at(8).as<int*>() = 1;
					} QUEUE_JOB_END_CLAUSE
				}

				ImGui::Separator();

				if (ImGui::Button("Max Snacks")) { STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_YUM_SNACKS"), 30, true); STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_HEALTH_SNACKS"), 5, true); STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NO_BOUGHT_EPIC_SNACKS"), 5, true); STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_CIGARETTES_BOUGHT"), 5, true); STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NUMBER_OF_ORANGE_BOUGHT"), 99999999, true); STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_NUMBER_OF_BOURGE_BOUGHT"), 99999999, true); }

				ImGui::SameLine();

				if (ImGui::Button("drop ballistic"))
				{
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						*script_global(2544210).at(884).as<int32_t*>() = 1;
					} QUEUE_JOB_END_CLAUSE
				}

				ImGui::SameLine();

				if (ImGui::Button("fall from sky"))
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true))
					{
						QUEUE_JOB_BEGIN_CLAUSE()
						{
							NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));

							while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false)))
								script::get_current()->yield(5ms);

							auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);

							ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), pos.x, pos.y, 1000.0f, FALSE, FALSE, FALSE, TRUE);
						} QUEUE_JOB_END_CLAUSE
					}
					else
					{
						QUEUE_JOB_BEGIN_CLAUSE()
						{
							auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);

							ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), pos.x, pos.y, 1000.0f, FALSE, FALSE, FALSE, TRUE);
						} QUEUE_JOB_END_CLAUSE
					}
				}
				
				static bool invisible = false;
				if (ImGui::Checkbox("Invisible", &invisible))
				{
					NETWORK::_NETWORK_SET_ENTITY_INVISIBLE_TO_NETWORK(PLAYER::PLAYER_PED_ID(), invisible);
				}

				static int HEALTH;
				if (ImGui::SliderInt("Health", &HEALTH, 0, 300))
				{
					g_fiber_pool->queue_job([]
					{
						ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), HEALTH, 0);
					});
				}

				/*
				static bool seatbelt = false;
				if (ImGui::Checkbox("Seatbelt", &seatbelt))
				{
					PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 32, seatbelt);
				}
				*/

				static char model_name[255];
				ImGui::InputText("##playermodel", model_name, 255);
				if (ImGui::Button("Player Model"))
				{
					g_fiber_pool->queue_job([]
					{

						Hash hash = MISC::GET_HASH_KEY(model_name);
						LOG(INFO) << hash;
						if (STREAMING::IS_MODEL_VALID(hash)
							&& STREAMING::IS_MODEL_IN_CDIMAGE(hash))
						{
							while (!STREAMING::HAS_MODEL_LOADED(hash))
							{
								LOG(INFO) << "REQUEST";
								STREAMING::REQUEST_MODEL(hash);
								LOG(INFO) << "REQUESTED";
								script::get_current()->yield();
							}
							LOG(INFO) << "Spawn";
							*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
							PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), hash);
							PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
							*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
							LOG(INFO) << "Spawned";
						}

						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
					});
				}

				if (ImGui::TreeNode("Animations"))
				{
					static char animDictionary[255];
					static char animationName[255];
					ImGui::InputText("animDictionary", animDictionary, 255);
					ImGui::InputText("animationName", animationName, 255);
					if (ImGui::Button("Play animation"))
					{
						g_fiber_pool->queue_job([]
						{
							while (!STREAMING::HAS_ANIM_DICT_LOADED(animDictionary))
							{
								STREAMING::REQUEST_ANIM_DICT(animDictionary);
								script::get_current()->yield();
							}
							TASK::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), animDictionary, animationName, 1.f, 1.f, -1, 1, -1, 0, 0, 0);
						});
					}

					ImGui::Separator();

					static char SCENARIoName[255];
					ImGui::InputText("SCENARIoName", SCENARIoName, 255);
					if (ImGui::Button("Play SCENARIo"))
					{
						g_fiber_pool->queue_job([]
						{
							TASK::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), SCENARIoName, -1, false);
						});
					}

					ImGui::Separator();

					static char Clipset[255];
					ImGui::InputText("Walk Style", Clipset, 255);
					if (ImGui::Button("Change Walk Style"))
					{
						g_fiber_pool->queue_job([]
						{
							STREAMING::REQUEST_ANIM_SET(Clipset);
							if (STREAMING::HAS_ANIM_SET_LOADED(Clipset))
							{
								PED::SET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), Clipset, 1);
							}
						});
					}

					ImGui::SameLine();


					if (ImGui::Button("Clear tasks"))
					{
						g_fiber_pool->queue_job([]
						{
							TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
						});
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Light Test"))
				{

					if (ImGui::Button("blip"))
					{
						HUD::SET_BLIP_SPRITE(HUD::GET_MAIN_PLAYER_BLIP_ID(), 788);
					}

					if (ImGui::Button("Climb Test"))
					{
						TASK::TASK_CLIMB(PLAYER::PLAYER_PED_ID(), true);
					}

					ImGui::Checkbox("Test Light", &features::self::light_test_bool);
					ImGui::ColorPicker3("Custom Vehicle Color", features::self::light_color, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Teleport"))
			{
				static float X_coord;
				static float Y_coord;
				static float Z_coord;
				ImGui::SliderFloat("X_coord", &X_coord, -10000, 10000);
				ImGui::SliderFloat("Y_coord", &Y_coord, -10000, 10000);
				ImGui::SliderFloat("Z_coord", &Z_coord, -10000, 10000);
				if (ImGui::Button("Teleport"))
				{
					Entity handle;
					PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : handle = PLAYER::PLAYER_PED_ID();
					ENTITY::SET_ENTITY_COORDS(handle, X_coord, Y_coord, Z_coord, false, false, false, false);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("world"))
			{
				if (ImGui::Button("Lightning"))
				{
					g_fiber_pool->queue_job([]
					{
						MISC::FORCE_LIGHTNING_FLASH();
					});
				}

				ImGui::SameLine();

				static bool blackout = false;
				if (ImGui::Checkbox("Blackout", &blackout))
				{
					GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(blackout);
				}

				static bool mobile_radio = false;
				if (ImGui::Checkbox("Mobile radio", &mobile_radio))
				{
					g_fiber_pool->queue_job([]
					{
						AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(mobile_radio);
					});
				}
				ImGui::SameLine();
				if (ImGui::Button("Stop Cutscene"))
				{
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
					} QUEUE_JOB_END_CLAUSE
				}
				ImGui::Separator();
				static char IPL_Name[255];
				ImGui::InputText("IPL NAME", IPL_Name, 255);
				if (ImGui::Button("Load IPL"))
				{
					g_fiber_pool->queue_job([]
					{
						STREAMING::REQUEST_IPL(IPL_Name);
					});
				}

				//_SET_ISLAND_HOPPER_ENABLED("HeistIsland", true)

				if (ImGui::Button("Load North Yankton"))
				{
					g_fiber_pool->queue_job([]
					{
						STREAMING::REQUEST_IPL("prologue01");
						STREAMING::REQUEST_IPL("prologue01c");
						STREAMING::REQUEST_IPL("prologue01d");
						STREAMING::REQUEST_IPL("prologue01e");
						STREAMING::REQUEST_IPL("prologue01f");
						STREAMING::REQUEST_IPL("prologue01g");
						STREAMING::REQUEST_IPL("prologue01h");
						STREAMING::REQUEST_IPL("prologue01i");
						STREAMING::REQUEST_IPL("prologue01j");
						STREAMING::REQUEST_IPL("prologue01k");
						STREAMING::REQUEST_IPL("prologue01z");
						STREAMING::REQUEST_IPL("prologue02");
						STREAMING::REQUEST_IPL("prologue03");
						STREAMING::REQUEST_IPL("prologue03b");
						STREAMING::REQUEST_IPL("prologue03_grv_dug");
						STREAMING::REQUEST_IPL("prologue_grv_torch");
						STREAMING::REQUEST_IPL("prologue04");
						STREAMING::REQUEST_IPL("prologue04b");
						STREAMING::REQUEST_IPL("prologue04_cover");
						STREAMING::REQUEST_IPL("des_protree_end");
						STREAMING::REQUEST_IPL("des_protree_start");
						STREAMING::REQUEST_IPL("prologue05");
						STREAMING::REQUEST_IPL("prologue05b");
						STREAMING::REQUEST_IPL("prologue06");
						STREAMING::REQUEST_IPL("prologue06b");
						STREAMING::REQUEST_IPL("prologue06_int");
						STREAMING::REQUEST_IPL("prologue06_pannel");
						STREAMING::REQUEST_IPL("plg_occl_00");
						STREAMING::REQUEST_IPL("prologue_occl");
						STREAMING::REQUEST_IPL("prologuerd");
						STREAMING::REQUEST_IPL("prologuerdb");
					});
				}
				ImGui::SameLine();
				static bool prologue_map = false;
				if (ImGui::Checkbox("Yankton map", &prologue_map))
				{
					HUD::SET_MINIMAP_IN_PROLOGUE(prologue_map);
				}

				ImGui::Separator();
				static int hours;
				static int minutes;
				if (ImGui::SliderInt("hours", &hours, 0, 23))
				{
					g_fiber_pool->queue_job([]
					{
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(hours, minutes, 0);
					});
				}
				if (ImGui::SliderInt("minutes", &minutes, 0, 60))
				{
					g_fiber_pool->queue_job([]
					{
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(hours, minutes, 0);
					});
				}
				static bool phone_disabled;
				if (ImGui::Checkbox("Mute phonecalls", &phone_disabled))
				{
					features::online::disable_phone(phone_disabled);
				}
				ImGui::SameLine();
				ImGui::Checkbox("no spam help", &features::self::no_spam_bool);

				//SET_PED_POPULATION_BUDGET
				static int ped_population;
				if (ImGui::SliderInt("Ped Population", &ped_population, 0, 3))
				{
					STREAMING::SET_PED_POPULATION_BUDGET(ped_population);
				}

				static int vehicle_population;
				if (ImGui::SliderInt("Vehicle Population", &vehicle_population, 0, 3))
				{
					STREAMING::SET_VEHICLE_POPULATION_BUDGET(vehicle_population	);
				}

				static float wave_STRENGTH;
				if (ImGui::SliderFloat("wave_STRENGTH", &wave_STRENGTH, 0, 23))
				{
					g_fiber_pool->queue_job([]
					{
						MISC::_0x31727907B2C43C55(wave_STRENGTH);
					});
				}
				ImGui::TreePop();
			}



			if (ImGui::TreeNode("spawns"))
			{
				static char vehicle_name[255];
				ImGui::InputText("vehicle name", vehicle_name, 255);
				if (ImGui::Button("Spawn by name"))
				{
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						Hash hash = MISC::GET_HASH_KEY(vehicle_name);
						while (!STREAMING::HAS_MODEL_LOADED(hash))
						{
							STREAMING::REQUEST_MODEL(hash);
							script::get_current()->yield();
						}

						auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
						*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
						Vehicle vehicle = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, 0.f, TRUE, FALSE, FALSE);
						*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574; //By writing the "old" bypass to the function, running CREATE_VEHICLE, then restoring it, the anti-cheat does not have enough time to catch the function in a dirty state.
						
						script::get_current()->yield();
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
						if (*g_pointers->m_is_session_started)
						{
							DECORATOR::DECOR_SET_INT(vehicle, "MPBitset", 0);
							ENTITY::_SET_ENTITY_SOMETHING(vehicle, TRUE); //True means it can be deleted by the engine when switching lobbies/missions/etc, false means the script is expected to clean it up.
							auto networkId = NETWORK::VEH_TO_NET(vehicle);
							if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(vehicle))
								NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
							VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, TRUE);
						}
					}
					QUEUE_JOB_END_CLAUSE
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Vehicle"))
			{
				if (ImGui::Button("add explosive"))
				{
					VEHICLE::ADD_VEHICLE_PHONE_EXPLOSIVE_DEVICE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
				}

				ImGui::SameLine();

				if (ImGui::Button("detonate explosive"))
				{
					VEHICLE::DETONATE_VEHICLE_PHONE_EXPLOSIVE_DEVICE();
				}
				ImGui::Separator();
				static int VEHICLE_HEALTH;
				if (ImGui::SliderInt("Enginer Health", &VEHICLE_HEALTH, -4000, 1000))
				{
					g_fiber_pool->queue_job([]
					{
						VEHICLE::SET_VEHICLE_ENGINE_HEALTH(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), VEHICLE_HEALTH);
					});
				}
				ImGui::Separator();

				static bool god_mode = false;
				if (ImGui::Checkbox("vehicle godmode", &god_mode))
				{
					g_fiber_pool->queue_job([]
					{
						ENTITY::SET_ENTITY_INVINCIBLE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), god_mode);
					});
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
						VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
						*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
					});
				}

				if (ImGui::Button("Delete Vehicle"))
				{
					g_fiber_pool->queue_job([]
					{
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));

						while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false)))
							script::get_current()->yield(5ms);
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh, true, true);
						VEHICLE::DELETE_VEHICLE(&veh);
					});
				}

				static float lights_power = 0.f;
				if (ImGui::SliderFloat("light_power", &lights_power, 0.f, 100.f))
				{
					g_fiber_pool->queue_job([]
					{
						VEHICLE::SET_VEHICLE_LIGHT_MULTIPLIER(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), lights_power);
					});
				}
				if (ImGui::Button("add maagnit to cargobob")) 
				{
					g_fiber_pool->queue_job([]
					{
						*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
						VEHICLE::CREATE_PICK_UP_ROPE_FOR_CARGOBOB(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 1);
						*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;
						VEHICLE::SET_CARGOBOB_PICKUP_MAGNET_ACTIVE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), true);
					});
				}
				static float max_vehicle_speed = 300.f;
				if (ImGui::SliderFloat("VEHICLE MAX SPEED", &max_vehicle_speed, 0.f, 6000.f))
				{
					g_fiber_pool->queue_job([]
					{
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));

						while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false)))
							script::get_current()->yield(5ms);
						VEHICLE::_SET_VEHICLE_MAX_SPEED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), max_vehicle_speed);
					});
				}
				
				static int selected_door = 0;
				const char* const vehicle_doors[]
				{
					"driver",
					"Passenger ",
					"Left Rear",
					"Right Rear",
					"hood",
					"5",
					"6",
					"7"
				};
				ImGui::Combo("vehicle door", &selected_door, vehicle_doors, IM_ARRAYSIZE(vehicle_doors));
				if (ImGui::Button("open selected door"))
				{
					g_fiber_pool->queue_job([]
					{
						VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), selected_door, false, true);
					});
				}

				ImGui::SameLine();

				if (ImGui::Button("close selected door"))
				{
					g_fiber_pool->queue_job([]
					{
						VEHICLE::SET_VEHICLE_DOOR_SHUT(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), selected_door, true);
					});
				}
				if (ImGui::Button("stop vehicle"))
				{
					g_fiber_pool->queue_job([]
					{
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));

						while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false)))
							script::get_current()->yield(5ms);
						VEHICLE::BRING_VEHICLE_TO_HALT(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 3, 1, 0);
					});

				}

				ImGui::SameLine();

				if (ImGui::Button("Rappel From Heli"))
				{
					g_fiber_pool->queue_job([]
					{
						TASK::TASK_RAPPEL_FROM_HELI(PLAYER::PLAYER_PED_ID(), 1);
					});

				}

				ImGui::SameLine();

				ImGui::Checkbox("Vehicle Flares", &features::vehicles::enableflares_bool);
				//
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Weapons"))
			{
				if (ImGui::Button("impulse 101"))
				{
					QUEUE_JOB_BEGIN_CLAUSE()
					{
						//PED::_0xB782F8238512BAD5(PLAYER::PLAYER_PED_ID(), nullptr); //This causes a crash at GTA5.exe+5845356 and nothing of value was in the log in the stack dump because of the context switch to GTA 5's memory. If you encounter something similar, you will have to figure out where the crash occured in the GTA 5 exe, and trace back that native, and figure out which function is calling the native that is crashing.
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xB1CA77B1, 20, false, false);   //weapon_rpg
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA284510B, 20, false, false);   //weapon_grenadelauncher
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x42BF8A85, 9999, false, false); //weapon_minigun
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7F7497E5, 20, false, false);   //weapon_firework
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x6D544C99, 20, false, false);   //weapon_railgun
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x63AB0442, 20, false, false);   //weapon_hominglauncher
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x0781FE4A, 20, false, false);   //weapon_compactlauncher

						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x9D07F764, 9999, false, false); //weapon_mg
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x7FD62962, 9999, false, false); //weapon_combatmg
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x61012683, 9999, false, false); //weapon_gusenberg
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x0C472FE2, 9999, false, false); //weapon_heavysniper
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xC734385A, 9999, false, false); //weapon_marksmanrifle
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xAF113F99, 9999, false, false); //weapon_advancedrifle
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA89CB99E, 9999, false, false); //weapon_musket
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x13532244, 9999, false, false); //weapon_microsmg
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x2BE6766B, 9999, false, false); //weapon_smg
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xEFE7E2DF, 9999, false, false); //weapon_assaultsmg
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x47757124, 20, false, false);   //weapon_flaregun
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x060EC506, 9999, false, false); //weapon_fireextinguisher
						//WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x4DD2DC56, 20, false, false);   //weapon_grenadelauncher_smoke //dont work
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 2, false, false); //gadget_parachute
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x34A67B97, 2, false, false); //weapon_petrolcan

						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x93E220BD, 30, false, false); //weapon_grenade
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x787F0BB, 9, false, false); //weapon_snowball
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x23C9F95C, 2, false, false); //weapon_ball
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xBA45E8B8, 30, false, false); //weapon_pipebomb
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFDBC8A50, 30, false, false); //weapon_smokegrenade
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x497FACC3, 30, false, false); //weapon_flare
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0x84BD7BFD, 1, false, false); //weapon_crowbar
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 2, false, false); //
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 2, false, false); //
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 2, false, false); //
						WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 2, false, false); //
					}
					QUEUE_JOB_END_CLAUSE
				}

				static char weapon_name[255];
				ImGui::InputText("Weapon name", weapon_name, 255);

				ImGui::Separator();

				if (ImGui::Button("Give Weapon"))
				{
					Hash hash = MISC::GET_HASH_KEY(weapon_name);
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 1000, 1);
				}

				ImGui::SameLine();

				if (ImGui::Button("Remove Weapon"))
				{
					Hash hash = MISC::GET_HASH_KEY(weapon_name);
					WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), hash);
				}
				ImGui::Checkbox("Remover", &features::weapons::WeaponDeleteGun_bool);
				ImGui::TreePop();

			}

			


			ImGui::Separator();

			if (ImGui::Button("Unload"))
			{
				g_running = false;
			}
			ImGui::EndTabItem();
		}
	}
}