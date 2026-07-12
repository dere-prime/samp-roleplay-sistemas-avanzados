/*

    Taxi Work
        Prime Roleplay

*/
#include <YSI_Coding\y_hooks>

/* RECEPCIONIST */
static taxi_recepcionist_actor = INVALID_ACTOR_ID;
static taxi_recepcionist_timer[MAX_PLAYERS] = -1;
hook OnScriptInit() {
    taxi_recepcionist_actor = CreateDynamicActor(9, -1740.2566, 471.1222, 2166.2393, 88.8870, .worldid = 3, .interiorid = 3);
    CreateDynamic3DTextLabel("Recepcionista", 0xCCCCCCCC, -1740.2566, 471.1222, 2166.2393 + 1.1, 10.0, .testlos = true, .worldid = 3, .interiorid = 3);
    CreateDynamic3DTextLabel("Acercate para hablar con la recepcionista", 0xCCCCCCCC, -1742.5781, 471.2834, 2166.2485, 5.0, .testlos = true, .worldid = 3, .interiorid = 3);
    new info[3], talk_pickup = CreateDynamicPickup(1314, 1, -1742.5781, 471.2834, 2166.2485, 3, 3);
    info[0] = PICKUP_TYPE_TAXI_RECEPCIONIST;
    Streamer_SetArrayData(STREAMER_TYPE_PICKUP, talk_pickup, E_STREAMER_EXTRA_ID, info);
}
hook OnPlayerPickUpDynPickup(playerid, pickupid) {
    if (pickupid == PICKUP_TYPE_NONE) return Y_HOOKS_CONTINUE_RETURN_1;
	if (PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] > gettime()) return Y_HOOKS_BREAK_RETURN_0;
	if (Dialog_IsOpen(playerid)) return Y_HOOKS_BREAK_RETURN_0;
	new info[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, info);
	if (info[0] == PICKUP_TYPE_TAXI_RECEPCIONIST) {
        if (taxi_recepcionist_timer[playerid] != -1) {
            KillTimer(taxi_recepcionist_timer[playerid]);
            taxi_recepcionist_timer[playerid] = -1;
        }
    	TogglePlayerControllableEx(playerid, false);
        ApplyDynamicActorAnimation(taxi_recepcionist_actor, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);
        ApplyDynamicActorAnimation(taxi_recepcionist_actor, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);
        if (IsPlayerWork(playerid, WORK_TAXI)) {
            ShowPlayerMissionDialog(playerid, 9, "Recepcionista", "Bienvenido de nuevo, ¿Que te trae por aqui?~n~Bueno, no importa, te mostrare el menu de ayuda, dame unos segundos.");
            taxi_recepcionist_timer[playerid] = SetTimerEx("TaxistHelpTimer", 4000, false, "ii", playerid, true);
        } else {
            ShowPlayerMissionDialog(playerid, 9, "Recepcionista", "Bienvenido a unity station, si buscas el trabajo de taxista, esta pasando la puerta que esta detras de mi. Suerte!");
            taxi_recepcionist_timer[playerid] = SetTimerEx("TaxistHelpTimer", 4000, false, "ii", playerid, false);
        }
        PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 10;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
forward TaxistHelpTimer(playerid, bool: mode);
public TaxistHelpTimer(playerid, bool: mode) {
    HidePlayerMissionDialog(playerid, false);
    TogglePlayerControllableEx(playerid, true);
    if (mode) {
        new dialog[1800];
        GetWorkInfo(WORK_TAXI, dialog);
        Dialog_Message(playerid, DIALOG_STYLE_MSGBOX, "Ayuda - Taxista", dialog, "Cerrar");
    }
    return 1;
}

/* GLOBAL */
hook OnPlayerConnect(playerid) {
    if (taxi_recepcionist_timer[playerid] != -1) {
        KillTimer(taxi_recepcionist_timer[playerid]);
        taxi_recepcionist_timer[playerid] = -1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid) {
    if (taxi_recepcionist_timer[playerid] != -1) {
        KillTimer(taxi_recepcionist_timer[playerid]);
        taxi_recepcionist_timer[playerid] = -1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

/* TAXI PLAYER MARKERS */
SendAlertToTaxiDrivers(playerid) {
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	
	foreach (new i : Player) {
		if (IsPlayerConnected(i)) {
			if (PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL) {
				if (i == playerid) continue;
				if (GetPlayerState(i) != PLAYER_STATE_DRIVER) continue;
				if (!PLAYER_WORKS[i][WORK_TAXI][pwork_SET]) continue;
				if (PLAYER_TEMP[i][pt_WORKING_IN] != WORK_TAXI) continue;
				
				SetPlayerMarkerForPlayer(i, playerid, 0xf2f224FF);
				SendClientMessageEx(i, -1, "Hay un nuevo cliente solicitando un taxi, distancia: {"#PRIMARY_COLOR"}%.2f Km.", (GetPlayerDistanceFromPoint(i, pos[0], pos[1], pos[2]) * 0.01));
			}
		}
	}
	return 1;
}
DisablePlayerTaxiMark(playerid) {
	foreach (new i : Player) {
		if (IsPlayerConnected(i)) {
			if (PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL) {
				if (i == playerid) continue;
				if (GetPlayerState(i) != PLAYER_STATE_DRIVER) continue;
				if (!PLAYER_WORKS[i][WORK_TAXI][pwork_SET]) continue;
				if (PLAYER_TEMP[i][pt_WORKING_IN] != WORK_TAXI) continue;
				
				SetPlayerMarkerForPlayer(i, playerid, PLAYER_COLOR);
			}
		}
	}
	return 1;
}
SetTaxiPlayerMarkers(playerid) {
	foreach (new i : Player) {
		if (IsPlayerConnected(i)) {
			if (PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL) {
				if (PLAYER_TEMP[i][pt_WANT_TAXI]) {
					SetPlayerMarkerForPlayer(playerid, i, 0xf2f224FF);
				}
			}
		}
	}
	return 1;
}

/* ALL TAXIMETER CODE */
ResetVehicleTaxiMeter(vehicleid) {
	KillTimer(TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_TIMER]);
	TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED] = false;
	TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_PAYMENT] = 0;
	TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_X] = 0.0;
	TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_Y] = 0.0;
	TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_OLD_Z] = 0.0;
	TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_DISTANCE] = 0.0;
	return 1;
}

ShowPlayerTaxiMeter(playerid) {
	PLAYER_TEMP[playerid][pt_TAXI_METER_ENABLED] = true;
	Dialog_Show(playerid, Dialog:VehicleTaximeterMenu);
	return 1;
}

DialogCreate:VehicleTaximeterMenu(playerid) {
	new dialog[300];

	strcatf(dialog, "{ffffff}Precio por kilometro\t{"#YELLOW_COLOR"}%s$\n", number_format_thousand(TAXI_METER_VEHICLE[ GetPlayerVehicleID(playerid) ][veh_taxi_meter_PRICE]));
	strcatf(dialog, "{ffffff}Distancia recorrida\t{"#YELLOW_COLOR"}%.2fkm\n", TAXI_METER_VEHICLE[ GetPlayerVehicleID(playerid) ][veh_taxi_meter_DISTANCE]);
	strcatf(dialog, "{ffffff}Dinero a pagar\t{"#YELLOW_COLOR"}%s$\n", number_format_thousand(TAXI_METER_VEHICLE[ GetPlayerVehicleID(playerid) ][veh_taxi_meter_PAYMENT]));

	if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
		strcatf(dialog, "{000000}\n");
		strcatf(dialog, "{ffffff}Cambiar tarifa\t{"#YELLOW_COLOR"}Taxista\n");
	}

	Dialog_Open(playerid, Dialog:VehicleTaximeterMenu, DIALOG_STYLE_TABLIST, "{"#YELLOW_COLOR"}·{ffffff}Taxímetro", dialog, "Continuar", "Cancelar");
	return 1;
}
DialogResponse:VehicleTaximeterMenu(playerid, response, listitem, inputtext[]) {
	if (response) {
		switch (listitem) {
			case 4: {
				Dialog_Show(playerid, Dialog:VehicleTaximeterPrice);
			}
			default: Dialog_Show(playerid, Dialog:VehicleTaximeterMenu);
		}
	}
	return 1;
}

DialogCreate:VehicleTaximeterPrice(playerid) {
	Dialog_Open(playerid, Dialog:VehicleTaximeterPrice, DIALOG_STYLE_INPUT, "{"#YELLOW_COLOR"}·{ffffff}Cambiar tarifa", "{ffffff}Introduce el nuevo precio por kilometro:", "Cambiar", "Cancelar");
	return 1;
}
DialogResponse:VehicleTaximeterPrice(playerid, response, listitem, inputtext[]) {
	if (response) {
		if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return ShowPlayerMessage(playerid, "No estás conduciendo.");
		if (!PLAYER_WORKS[playerid][WORK_TAXI][pwork_SET]) return ShowPlayerMessage(playerid, "No eres taxista.");
		if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TAXI) return ShowPlayerMessage(playerid, "No estás de servicio como taxista.");
		
		new vehicleid = GetPlayerVehicleID(playerid);
		if (TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_ENABLED]) return ShowPlayerMessage(playerid, "Solo puedes cambiar la tarifa cuando no haya ningún pasajero.");
		
		if (sscanf(inputtext, "d", inputtext[0])) return ShowPlayerMessage(playerid, "Solo puedes introducir una tarifa entre 0"#P_CURR" y 14"#P_CURR".");
		if (inputtext[0] < 0 || inputtext[0] > 14) return ShowPlayerMessage(playerid, "Solo puedes introducir una tarifa entre 0"#P_CURR" y 14"#P_CURR".");
		
		TAXI_METER_VEHICLE[vehicleid][veh_taxi_meter_PRICE] = inputtext[0];
		Auto_SendPlayerAction(playerid, "ajusta el taxímetro.");
	}
	Dialog_Show(playerid, Dialog:VehicleTaximeterMenu);
	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (newkeys & KEY_YES) {
        if (PI[playerid][pi_STATE] != ROLEPLAY_STATE_CRACK && PI[playerid][pi_STATE] != ROLEPLAY_STATE_JAIL && PI[playerid][pi_STATE] != ROLEPLAY_STATE_ARRESTED) {
            if (GetPlayerState(playerid) == PLAYER_STATE_PASSENGER) {
				if (PLAYER_TEMP[playerid][pt_TAXI_METER_ENABLED]) {
					Dialog_Show(playerid, Dialog:VehicleTaximeterMenu);
					return Y_HOOKS_CONTINUE_RETURN_0;
				}
			}
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

forward UpdateVehicleTaximeter(taxi, driver, passenger);
public UpdateVehicleTaximeter(taxi, driver, passenger) {
	new driver_vehicle = GetPlayerVehicleID(driver), passenger_vehicle = GetPlayerVehicleID(passenger);
	
	if(!IsPlayerConnected(driver))
	{
		SendPlayerClientMessage(passenger, "{"#YELLOW_COLOR"}El taxi te costó %s$.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));		
		GivePlayerCash(passenger, -TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, true);
		
		ResetVehicleTaxiMeter(taxi);
		HidePlayerTaxiMeter(passenger);
		return 1;
	}
	if(!IsPlayerConnected(passenger))
	{		
		PLAYER_WORKS[driver][WORK_TAXI][pwork_LEVEL] ++;
		
		SendPlayerClientMessage(driver, "{"#YELLOW_COLOR"}Has ganado %s$ con este viaje.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		GivePlayerCash(driver, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, false);
		
		ResetVehicleTaxiMeter(taxi);
		
		new new_passenger = GetVehicleFirstPassenger(taxi);
		if(new_passenger != INVALID_PLAYER_ID)
		{
			ShowPlayerTaxiMeter(new_passenger);
			if(PLAYER_TEMP[new_passenger][pt_WANT_TAXI])
			{
				PLAYER_TEMP[new_passenger][pt_WANT_TAXI] = false;
				DisablePlayerTaxiMark(new_passenger);
			}
			
			TAXI_METER_VEHICLE[taxi][veh_taxi_meter_ENABLED] = true;
			GetVehiclePos(taxi, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Z]);
			KillTimer(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_TIMER]);
			TAXI_METER_VEHICLE[taxi][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", taxi, GLOBAL_VEHICLES[taxi][gb_vehicle_DRIVER], new_passenger);
		}
		return 1;
	}
	
	if(driver_vehicle != taxi)
	{
		PLAYER_WORKS[driver][WORK_TAXI][pwork_LEVEL] ++;
		
		SendPlayerClientMessage(driver, "{"#YELLOW_COLOR"}Has ganado %s$ con este viaje.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		SendPlayerClientMessage(passenger, "{"#YELLOW_COLOR"}El taxi te costó %s$.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		
		GivePlayerCash(driver, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, false);
		GivePlayerCash(passenger, -TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, true);
		
		ResetVehicleTaxiMeter(taxi);
		HidePlayerTaxiMeter(driver);
		HidePlayerTaxiMeter(passenger);
		return 1;
	}
	if(passenger_vehicle != taxi)
	{
		PLAYER_WORKS[driver][WORK_TAXI][pwork_LEVEL] ++;
		
		SendPlayerClientMessage(driver, "{"#YELLOW_COLOR"}Has ganado %s$ con este viaje.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		SendPlayerClientMessage(passenger, "{"#YELLOW_COLOR"}El taxi te costó %s$.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		
		GivePlayerCash(driver, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, false);
		GivePlayerCash(passenger, -TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, true);
		
		ResetVehicleTaxiMeter(taxi);
		HidePlayerTaxiMeter(passenger);
		
		new new_passenger = GetVehicleFirstPassenger(taxi);
		if(new_passenger != INVALID_PLAYER_ID)
		{
			ShowPlayerTaxiMeter(new_passenger);
			if(PLAYER_TEMP[new_passenger][pt_WANT_TAXI])
			{
				PLAYER_TEMP[new_passenger][pt_WANT_TAXI] = false;
				DisablePlayerTaxiMark(new_passenger);
			}
			
			TAXI_METER_VEHICLE[taxi][veh_taxi_meter_ENABLED] = true;
			GetVehiclePos(taxi, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Z]);
			KillTimer(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_TIMER]);
			TAXI_METER_VEHICLE[taxi][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", taxi, GLOBAL_VEHICLES[taxi][gb_vehicle_DRIVER], new_passenger);
		}
		return 1;
	}
	
	if(GetPlayerState(driver) != PLAYER_STATE_DRIVER)
	{
		PLAYER_WORKS[driver][WORK_TAXI][pwork_LEVEL] ++;
		
		SendPlayerClientMessage(driver, "{"#YELLOW_COLOR"}Has ganado %s$ con este viaje.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		SendPlayerClientMessage(passenger, "{"#YELLOW_COLOR"}El taxi te costó %s$.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		
		GivePlayerCash(driver, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, false);
		GivePlayerCash(passenger, -TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, true);
		
		ResetVehicleTaxiMeter(taxi);
		HidePlayerTaxiMeter(driver);
		HidePlayerTaxiMeter(passenger);
		return 1;
	}
	if(GetPlayerState(passenger) != PLAYER_STATE_PASSENGER)
	{
		PLAYER_WORKS[driver][WORK_TAXI][pwork_LEVEL] ++;
		
		SendPlayerClientMessage(driver, "{"#YELLOW_COLOR"}Has ganado %s$ con este viaje.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		SendPlayerClientMessage(passenger, "{"#YELLOW_COLOR"}El taxi te costó %s$.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		
		GivePlayerCash(driver, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, false);
		GivePlayerCash(passenger, -TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, true);
		
		ResetVehicleTaxiMeter(taxi);
		HidePlayerTaxiMeter(passenger);
		
		new new_passenger = GetVehicleFirstPassenger(taxi);
		if(new_passenger != INVALID_PLAYER_ID)
		{
			ShowPlayerTaxiMeter(new_passenger);
			if(PLAYER_TEMP[new_passenger][pt_WANT_TAXI])
			{
				PLAYER_TEMP[new_passenger][pt_WANT_TAXI] = false;
				DisablePlayerTaxiMark(new_passenger);
			}
			
			TAXI_METER_VEHICLE[taxi][veh_taxi_meter_ENABLED] = true;
			GetVehiclePos(taxi, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Z]);
			KillTimer(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_TIMER]);
			TAXI_METER_VEHICLE[taxi][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", taxi, GLOBAL_VEHICLES[taxi][gb_vehicle_DRIVER], new_passenger);
		}
		return 1;
	}

	if(PI[passenger][pi_CASH] < TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT])
	{
		PLAYER_WORKS[driver][WORK_TAXI][pwork_LEVEL] ++;
		
		SendPlayerClientMessage(driver, "{"#YELLOW_COLOR"}Has ganado %s$ con este viaje.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		SendClientMessage(driver, -1, "{"#SILVER_COLOR"}El pasajero no tiene suficiente dinero para seguir pagando el viaje.");
		GivePlayerCash(driver, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, false);
		
		SendPlayerClientMessage(passenger, "{"#YELLOW_COLOR"}El taxi te costó %s$.", number_format_thousand(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT]));
		SendClientMessage(passenger, -1, "{"#SILVER_COLOR"}No tienes suficiente dinero para seguir pagando el viaje.");
		GivePlayerCash(passenger, -TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT], true, true);
		if(PI[passenger][pi_CASH] < 0) SetPlayerCash(passenger, 0);
		
		ResetVehicleTaxiMeter(taxi);
		HidePlayerTaxiMeter(passenger);
		
		RemovePlayerFromVehicle(passenger);
		
		new new_passenger = GetVehicleFirstPassenger(taxi);
		if(new_passenger != INVALID_PLAYER_ID && new_passenger != passenger)
		{
			ShowPlayerTaxiMeter(new_passenger);
			if(PLAYER_TEMP[new_passenger][pt_WANT_TAXI])
			{
				PLAYER_TEMP[new_passenger][pt_WANT_TAXI] = false;
				DisablePlayerTaxiMark(new_passenger);
			}
			
			TAXI_METER_VEHICLE[taxi][veh_taxi_meter_ENABLED] = true;
			GetVehiclePos(taxi, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Z]);
			KillTimer(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_TIMER]);
			TAXI_METER_VEHICLE[taxi][veh_taxi_meter_TIMER] = SetTimerEx("UpdateVehicleTaximeter", 4000, true, "iii", taxi, GLOBAL_VEHICLES[taxi][gb_vehicle_DRIVER], new_passenger);
		}
		return 1;
	}
	
	new Float:distance = GetVehicleDistanceFromPoint(taxi, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Z]) * 0.01;

	TAXI_METER_VEHICLE[taxi][veh_taxi_meter_DISTANCE] += distance;
	TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PAYMENT] = TAXI_METER_VEHICLE[taxi][veh_taxi_meter_PRICE] * floatround(TAXI_METER_VEHICLE[taxi][veh_taxi_meter_DISTANCE], floatround_round);
	
	GetVehiclePos(taxi, TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_X], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Y], TAXI_METER_VEHICLE[taxi][veh_taxi_meter_OLD_Z]);
	return 1;
}

HidePlayerTaxiMeter(playerid) {
	PLAYER_TEMP[playerid][pt_TAXI_METER_ENABLED] = false;
	return 1;
}