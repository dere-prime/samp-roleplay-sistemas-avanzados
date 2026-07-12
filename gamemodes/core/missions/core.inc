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
    mission_updates_timer
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
    MISSION_CLEAN_THE_BOAT
}

new 
    mission_info[MAX_PLAYERS][e_player_mission_info],
    player_mission[MAX_PLAYERS][MAX_MISSIONS][e_player_missions],
    
    PlayerText:Mission_Dialog[MAX_PLAYERS][4];

forward OnPlayerFinishMission(const playerid, const index);

#define IsPlayerMission(%0) mission_info[%0][mission_current]
#define PlayerMissionId(%0) mission_info[%0][mission_id]
#define PlayerMissionStep(%0) mission_info[%0][mission_step]
#define MissionUpdates(%0) mission_info[%0][mission_updates]

#define Mission:%0(%1) forward mission_%0(%1); public mission_%0(%1)

//Impl
public OnPlayerFinishMission(const playerid, const index) {
    if (index == MISSION_THE_APPERANCE) {
        if(PlayerMissionId(playerid) != MISSION_THE_APPERANCE) return 0;

        player_mission[playerid][MISSION_THE_APPERANCE][m_completed] = true;
        mission_info[playerid][mission_step] = -1;

        TogglePlayerSpectatingEx(playerid, false);

        PI[playerid][pi_STATE] = ROLEPLAY_STATE_NORMAL;
        PI[playerid][pi_LOCAL_INTERIOR] = 0;
        PI[playerid][pi_POS_X] = SERVER_MAIN_SPAWN_X;
        PI[playerid][pi_POS_Y] = SERVER_MAIN_SPAWN_Y;
        PI[playerid][pi_POS_Z] = SERVER_MAIN_SPAWN_Z;
        PI[playerid][pi_ANGLE] = SERVER_MAIN_SPAWN_ANGLE;
        PI[playerid][pi_INTERIOR] = 0;
        
        SetPlayerPosEx(playerid, PI[playerid][pi_POS_X], PI[playerid][pi_POS_Y], PI[playerid][pi_POS_Z], PI[playerid][pi_ANGLE], PI[playerid][pi_INTERIOR], 0);

        SetCameraBehindPlayer(playerid);
        TogglePlayerControllableEx(playerid, false);
        SetPlayerVirtualWorld(playerid, 0);

        EaseHidePlayerBlackScreen(playerid);
        return 1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

stock PlayPlayerMission(const playerid, const mission[], const index, const bool:bypass = false) {
    if (IsPlayerMission(playerid)) return ShowPlayerMessage(playerid, "~r~Ahora mismo estas en una mision.");
    if (bypass == false && PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL || PI[playerid][pi_STATE] != ROLEPLAY_STATE_NORMAL) return ShowPlayerMessage(playerid, "~r~Ahora no puedes hacer misiones.");
    
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
