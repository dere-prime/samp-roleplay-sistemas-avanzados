/* 

    Missions
        Core

*/

#include <YSI_Coding\y_hooks>

#define MAX_MISSIONS 2

//Header
enum e_player_mission_info {
    bool:mission_current,
    mission_id,
    mission_step,
    mission_updates,
    mission_updates_timer,
    mission_checkpoint
}

enum e_player_missions {
    bool:m_completed
}

enum {
    MISSION_STEP_PLAYABLE,
    MISSION_STEP_CINEMATIC
}

enum {
    MISSION_THE_APPERANCE,
    MISSION_CLEAN_THE_BOAT,
    MISSION_WEAPONS_FACTORY
}

new 
    mission_info[MAX_PLAYERS][e_player_mission_info],
    player_mission[MAX_PLAYERS][MAX_MISSIONS][e_player_missions],
    
    PlayerText:Mission_Dialog[MAX_PLAYERS][4];

forward OnPlayerFinishMission(const playerid, const index); //Starter

#define IsPlayerMission(%0) mission_info[%0][mission_current]
#define PlayerMissionId(%0) mission_info[%0][mission_id]
#define PlayerMissionStep(%0) mission_info[%0][mission_step]
#define MissionUpdates(%0) mission_info[%0][mission_updates]

#define Mission:%0(%1) forward mission_%0(%1); public mission_%0(%1)

//Impl
public OnPlayerFinishMission(const playerid, const index) {
    switch (index) {
        case MISSION_THE_APPERANCE: {
            missionTAFinish(playerid);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

/* Code */
stock PlayPlayerMission(const playerid, const mission[], const index, const bool:bypass = false) {
    if (IsPlayerMission(playerid)) return ShowPlayerMessage(playerid, "~r~Ahora mismo estas en una mision.");
    if (bypass == false && PI[playerid][pi_WANTED_LEVEL] > 0 || PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL || PI[playerid][pi_STATE] != ROLEPLAY_STATE_NORMAL) return ShowPlayerMessage(playerid, "~r~Ahora no puedes hacer misiones.");

    mission_info[playerid][mission_current] = true;
    mission_info[playerid][mission_id] = index;
    //mission_info[playerid][mission_step] = 0;

    new gMissionFunction[64] = "mission_";
    strcat(gMissionFunction, mission, sizeof(gMissionFunction));
    CallLocalFunction(gMissionFunction, "dd", playerid, index);
    return 1;
}
stock LoadPlayerMissions(const playerid) {
	if(PI[playerid][pi_ID] == 0) return 0;
	
	inline OnPlayerMissionsLoad() {
		new rows;
		if(cache_get_row_count(rows)) {
			for(new i = 0; i != rows; i ++) {
                new index;
                cache_get_value_name_int(i, "id_mission", index);
                cache_get_value_name_bool(i, "completed", player_mission[playerid][index][m_completed]);
			}
		}
	}
	mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM missions WHERE id_player = %d;", PI[playerid][pi_ID]);
	MySQL_TQueryInline(handle_db,  using inline OnPlayerMissionsLoad, QUERY_BUFFER);
    return 1;
}
stock SavePlayerMissions(const playerid) {
    for (new i = 0; i != MAX_MISSIONS; i ++) {
        mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO missions (id_player, id_mission, completed) VALUES(%d, %d, %d) ON DUPLICATE KEY UPDATE completed = VALUES(completed);", PI[playerid][pi_ID], i, player_mission[playerid][i][m_completed]);
		mysql_tquery(handle_db, QUERY_BUFFER);
    }
    return 1;
}
forward HidePlayerMissionDialog(playerid, bool: controllable);
public HidePlayerMissionDialog(playerid, bool: controllable) {
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][17]);
	if (!PLAYER_TEMP[playerid][pt_USER_LOGGED]) return 0;
    if (controllable) TogglePlayerControllableEx(playerid, true);
	PlayerTextDrawSetString(playerid, Mission_Dialog[playerid][3], "_");
	PlayerTextDrawHide(playerid, Mission_Dialog[playerid][0]);
	PlayerTextDrawHide(playerid, Mission_Dialog[playerid][1]);
	PlayerTextDrawHide(playerid, Mission_Dialog[playerid][2]);
	PlayerTextDrawHide(playerid, Mission_Dialog[playerid][3]);
	return 1;
}
stock ShowPlayerMissionDialog(playerid, skin, const tittle[], const message[], GLOBAL_TAG_TYPES:...) {
    if (!skin) skin = 19300;
	if (YSI_CheckNumargs__(4))
		return bool:PlayerTextDrawSetString(playerid, Mission_Dialog[playerid][3], __const(message)),
        PlayerTextDrawSetPreviewModel(playerid, Mission_Dialog[playerid][1], skin),
        bool:PlayerTextDrawSetString(playerid, Mission_Dialog[playerid][2], __const(tittle)),
		PlayerTextDrawShow(playerid, Mission_Dialog[playerid][0]), PlayerTextDrawShow(playerid, Mission_Dialog[playerid][1]),
        PlayerTextDrawShow(playerid, Mission_Dialog[playerid][2]), PlayerTextDrawShow(playerid, Mission_Dialog[playerid][3]);
	return
		format(YSI_UNSAFE_HUGE_STRING, YSI_UNSAFE_HUGE_LENGTH, message, ___(4)),
        PlayerTextDrawSetPreviewModel(playerid, Mission_Dialog[playerid][1], skin),
		bool:PlayerTextDrawSetString(playerid, Mission_Dialog[playerid][3], YSI_UNSAFE_HUGE_STRING),
        bool:PlayerTextDrawSetString(playerid, Mission_Dialog[playerid][2], __const(tittle)),
		PlayerTextDrawShow(playerid, Mission_Dialog[playerid][0]), PlayerTextDrawShow(playerid, Mission_Dialog[playerid][1]),
        PlayerTextDrawShow(playerid, Mission_Dialog[playerid][2]), PlayerTextDrawShow(playerid, Mission_Dialog[playerid][3]);
}
stock createMissionCheckpoint(const playerid, Float: x, Float: y, Float: z, world, interior, Float: size = 5.0) {
	if (IsValidDynamicCP(mission_info[playerid][mission_checkpoint])) DestroyDynamicCP(mission_info[playerid][mission_checkpoint]);
	mission_info[playerid][mission_checkpoint] = CreateDynamicCP(x, y, z, size, world, interior, playerid, 9999999999.0);
	
	new info[1];
	info[0] = CHECKPOINT_TYPE_MISSION;
	Streamer_SetArrayData(STREAMER_TYPE_CP, mission_info[playerid][mission_checkpoint], E_STREAMER_EXTRA_ID, info);
	Streamer_Update(playerid, STREAMER_TYPE_CP);
}

stock createMissionVehicle(modelid, Float: X, Float: Y, Float: Z, Float: Angle, world, interior, Color1, Color2, bool: static_veh) { 
	new vehicle_id = INVALID_VEHICLE_ID;
	vehicle_id = CreateVehicle(modelid, X, Y, Z, Angle, Color1, Color2, -1, false);
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_NONE;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = modelid;
	format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "SAMP");
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = X;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = Y;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = Z;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = Angle;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = Color1;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = Color2;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3; // No paintjob
    if (static_veh) {
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PARAMS_DOORS] = 1;
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PARAMS_ENGINE] = 0;
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = 0.0;
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = 0.0;
    } else {
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = VEHICLE_INFO[modelid - 400][vehicle_info_MAX_GAS];
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS];
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PARAMS_DOORS] = 0;
    }
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_INTERIOR] = interior;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = world;		
	UpdateVehicleParams(vehicle_id);
	SetVehicleVirtualWorldEx(vehicle_id, world);
	SetVehicleToRespawnEx(vehicle_id);
    if (static_veh) SetVehicleActiveStaticPos(vehicle_id);
	return vehicle_id;
}

/*hook OnPlayerEnterDynamicCP(playerid, checkpointid) {
    new info[1];
	Streamer_GetArrayData(STREAMER_TYPE_CP, checkpointid, E_STREAMER_EXTRA_ID, info);
	if (info[0] == CHECKPOINT_TYPE_MISSION) {
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}*/

hook OnPlayerConnect(playerid) {
    Mission_Dialog[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 325.000, "bg");
    PlayerTextDrawLetterSize(playerid, Mission_Dialog[playerid][0], 0.000, 15.000);
    PlayerTextDrawAlignment(playerid, Mission_Dialog[playerid][0], 2);
    PlayerTextDrawColor(playerid, Mission_Dialog[playerid][0], 255);
    PlayerTextDrawUseBox(playerid, Mission_Dialog[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, Mission_Dialog[playerid][0], 155);
    PlayerTextDrawSetShadow(playerid, Mission_Dialog[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, Mission_Dialog[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, Mission_Dialog[playerid][0], 255);
    PlayerTextDrawFont(playerid, Mission_Dialog[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, Mission_Dialog[playerid][0], 1);

    Mission_Dialog[playerid][1] = CreatePlayerTextDraw(playerid, -55.000, 245.000, "_");
    PlayerTextDrawTextSize(playerid, Mission_Dialog[playerid][1], 190.000, 190.000);
    PlayerTextDrawAlignment(playerid, Mission_Dialog[playerid][1], 1);
    PlayerTextDrawColor(playerid, Mission_Dialog[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, Mission_Dialog[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, Mission_Dialog[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, Mission_Dialog[playerid][1], 0);
    PlayerTextDrawFont(playerid, Mission_Dialog[playerid][1], 5);
    PlayerTextDrawSetProportional(playerid, Mission_Dialog[playerid][1], 0);
    PlayerTextDrawSetPreviewModel(playerid, Mission_Dialog[playerid][1], 19300);
    PlayerTextDrawSetPreviewRot(playerid, Mission_Dialog[playerid][1], 0.000, 0.000, 30.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, Mission_Dialog[playerid][1], 0, 0);

    Mission_Dialog[playerid][2] = CreatePlayerTextDraw(playerid, 76.000, 330.000, "Tittle");
    PlayerTextDrawLetterSize(playerid, Mission_Dialog[playerid][2], 0.400, 2.000);
    PlayerTextDrawAlignment(playerid, Mission_Dialog[playerid][2], 1);
    PlayerTextDrawColor(playerid, Mission_Dialog[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, Mission_Dialog[playerid][2], 1);
    PlayerTextDrawSetOutline(playerid, Mission_Dialog[playerid][2], 1);
    PlayerTextDrawBackgroundColor(playerid, Mission_Dialog[playerid][2], 0);
    PlayerTextDrawFont(playerid, Mission_Dialog[playerid][2], 1);
    PlayerTextDrawSetProportional(playerid, Mission_Dialog[playerid][2], 1);

    Mission_Dialog[playerid][3] = CreatePlayerTextDraw(playerid, 76.000, 355.000, "Text");
    PlayerTextDrawLetterSize(playerid, Mission_Dialog[playerid][3], 0.350, 1.800);
    PlayerTextDrawTextSize(playerid, Mission_Dialog[playerid][3], 580.000, 0.000);
    PlayerTextDrawAlignment(playerid, Mission_Dialog[playerid][3], 1);
    PlayerTextDrawColor(playerid, Mission_Dialog[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, Mission_Dialog[playerid][3], 1);
    PlayerTextDrawSetOutline(playerid, Mission_Dialog[playerid][3], 1);
    PlayerTextDrawBackgroundColor(playerid, Mission_Dialog[playerid][3], 0);
    PlayerTextDrawFont(playerid, Mission_Dialog[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, Mission_Dialog[playerid][3], 1);
}