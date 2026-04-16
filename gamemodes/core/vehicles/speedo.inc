#include <YSI_Coding\y_hooks>

new
    pLastFuelSubtraction[MAX_PLAYERS],
	bool:pSpeedoShown[MAX_PLAYERS],
    pSpeedoTimer[MAX_PLAYERS] = {-1, ...},
    PlayerText:pSpeedoTd[MAX_PLAYERS][9];

hook OnPlayerConnect(playerid) {
    pSpeedoTd[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 420.000, "_");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][0], 0.000, 1.799);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][0], 0.000, 85.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][0], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][0], -1);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][0], 255);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][0], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][0], 1);

    pSpeedoTd[playerid][1] = CreatePlayerTextDraw(playerid, 275.000, 422.000, "000_KM/H");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][1], 0.150, 1.100);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][1], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][1], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][1], 2);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][1], 1);

    pSpeedoTd[playerid][2] = CreatePlayerTextDraw(playerid, 352.000, 422.000, "0.0");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][2], 0.150, 1.100);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][2], 3);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][2], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][2], 2);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][2], 1);

    pSpeedoTd[playerid][3] = CreatePlayerTextDraw(playerid, 350.000, 413.600, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][3], 25.000, 29.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][3], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][3], 255);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][3], 255);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][3], 1);

    pSpeedoTd[playerid][4] = CreatePlayerTextDraw(playerid, 265.000, 413.600, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][4], 25.000, 29.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][4], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][4], 255);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][4], 255);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][4], 1);

    pSpeedoTd[playerid][5] = CreatePlayerTextDraw(playerid, 358.000, 424.000, "_");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][5], 0.000, 0.799);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][5], 0.000, 3.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][5], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][5], -1);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][5], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][5], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][5], 1);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][5], 1);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][5], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][5], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][5], 1);

    pSpeedoTd[playerid][6] = CreatePlayerTextDraw(playerid, 358.000, 425.000, "_");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][6], 0.000, 0.099);
    PlayerTextDrawTextSize(playerid, pSpeedoTd[playerid][6], 0.000, 2.000);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][6], 2);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][6], -1);
    PlayerTextDrawUseBox(playerid, pSpeedoTd[playerid][6], 1);
    PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][6], 255);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][6], 1);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][6], 1);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][6], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][6], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][6], 1);

    pSpeedoTd[playerid][7] = CreatePlayerTextDraw(playerid, 361.000, 421.000, ")");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][7], 0.230, 1.099);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][7], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][7], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][7], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][7], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][7], 1);

    pSpeedoTd[playerid][8] = CreatePlayerTextDraw(playerid, 320.000, 421.000, "-");
    PlayerTextDrawLetterSize(playerid, pSpeedoTd[playerid][8], 0.230, 1.099);
    PlayerTextDrawAlignment(playerid, pSpeedoTd[playerid][8], 1);
    PlayerTextDrawColor(playerid, pSpeedoTd[playerid][8], -1);
    PlayerTextDrawSetShadow(playerid, pSpeedoTd[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, pSpeedoTd[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, pSpeedoTd[playerid][8], 150);
    PlayerTextDrawFont(playerid, pSpeedoTd[playerid][8], 1);
    PlayerTextDrawSetProportional(playerid, pSpeedoTd[playerid][8], 1);
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
        if(VEHICLE_INFO[GetVehicleModel(vehicleid) - 400][vehicle_info_NORMAL_SPEEDO] && !PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE]) ShowPlayerSpeedoMeter(playerid);
    }
    else if(oldstate == PLAYER_STATE_DRIVER) {
        HidePlayerSpeedoMeter(playerid);
    }
}

ShowPlayerSpeedoMeter(playerid) {
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 0;
	new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
	if(!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return 0;
    if(pSpeedoTimer[playerid] != -1) {
	    KillTimer(pSpeedoTimer[playerid]);
        pSpeedoTimer[playerid] = -1;
    }
	
	va_PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][2], "%.1f", GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][1], "000_KM/H");
	
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

forward UpdatePlayerSpeedo(playerid, vehicleid, Float:maxvel);
public UpdatePlayerSpeedo(playerid, vehicleid, Float:maxvel) {
	for(new i = 0; i < sizeof pSpeedoTd[]; i ++) {
		PlayerTextDrawShow(playerid, pSpeedoTd[playerid][i]);
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
	
	PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][2], "%.1f", GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);
    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH] <= MIN_VEHICLE_HEALTH) PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][1], "AVERIADO");
	else PlayerTextDrawSetString(playerid, pSpeedoTd[playerid][1], "%03d_KM/H", floatround(vel));

    new Float:half_fuel = floatdiv(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS], 2.0);
    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] <= half_fuel) {
        PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][5], 0xF54927FF);
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][7], 0xF54927FF);
    } else {
        PlayerTextDrawBoxColor(playerid, pSpeedoTd[playerid][5], -1);
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][7], -1);
    }
    if (floatround(vel) >= floatround(maxvel)) {
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][1], 0xA6FFC4FF);
    } else {
        PlayerTextDrawColor(playerid, pSpeedoTd[playerid][1], -1);
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