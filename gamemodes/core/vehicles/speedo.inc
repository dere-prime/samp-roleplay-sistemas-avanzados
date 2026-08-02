#include <YSI_Coding\y_hooks>

new
    pLastFuelSubtraction[MAX_PLAYERS],
	bool:pSpeedoShown[MAX_PLAYERS],
    pSpeedoTimer[MAX_PLAYERS] = {-1, ...},
    PlayerText:pSpeedoTd[MAX_PLAYERS][16];

hook OnPlayerConnect(playerid) {
    pSpeedoTd[playerid][0] = CreatePlayerTextDraw(playerid, 524.000, 400.000, "1");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][0], 0.000, 1.800);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][0], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][0], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][0], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][0], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][0], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][0], 1);

    pSpeedoTd[playerid][1] = CreatePlayerTextDraw(playerid, 534.000, 400.000, "2");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][1], 0.000, 1.800);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][1], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][1], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][1], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][1], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][1], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][1], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][1], 1);

    pSpeedoTd[playerid][2] = CreatePlayerTextDraw(playerid, 544.000, 400.000, "3");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][2], 0.000, 1.800);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][2], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][2], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][2], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][2], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][2], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][2], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][2], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][2], 1);

    pSpeedoTd[playerid][3] = CreatePlayerTextDraw(playerid, 554.000, 400.000, "4");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][3], 0.000, 1.800);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][3], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][3], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][3], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][3], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][3], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][3], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][3], 1);

    pSpeedoTd[playerid][4] = CreatePlayerTextDraw(playerid, 564.000, 392.000, "5");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][4], 0.000, 2.699);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][4], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][4], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][4], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][4], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][4], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][4], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][4], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][4], 1);

    pSpeedoTd[playerid][5] = CreatePlayerTextDraw(playerid, 574.000, 392.000, "6");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][5], 0.000, 2.699);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][5], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][5], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][5], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][5], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][5], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][5], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][5], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][5], 1);

    pSpeedoTd[playerid][6] = CreatePlayerTextDraw(playerid, 584.000, 392.000, "7");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][6], 0.000, 2.699);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][6], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][6], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][6], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][6], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][6], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][6], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][6], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][6], 1);

    pSpeedoTd[playerid][7] = CreatePlayerTextDraw(playerid, 594.000, 392.000, "8");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][7], 0.000, 2.699);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][7], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][7], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][7], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][7], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][7], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][7], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][7], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][7], 1);

    pSpeedoTd[playerid][8] = CreatePlayerTextDraw(playerid, 604.000, 386.000, "9");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][8], 0.000, 3.399);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][8], 0.000, 6.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][8], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][8], 1768516095);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][8], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][8], 1768516095);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][8], 1768516095);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][8], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][8], 1);

    pSpeedoTd[playerid][9] = CreatePlayerTextDraw(playerid, 599.000, 375.000, "KM/H");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][9], 0.150, 0.800);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][9], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][9], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][9], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][9], 2);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][9], 1);

    pSpeedoTd[playerid][10] = CreatePlayerTextDraw(playerid, 597.000, 361.000, "000");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][10], 0.409, 3.400);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][10], 3);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][10], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][10], 1);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][10], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][10], 2);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][10], 1);

    pSpeedoTd[playerid][11] = CreatePlayerTextDraw(playerid, 525.000, 386.000, "_");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][11], 0.129, 1.000);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][11], 0.000, 4.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][11], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][11], -1);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][11], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][11], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][11], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][11], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][11], -1);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][11], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][11], 1);

    pSpeedoTd[playerid][12] = CreatePlayerTextDraw(playerid, 525.000, 387.600, "_");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][12], 0.129, 0.199);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][12], 0.000, 2.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][12], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][12], -1);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][12], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][12], 255);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][12], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][12], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][12], -1);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][12], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][12], 1);

    pSpeedoTd[playerid][13] = CreatePlayerTextDraw(playerid, 528.000, 383.000, ")");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][13], 0.300, 1.299);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][13], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][13], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][13], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][13], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][13], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][13], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][13], 1);

    pSpeedoTd[playerid][14] = CreatePlayerTextDraw(playerid, 533.000, 379.000, "---");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][14], 0.250, 2.200);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][14], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][14], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][14], 1);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][14], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][14], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][14], 2);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][14], 1);

    pSpeedoTd[playerid][15] = CreatePlayerTextDraw(playerid, 552.000, 370.000, "HUD:radar_modGarage");
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][15], 10.000, 10.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][15], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][15], 1684301055);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][15], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][15], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][15], 255);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][15], 4);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][15], 1);
}
hook OnPlayerDisconnect(playerid, reason) {
    for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
        PlayerTextDrawDestroy(playerid, pSpeedoTd[playerid][i]);
    }

	pSpeedoShown[playerid] = false;
    pLastFuelSubtraction[playerid] = 0;
    if(pSpeedoTimer[playerid] != -1) {
	    KillTimer(pSpeedoTimer[playerid]);
        pSpeedoTimer[playerid] = -1;
    }
}
hook OnPlayerStateChange(playerid, newstate, oldstate) {
    if(newstate == PLAYER_STATE_DRIVER) {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(VEHICLE_INFO[GetVehicleModel(vehicleid) - 400][vehicle_info_NORMAL_SPEEDO] && !PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE] && mission_info[playerid][mission_step] != MISSION_STEP_CINEMATIC) ShowPlayerSpeedoMeter(playerid);
    }
    else if(oldstate == PLAYER_STATE_DRIVER) {
        HidePlayerSpeedoMeter(playerid);
    }
}

forward ShowPlayerSpeedoMeter(playerid);
public ShowPlayerSpeedoMeter(playerid) {
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 0;
	new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
	if(!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return 0;
    if(pSpeedoTimer[playerid] != -1) {
	    KillTimer(pSpeedoTimer[playerid]);
        pSpeedoTimer[playerid] = -1;
    }
	
	va_PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][14], "%.1f", GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][10], "000");
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][9], "KM/H");
	
	for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
		PlayerTextDrawShow(playerid, pSpeedoTd[playerid][i]);
	}
	pSpeedoShown[playerid] = true;

	pLastFuelSubtraction[playerid] = gettime();
	pSpeedoTimer[playerid] = SetTimerEx("UpdatePlayerSpeedo", 250, true, "iif", playerid, vehicleid, VEHICLE_INFO[modelid - 400][vehicle_info_MAX_VEL]);
	return 1;
}

HidePlayerSpeedoMeter(playerid) {
    pLastFuelSubtraction[playerid] = 0;
	pSpeedoShown[playerid] = false;
	if(pSpeedoTimer[playerid] != -1) {
	    KillTimer(pSpeedoTimer[playerid]);
        pSpeedoTimer[playerid] = -1;
    }
	for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
        PlayerTextDrawHide(playerid, pSpeedoTd[playerid][i]);
    }
	return 1;
}

temporarilyHidePlayerSpeedoMetr(playerid, time_ms) {
    for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
        PlayerTextDrawHide(playerid, pSpeedoTd[playerid][i]);
    }
    pSpeedoShown[playerid] = false;

    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][23]);
    PLAYER_TEMP[playerid][pt_TIMERS][23] = SetTimerEx("ShowPlayerSpeedoMeter", time_ms, false, "i", playerid);
    return 1;
}

updateSpeedoBars(const playerid, Float:current_vel, Float:max_vel) {
    new bars = floatround(((current_vel) / (max_vel / 9.0) * 1.10), floatround_floor);

    if (bars > 9) bars = 9;
    if (bars < 0) bars = 0;

    for (new i = 0; i < 9; i++) {
        PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][i],
            (i < bars) ? 0xB02EFFFF : 1768516095);
    }
    return 1;
}

forward UpdatePlayerSpeedo(playerid, vehicleid, Float:maxvel);
public UpdatePlayerSpeedo(playerid, vehicleid, Float:maxvel) {
    if (!pSpeedoShown[playerid]) {
        for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
            PlayerTextDrawShow(playerid, pSpeedoTd[playerid][i]);
        }
    }

	if(vehicleid != GetPlayerVehicleID(playerid)) {
		HidePlayerSpeedoMeter(playerid);
		ShowPlayerSpeedoMeter(playerid);
		
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_DRIVER] = INVALID_PLAYER_ID;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_LAST_DRIVER] = playerid;
		GLOBAL_VEHICLES[vehicleid][gb_vehicle_OCCUPIED] = false;
		PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = GetPlayerVehicleID(playerid);
		GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_OCCUPIED] = true;
		return 0;
	}
	
	new Float:vel = GetVehicleSpeed(vehicleid);
	
	if(ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Enabled]) {
		if(gettime() > PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_IMMUNITY]) {
			if(vel > maxvel + 100.0) {
				if(!ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_SPEED_HACK);
				else
				{
					if(gettime() - PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_LAST_DETECTION] = gettime();
					if(PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_SPEED_HACK][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_VEHICLE_SPEED_HACK][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_VEHICLE_SPEED_HACK);
				}
			}
		}
	}
	
	if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] == VEHICLE_STATE_NORMAL) {
		GetVehicleHealth(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH]);
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] < MIN_VEHICLE_HEALTH) {	
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_STATE] = VEHICLE_STATE_DAMAGED;
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] = MIN_VEHICLE_HEALTH;
			SetVehicleHealthEx(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH], playerid);
				
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
			UpdateVehicleParams(vehicleid);
			SendClientMessage(playerid, -1, "{CCCCCC}El motor del vehículo esta totalmente inutilizable.");
		}
	}
	if(gettime() > pLastFuelSubtraction[playerid] + 5) {
		if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE]) {
			GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] -= floatmul(floatdiv(vel, maxvel), 0.1);
			
			if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] <= 0.1) {
				PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY] = gettime() + 15;
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = 0.0;
				GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
				UpdateVehicleParams(vehicleid);
				
				SendPlayerClientMessage(playerid, "{CCCCCC}El vehículo ya no tiene combustible, el motor se ha apagado.");
			}
		}
		pLastFuelSubtraction[playerid] = gettime();
	}
    
    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] <= 0.1) PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][14], "---");
	else PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][14], "%.1f", GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);

    PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][10], "%03d", floatround(vel));

    updateSpeedoBars(playerid, vel, maxvel);

    if(GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] <= 500.0) {
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][15], -1);
    } else {
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][15], 1684301055);
    }

    new Float:half_fuel = floatdiv(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS], 2.0);
    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] <= half_fuel) {
        PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][11], 0xF54927FF);
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][13], 0xF54927FF);
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][14], 0xF54927FF);
    } else {
        PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][11], -1);
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][13], -1);
    }

    if (vel >= (maxvel * 0.90)) {
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][10], 0xC666FFFF);
    } else {
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][10], -1);
    }

	GetVehiclePos(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][0], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2]);
	return 1;
}

stock Float:GetVehicleSpeed(vehicleid) {
    new Float:vx, Float:vy, Float:vz;
    GetVehicleVelocity(vehicleid, vx, vy, vz);
	new Float:vel = floatmul(floatsqroot(floatadd(floatadd(floatpower(vx, 2), floatpower(vy, 2)),  floatpower(vz, 2))), 181.5);
	return vel;
}