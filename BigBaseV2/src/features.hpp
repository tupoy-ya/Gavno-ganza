#pragma once
#include "common.hpp"

namespace big::features
{
	// Players list
	namespace online {
		extern bool block_afk_bool;
		extern bool Freeze_player_bool;

		void show_players();
		void draw_extra_info();
		extern int selectedPlayer;

		void teleport_to_player(Player player);
		void cage(Player player);

		int ReturnPlayerRockstarID(Player PlayerHandle);
		void EnableDisableAntiCrashCamera();
		void block_afk();
		void Freeze_player();
		void change_clothe(int componentId, int drawableId, int textureId, int paletteId);
		void disable_phone(bool b);
		void bst();
		void bribe();
		void no_orb_cooldown();
		void ewo_cooldown_remove();
		void grinding_mode();
		void block_vote();
	}

	namespace basic {
		float deg_to_rad(float deg);
		Hash load(char* str);
		Hash $(std::string str);
	}

	// Recovery 
	namespace recovery {

		void packed_bool(int loop, int looptwo, char* boolname);
		void remove_bad_sport();
		void reset_reports();
		void unlock_fast_run();
		void unlock_wireframe_suits();
		void unlock_raygun_paint();
		void unlock_character_stats();
		void unlock_weapon_colors();
		void setup_cayo(const int choice);
		void change_kd(const int choice, const int kills = 100, const int deaths = 0);
	}

	namespace weapons {
		extern bool unlimited_ammo_bool;
		extern bool unlimited_clip_bool;
		extern bool WeaponDeleteGun_bool;

		void unlimited_ammo(Ped ped);
		void unlimited_clip(Ped ped);
		void give_all_weps(Ped ped);
		void give_all_mk2(Ped ped);
		void give_stun_gun(Ped ped);
		void give_scanner(Ped ped);
		void WeaponDeleteGun();
		void refill_ammo(Ped ped);
	}

	namespace vehicles {
		extern bool enableflares_bool;
		void max_vehicle();
		void spawn_vehicle(Ped ped, char* c);
		void enableflares();
		void set_plate(Vehicle veh, char* c);
	}

	namespace objects {
		inline float  x_rot;
		inline float  y_rot;
		inline float  z_rot;
		inline float  x_pos;
		inline float  y_pos;
		inline float  z_pos;
		void request_control_id(int network_id);
		void request_control_ent(Entity entity);
		void attach_obj_to_player(char* object, Player player);
		void spawnPed(const char* pedName, Player player);
		void spawnGriferPed(const char* pedName, Player player);
		void spawnExtimeGriferPed(const char* pedName, Player player);
		void spawnGriferJet(const char* pedName, const char* vehicle, Player player, bool isUFO);
		void spawnGriferVehicle(const char* pedName, const char* vehName, Player player);
		void detach_all(Player player);
	}

	// Self
	namespace self {
		extern bool god_mode_bool;
		extern bool never_wanted_bool;
		extern bool buff_hp_regen_bool;
		extern bool super_jump_bool;
		extern bool no_clip_bool;
		extern bool light_test_bool;
		extern bool no_spam_bool;
		extern bool pron_mod_bool;
		inline float light_color[3]{};

		Vector3 get_blip_marker();
		Vector3 get_mission_objective();

		void tp_to_destination(Entity e, Vector3 coords);
		void tp_to_waypoint();
		void tp_to_objective();

		void god_mode(Player player);
		void never_wanted(Player player);
		void buff_hp_regen(Player player);
		void super_jump();
		void light_test();
		void no_spam();
		void pron_mod();
		void no_clip();
		void give_needs(Ped ped);
		void player_alpha(int alpha);
		void suicide(Ped ped);
		void skip_cutscene();
	}

	void run_tick();
	void script_func();
}
