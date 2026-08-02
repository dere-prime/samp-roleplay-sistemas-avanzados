#include <YSI_Coding\y_hooks>

enum E_Airports_Areas {
    airport_area_AREAID,
    Float:airport_area_MIN_X,
    Float:airport_area_MIN_Y,
    Float:airport_area_MAX_X,
    Float:airport_area_MAX_Y
};

new Airports_Areas[][E_Airports_Areas] = {
    {INVALID_STREAMER_ID, 881.229003, -2811.327148, 2625.628417, -2188.754638}, //LS
    {INVALID_STREAMER_ID, -2299.068603, -1045.947998, -670.923095, 1045.822631}, //SF
    {INVALID_STREAMER_ID, 1234.226196, 573.200256, 1758.641479, 2468.117675}, //LV
    {INVALID_STREAMER_ID, -475.365783, 2198.848144, 870.382324, 2850.787109} //AA
};

new
    bool:pAirSpeedo[MAX_PLAYERS],
    bool:pInAirport[MAX_PLAYERS],
    pAirSpeedoTimer[MAX_PLAYERS] = {-1, ...};

forward OnAirSpeedoRequestUpdate(playerid, vehicleid);

hook OnScriptInit() {
    for(new i = 0; i < sizeof Airports_Areas; i ++) {
        Airports_Areas[i][airport_area_AREAID] = CreateDynamicRectangle(Airports_Areas[i][airport_area_MIN_X], Airports_Areas[i][airport_area_MIN_Y], Airports_Areas[i][airport_area_MAX_X], Airports_Areas[i][airport_area_MAX_Y], 0, 0);
        Streamer_SetArrayData(STREAMER_TYPE_AREA, Airports_Areas[i][airport_area_AREAID], E_STREAMER_EXTRA_ID, { AREA_TYPE_AIRPORT });
    }
}

hook OnPlayerConnect(playerid) {
    pInAirport[playerid] = false;
}

hook OnPlayerDisconnect(playerid, reason) {
    DestroyPlayerAirSpeedo(playerid);
}

hook OnPlayerStateChange(playerid, newstate, oldstate) {
    if(newstate == PLAYER_STATE_DRIVER) {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(!VEHICLE_INFO[GetVehicleModel(vehicleid) - 400][vehicle_info_NORMAL_SPEEDO] && (Vehicle_IsPlane(vehicleid) || Vehicle_IsHelicopter(vehicleid))) {
            CreatePlayerAirSpeedo(playerid, vehicleid);
        }
    } else if(oldstate == PLAYER_STATE_DRIVER && pAirSpeedo[playerid]) {
        DestroyPlayerAirSpeedo(playerid);
    }
}

hook OnPlayerEnterDynArea(playerid, areaid) {
	new info[2];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);
    if(info[0] == AREA_TYPE_AIRPORT) {
        pInAirport[playerid] = true;
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerLeaveDynArea(playerid, areaid) {
	new info[2];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);
    if(info[0] == AREA_TYPE_AIRPORT) {
        pInAirport[playerid] = false;
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

public OnAirSpeedoRequestUpdate(playerid, vehicleid) {
    new Float:vel = GetVehicleAirSpeed(vehicleid);
	va_PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][14], "%.1f", GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][10], "%03d", floatround(vel));
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][9], "KTS");

    for (new i = 0; i < 9; i++) {
        PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][i], 0xB02EFFFF);
    }
    
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][10], 0xC666FFFF);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][11], -1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][13], -1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][14], -1);

    if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE]) {
        GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] -= floatmul(floatdiv(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS], 200), vel * 0.0004);
        if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] <= 0.1) {
            PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY] = gettime() + 15;
            GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = 0.0;
            GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
            UpdateVehicleParams(vehicleid);
            SendPlayerClientMessage(playerid, "{cccccc}El vehículo se ha quedado sin gasolina...");
        }
    }
}

CreatePlayerAirSpeedo(playerid, vehicleid) {
    DestroyPlayerAirSpeedo(playerid);

    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][5], -1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][7], -1);

    for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
        PlayerTextDrawShow(playerid, pSpeedoTd[playerid][i]);
    }

    pAirSpeedoTimer[playerid] = SetTimerEx("OnAirSpeedoRequestUpdate", 350, true, "ii", playerid, vehicleid);
    pAirSpeedo[playerid] = true;
}

DestroyPlayerAirSpeedo(playerid) {
    for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
        PlayerTextDrawHide(playerid, pSpeedoTd[playerid][i]);
    }

    if(pAirSpeedoTimer[playerid] != -1) {
        KillTimer(pAirSpeedoTimer[playerid]);
        pAirSpeedoTimer[playerid] = -1;
    }

    pAirSpeedo[playerid] = false;
}

stock Float:GetVehicleAirSpeed(vehicleid) {
    new Float:vel = GetVehicleSpeed(vehicleid);
	return vel * 1.2; //vel * 0.54;
}