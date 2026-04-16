/* 

    Plumas, portones, puertas, barreras, etc
        Prime Roleplay

*/
#include <YSI_Coding\y_hooks>

enum San_Andreas_Barriers_Info {
	barrier_VEHICLE_TYPE,
	barrier_VEHICLE_TYPE_IN,
	barrier_VEHICLE_TYPE_IN2,
	Float:barrier_X,
	Float:barrier_Y,
	Float:barrier_Z,
	Float:barrier_ROTATION,
	barrier_INTERIOR,
	barrier_WORLD,
	barrier_OBJECT_ID,
	barrier_TIMER,
	bool:barrier_CLOSING,
	Float:barrier_DISTANCE,
	Float:barrier_PLAYER_X,
	Float:barrier_PLAYER_Y,
	Float:barrier_PLAYER_Z
}
static const San_Andreas_Barriers[][San_Andreas_Barriers_Info] = {
	{VEHICLE_TYPE_WORK, WORK_TAXI, WORK_NONE, 1811.32373, -1886.26331, 12.36843, 90.00000, 0, 0, INVALID_STREAMER_ID, -1, false, 15.0, 0.0, 0.0, 0.0}, // barrera taxi ls
	{VEHICLE_TYPE_WORK, WORK_TAXI, WORK_NONE, 1717.0457, 1308.0387, 9.7157, 90.0, 0, 0, INVALID_STREAMER_ID, -1, false, 15.0, 0.0, 0.0, 0.0}, // barrera taxi lv
	{VEHICLE_TYPE_NONE, WORK_NONE, WORK_NONE, -24.879825, -316.779937, 4.422851, 0.0, 0, 0, INVALID_STREAMER_ID, -1, false, 15.0, 0.0, 0.0, 0.0}, // barrera camioneros TRUCK
	{VEHICLE_TYPE_NONE, WORK_NONE, WORK_NONE, -82.1645, -1123.0260, 0.0373, 67.1102, 0, 0, INVALID_STREAMER_ID, -1, false, 15.0, 0.0, 0.0, 0.0}, // grua
	{VEHICLE_TYPE_WORK, WORK_POLICE, WORK_NONE, 1544.69629, -1630.79651, 12.31840, -90.0, 0, 0, INVALID_STREAMER_ID, -1, false, 15.0, 0.0, 0.0, 0.0}, // barrera POLICIA LS
	{VEHICLE_TYPE_WORK, WORK_POLICE, WORK_NONE, 2238.19897, 2450.41797, 9.80680, -90.0, 0, 0, INVALID_STREAMER_ID, -1, false, 15.0, 0.0, 0.0, 0.0}, // barrera POLICIA LV
	{VEHICLE_TYPE_WORK, WORK_POLICE, WORK_NONE, -1572.20313, 658.83588, 6.07810, -90.0, 0, 0, INVALID_STREAMER_ID, -1, false, 15.0, 0.0, 0.0, 0.0}, // barrera POLICIA SF 1
	{VEHICLE_TYPE_WORK, WORK_POLICE, WORK_NONE, -1701.42969, 687.59381, 23.88280, 90.0, 0, 0, INVALID_STREAMER_ID, -1, false, 15.0, 0.0, 0.0, 0.0} // barrera POLICIA SF 2
};

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (newkeys & KEY_CROUCH) {
        if (PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL) {
            if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
                for(new i = 0; i != sizeof San_Andreas_Barriers; i ++) {
                    if (IsPlayerInRangeOfPoint(playerid, San_Andreas_Barriers[i][barrier_DISTANCE], San_Andreas_Barriers[i][barrier_PLAYER_X], San_Andreas_Barriers[i][barrier_PLAYER_Y], San_Andreas_Barriers[i][barrier_PLAYER_Z])) {
                        if (GetPlayerInterior(playerid) == San_Andreas_Barriers[i][barrier_INTERIOR] && GetPlayerVirtualWorld(playerid) == San_Andreas_Barriers[i][barrier_WORLD]) {
                            if (San_Andreas_Barriers[i][barrier_VEHICLE_TYPE] == VEHICLE_TYPE_WORK) {
                                if (!PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET] && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) {
                                    if (San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN2] != WORK_NONE) {
                                        if (!PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][pwork_SET] && !PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN2] ][pwork_SET]) break;
                                    } else {
                                        if (!PLAYER_WORKS[playerid][ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][pwork_SET]) break;
                                    }
                                }
                            }

                            if (IsDynamicObjectMoving(San_Andreas_Barriers[i][barrier_OBJECT_ID]) && !San_Andreas_Barriers[i][barrier_CLOSING]) break;
                            
                            KillTimer(San_Andreas_Barriers[i][barrier_TIMER]);
                            San_Andreas_Barriers[i][barrier_TIMER] = SetTimerEx("CloseBarrier", 5000, false, "i", i);
                            MoveDynamicObject(San_Andreas_Barriers[i][barrier_OBJECT_ID], San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z] + 0.72967 + 0.01, 0.0065, 0.0, 0.0, San_Andreas_Barriers[i][barrier_ROTATION]);
                            San_Andreas_Barriers[i][barrier_CLOSING] = false;
                            break;
                        }
                        else break;
                    }
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnScriptInit() {
	for(new i = 0; i != sizeof San_Andreas_Barriers; i ++) {
		CreateDynamicObject(966, San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z], 0.0, 0.0, San_Andreas_Barriers[i][barrier_ROTATION], San_Andreas_Barriers[i][barrier_WORLD], San_Andreas_Barriers[i][barrier_INTERIOR], .streamdistance = 600.0, .drawdistance = 600.0);
		San_Andreas_Barriers[i][barrier_OBJECT_ID] = CreateDynamicObject(968, San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z] + 0.72967, 0.0, -90.0, San_Andreas_Barriers[i][barrier_ROTATION], San_Andreas_Barriers[i][barrier_WORLD], San_Andreas_Barriers[i][barrier_INTERIOR], .streamdistance = 600.0, .drawdistance = 600.0);

        new label_str[180];
        if (San_Andreas_Barriers[i][barrier_VEHICLE_TYPE]) format(label_str, sizeof label_str, "{%s}%c%s\n\n{FFFFFF}Pulsa {"#PRIMARY_COLOR"}H{FFFFFF} o toca la {"#SECONDARY_COLOR"}Bocina{FFFFFF} para abrir", work_info[ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][work_info_COLOR], toupper(work_info[ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][work_info_NAME][0]), work_info[ San_Andreas_Barriers[i][barrier_VEHICLE_TYPE_IN] ][work_info_NAME][1]);
        else format(label_str, sizeof label_str, "{FFFFFF}Pulsa {"#PRIMARY_COLOR"}H{FFFFFF} o toca la {"#SECONDARY_COLOR"}Bocina{FFFFFF} para abrir");
		CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z] + 1.5, 15.0, .worldid = San_Andreas_Barriers[i][barrier_WORLD], .interiorid = San_Andreas_Barriers[i][barrier_INTERIOR], .testlos = true);

		San_Andreas_Barriers[i][barrier_PLAYER_X] = San_Andreas_Barriers[i][barrier_X] + (3.4 * floatsin(-(San_Andreas_Barriers[i][barrier_ROTATION] + 90.0), degrees));
		San_Andreas_Barriers[i][barrier_PLAYER_Y] = San_Andreas_Barriers[i][barrier_Y] + (3.4 * floatcos(-(San_Andreas_Barriers[i][barrier_ROTATION] + 90.0), degrees));
		San_Andreas_Barriers[i][barrier_PLAYER_Z] = San_Andreas_Barriers[i][barrier_Z];
	}
}

forward CloseBarrier(i);
public CloseBarrier(i) {
	San_Andreas_Barriers[i][barrier_CLOSING] = true;
	MoveDynamicObject(San_Andreas_Barriers[i][barrier_OBJECT_ID], San_Andreas_Barriers[i][barrier_X], San_Andreas_Barriers[i][barrier_Y], San_Andreas_Barriers[i][barrier_Z] + 0.72967, 0.0065, 0.0, -90.0, San_Andreas_Barriers[i][barrier_ROTATION]);
	return 1;
}
