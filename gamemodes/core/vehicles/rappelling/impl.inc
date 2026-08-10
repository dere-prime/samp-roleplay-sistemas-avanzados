/*
*
*   Vetra Roleplay
*       Rappelling
*
*/

//... Includes
#include <YSI_Coding\y_hooks>

//... Functions
stock start_player_rappeling(const playerid, const vehicleid) {
    if (!Vehicle_IsHelicopter(vehicleid)) return 0;
    if (rappelling_data[playerid][r_rappelling]) return 0;

    new Float:x, Float:y, Float:z, Float:z_terrain;
    GetPlayerPos(playerid, x, y, z);
    CA_FindZ_For2DCoord(x, y, z_terrain);

    rappelling_data[playerid][r_rappelling] = true;

    if (IsValidDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint])) {
        DestroyDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint]);
        rappelling_data[playerid][r_rappelling_checkpoint] = INVALID_STREAMER_ID;
    }
    rappelling_data[playerid][r_rappelling_checkpoint] = CreateDynamicCP(x, y, z_terrain + 1.0, 5.0, 0, 0, playerid, 9999999999.0);
    new info[1];
    info[0] = CHECKPOINT_TYPE_RAPPELLING;
    Streamer_SetArrayData(STREAMER_TYPE_CP, rappelling_data[playerid][r_rappelling_checkpoint], E_STREAMER_EXTRA_ID, info);
    Streamer_Update(playerid, STREAMER_TYPE_CP);

    SetPlayerPos(playerid, x, y, z - 1.0);
    ApplyAnimation(playerid, "PED", "abseil", 4.1, 1, 0, 0, 0, 0, true);

    for(new i = 0; i != RAPPELLING_OBJECTS; i ++) {
        rappelling_data[playerid][r_rappelling_objectid][i] = CreateDynamicObject(3004, x, y, z, 86.0, 0.0, 0.0, 0, 0, -1, 300.0, 300.0);
        z -= 0.5;
    }

    rappeling_timeout(playerid);
    return 1;
}

//... Callbacks
public rappeling_timeout(const playerid) {
    if (rappelling_data[playerid][r_rappelling_timeout] != -1) {
        KillTimer(rappelling_data[playerid][r_rappelling_timeout]);
        rappelling_data[playerid][r_rappelling_timeout] = -1;
    }

    if (rappelling_data[playerid][r_rappelling]) {
        new Float:p_altitude;
        GetPlayerAltitude(playerid, p_altitude);

        if (p_altitude <= 3.0) {
            rappelling_data[playerid][r_rappelling] = false;
            ClearAnimations(playerid);

            for(new i = 0; i != RAPPELLING_OBJECTS; i ++) {
                if (IsValidDynamicObject(rappelling_data[playerid][r_rappelling_objectid][i])) {
                    DestroyDynamicObject(rappelling_data[playerid][r_rappelling_objectid][i]);
                    rappelling_data[playerid][r_rappelling_objectid][i] = INVALID_OBJECT_ID;
                }
            }
            if (rappelling_data[playerid][r_rappelling_timeout] != -1) {
                KillTimer(rappelling_data[playerid][r_rappelling_timeout]);
                rappelling_data[playerid][r_rappelling_timeout] = -1;
            }
            if (IsValidDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint])) {
                DestroyDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint]);
                rappelling_data[playerid][r_rappelling_checkpoint] = INVALID_STREAMER_ID;
            }
            return 1;
        }
        rappelling_data[playerid][r_rappelling_timeout] = SetTimerEx("rappeling_timeout", 1000, false, "d", playerid);
    }
    return 1;
}

hook OnPlayerConnect(playerid) {
    rappelling_data[playerid][r_rappelling] = false;
    for(new i = 0; i != RAPPELLING_OBJECTS; i ++) {
        if (IsValidDynamicObject(rappelling_data[playerid][r_rappelling_objectid][i])) {
            DestroyDynamicObject(rappelling_data[playerid][r_rappelling_objectid][i]);
            rappelling_data[playerid][r_rappelling_objectid][i] = INVALID_OBJECT_ID;
        }
    }
    if (rappelling_data[playerid][r_rappelling_timeout] != -1) {
        KillTimer(rappelling_data[playerid][r_rappelling_timeout]);
        rappelling_data[playerid][r_rappelling_timeout] = -1;
    }
    if (IsValidDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint])) {
        DestroyDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint]);
        rappelling_data[playerid][r_rappelling_checkpoint] = INVALID_STREAMER_ID;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid) {
    rappelling_data[playerid][r_rappelling] = false;
    for(new i = 0; i != RAPPELLING_OBJECTS; i ++) {
        if (IsValidDynamicObject(rappelling_data[playerid][r_rappelling_objectid][i])) {
            DestroyDynamicObject(rappelling_data[playerid][r_rappelling_objectid][i]);
            rappelling_data[playerid][r_rappelling_objectid][i] = INVALID_OBJECT_ID;
        }
    }
    if (rappelling_data[playerid][r_rappelling_timeout] != -1) {
        KillTimer(rappelling_data[playerid][r_rappelling_timeout]);
        rappelling_data[playerid][r_rappelling_timeout] = -1;
    }
    if (IsValidDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint])) {
        DestroyDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint]);
        rappelling_data[playerid][r_rappelling_checkpoint] = INVALID_STREAMER_ID;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterDynamicCP(playerid, STREAMER_TAG_CP:checkpointid) {
	new info[1];
	Streamer_GetArrayData(STREAMER_TYPE_CP, checkpointid, E_STREAMER_EXTRA_ID, info);

	switch(info[0]) {
		case CHECKPOINT_TYPE_RAPPELLING: {
            rappelling_data[playerid][r_rappelling] = false;
            ClearAnimations(playerid);

            for(new i = 0; i != RAPPELLING_OBJECTS; i ++) {
                if (IsValidDynamicObject(rappelling_data[playerid][r_rappelling_objectid][i])) {
                    DestroyDynamicObject(rappelling_data[playerid][r_rappelling_objectid][i]);
                    rappelling_data[playerid][r_rappelling_objectid][i] = INVALID_OBJECT_ID;
                }
            }
            if (rappelling_data[playerid][r_rappelling_timeout] != -1) {
                KillTimer(rappelling_data[playerid][r_rappelling_timeout]);
                rappelling_data[playerid][r_rappelling_timeout] = -1;
            }
            if (IsValidDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint])) {
                DestroyDynamicCP(rappelling_data[playerid][r_rappelling_checkpoint]);
                rappelling_data[playerid][r_rappelling_checkpoint] = INVALID_STREAMER_ID;
            }
            return Y_HOOKS_BREAK_RETURN_1;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (newkeys & KEY_HANDBRAKE) {
        if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_NORMAL) {
            if (GetPlayerState(playerid) == PLAYER_STATE_PASSENGER) {
                new vehicleid = GetPlayerVehicleID(playerid);
                if (vehicleid != INVALID_VEHICLE_ID) {
                    if (Vehicle_IsHelicopter(vehicleid)) {
                        new Float:p_altitude;
                        GetPlayerAltitude(playerid, p_altitude);
                        if (p_altitude >= 10.0 && p_altitude < 75.0) {
                            start_player_rappeling(playerid, vehicleid);
                            return Y_HOOKS_CONTINUE_RETURN_1;
                        }
                    }
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}