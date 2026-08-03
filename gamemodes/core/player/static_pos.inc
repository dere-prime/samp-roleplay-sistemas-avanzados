/* 

    Soluccion alternativa al TogglePlayerControllable
        Para los android

*/

#include <YSI_Coding\y_hooks>

new
    bool: p_ActiveStaticPos[MAX_PLAYERS],
    p_StaticPos_Timer[MAX_PLAYERS],
    Float: p_StaticPos_Coords[MAX_PLAYERS][4];

hook OnPlayerConnect(playerid) {
    p_ActiveStaticPos[playerid] = false;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

IsPlayerActiveStaticPos(const playerid) { return p_ActiveStaticPos[playerid]; }

SetPlayerStaticPos(const playerid, Float: x, Float: y, Float: z, Float: angle) {
    p_StaticPos_Coords[playerid][0] = x;
    p_StaticPos_Coords[playerid][1] = y;
    p_StaticPos_Coords[playerid][2] = z;
    p_StaticPos_Coords[playerid][3] = angle;
    return p_ActiveStaticPos[playerid];
}

forward SetPlayerActiveStaticPos(const playerid);
public SetPlayerActiveStaticPos(const playerid) {
    GetPlayerPos(playerid, p_StaticPos_Coords[playerid][0], p_StaticPos_Coords[playerid][1], p_StaticPos_Coords[playerid][2]);
    GetPlayerFacingAngle(playerid, p_StaticPos_Coords[playerid][3]);
    p_ActiveStaticPos[playerid] = true;
    UpdateSetPlayerEquippedWeapons(playerid);
    KillTimer(p_StaticPos_Timer[playerid]);
    return p_ActiveStaticPos[playerid];
}

setGetPlayerStaticPos(const playerid) {
    GetPlayerPos(playerid, p_StaticPos_Coords[playerid][0], p_StaticPos_Coords[playerid][1], p_StaticPos_Coords[playerid][2]);
    GetPlayerFacingAngle(playerid, p_StaticPos_Coords[playerid][3]);
    return p_ActiveStaticPos[playerid];
}

enablePlayerActiveStaticPos(const playerid) { 
    p_ActiveStaticPos[playerid] = true;
    UpdateSetPlayerEquippedWeapons(playerid);
    KillTimer(p_StaticPos_Timer[playerid]);
    return SetPlayerActiveStaticPos(playerid); 
}

DisablePlayerActiveStaticPos(const playerid) {
    p_ActiveStaticPos[playerid] = false;
    UpdateSetPlayerEquippedWeapons(playerid);
    return p_ActiveStaticPos[playerid];
}

hook OnPlayerUpdate(playerid) {
    if (p_ActiveStaticPos[playerid]) {
        if (GetPlayerDistanceFromPoint(playerid, p_StaticPos_Coords[playerid][0], p_StaticPos_Coords[playerid][1], p_StaticPos_Coords[playerid][2]) > 4.0) {
            PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 2;
            PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 2;
            PLAYER_AC_INFO[playerid][CHEAT_UNOCCUPIED_VEHICLE_TP][p_ac_info_IMMUNITY] = gettime() + 3;
            SetPlayerPos(playerid, p_StaticPos_Coords[playerid][0], p_StaticPos_Coords[playerid][1], p_StaticPos_Coords[playerid][2]);
            SetPlayerFacingAngle(playerid, p_StaticPos_Coords[playerid][3]);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
