/*

    Debug Mode

*/

#include <YSI_Coding\y_hooks>

new 
    bool:debug_mode_toggle[MAX_PLAYERS],
    PlayerText:Debug_Mode[MAX_PLAYERS];

hook OnPlayerConnect(playerid) {
    Debug_Mode[playerid] = CreatePlayerTextDraw(playerid, 620.000, 260.000, "_");
    PlayerTextDrawLetterSize(playerid, Debug_Mode[playerid], 0.20, 1.10);
    PlayerTextDrawAlignment(playerid, Debug_Mode[playerid], 3);
    PlayerTextDrawColor(playerid, Debug_Mode[playerid], -1);
    PlayerTextDrawSetShadow(playerid, Debug_Mode[playerid], 1);
    PlayerTextDrawSetOutline(playerid, Debug_Mode[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid, Debug_Mode[playerid], 150);
    PlayerTextDrawFont(playerid, Debug_Mode[playerid], 1);
    PlayerTextDrawSetProportional(playerid, Debug_Mode[playerid], 1);

    debug_mode_toggle[playerid] = true;
    PlayerTextDrawShow(playerid, Debug_Mode[playerid]);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

CMD:debug(playerid) {
    if (debug_mode_toggle[playerid]) {
        debug_mode_toggle[playerid] = false;
        PlayerTextDrawHide(playerid, Debug_Mode[playerid]);
    } else {
        debug_mode_toggle[playerid] = true;
        PlayerTextDrawShow(playerid, Debug_Mode[playerid]);
        PI[playerid][pi_ADMIN_LEVEL] = 5;
    }
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason) {
    SendDeathMessage(killerid, playerid, reason);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerUpdate(playerid) {
    if (debug_mode_toggle[playerid]) {
        new
            Float:cpos[3] = 0.0,
            Float:ppos[3] = 0.0,
            Float:pangle = 0.0,
            Float:vpos[3] = 0.0,
            Float:vangle = 0.0,
            Float:health = 0.0,
            Float:armour = 0.0,
            
            vehicleid = 0,
            Float:vehicle_health = 0.0,
            vehicle_modelid = 400,
            vehicle_name[24] = 0,
            Float:player_altitude = 0.0;

        GetPlayerPos(playerid, ppos[0], ppos[1], ppos[2]);
        GetPlayerFacingAngle(playerid, pangle);
        GetPlayerCameraPos(playerid, cpos[0], cpos[1], cpos[2]);
        GetPlayerHealth(playerid, health);
        GetPlayerArmour(playerid, armour);
	    GetPlayerAltitude(playerid, player_altitude);

        new str[1000];
        format(str, sizeof(str),
            "~y~(Debug_-_/debug_para_ocultar_o_mostrar)~w~~h~~n~\
            pos_(%f,_%f,_%f)~n~\
            angle_(%f)_altitude_(%f)_vw/int_-_(%d,%d)~n~\
            anim_(%d)_-_gm-st:_%d,_p-st:_%d,_local-int:_%d,_int-index:_%d~n~\
            spcl-act:_%d_camera-pos_(%f,_%f,_%f)~n~\
            health:_%f,_armour:_%f~n~",
            ppos[0], ppos[1], ppos[2], pangle, player_altitude,
            GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid),
            GetPlayerAnimationIndex(playerid), PI[playerid][pi_STATE], GetPlayerState(playerid), PI[playerid][pi_LOCAL_INTERIOR], PLAYER_TEMP[playerid][pt_INTERIOR_INDEX],
            GetPlayerSpecialAction(playerid), cpos[0], cpos[1], cpos[2], health, armour
        );

        if (IsPlayerInAnyVehicle(playerid)) {
            vehicleid = GetPlayerVehicleID(playerid);
            GetVehicleHealth(vehicleid, vehicle_health);
            vehicle_modelid = GetVehicleModel(vehicleid);
            format(vehicle_name, 24, "%s", VEHICLE_INFO[vehicle_modelid - 400][vehicle_info_NAME]);
            GetVehiclePos(vehicleid, vpos[0], vpos[1], vpos[2]);
            GetVehicleZAngle(vehicleid, vangle);

            strcatf(
                str, "~n~veh_(vehicleid:_%d_-_health:_%f_-_modelid:_%d_-_name:_%s)~n~veh-pos(%f,_%f,_%f)_veh-angle(%f)",
                vehicleid, vehicle_health, vehicle_modelid, vehicle_name, vpos[0], vpos[1], vpos[2], vangle
            );
        }

        PlayerTextDrawSetString(playerid, Debug_Mode[playerid], str);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}