/*

    Vehicle Interaction Menu
        Impl

*/

#include <YSI_Coding\y_hooks>

/* Interaction Menu */
DialogCreate:VehicleInteractionMenu(playerid) {
    new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
    if (!Vehicle_IsValid(vehicleid)) return 0;
    GetVehicleHealth(vehicleid, GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH]);

    new caption[120], dialog[256];
    format(caption, sizeof(caption), "Vehiculo · {%06x}%s [%d]", (VEHICLE_COLORS[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_COLOR_1] ]  >>> 8), VEHICLE_INFO[modelid-400][vehicle_info_NAME], vehicleid);

    strcatf(dialog, "{f0f0f0}Motor\t%s\n", (GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE]) ? "{"#GREEN_COLOR"}Encendido" : "{"#ERROR_COLOR"}Apagado");
    strcatf(dialog, "{f0f0f0}Luces\t%s\n", (GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_LIGHTS]) ? "{"#GREEN_COLOR"}Encendidas" : "{"#ERROR_COLOR"}Apagadas");
    strcatf(dialog, "{000000}\n");
    strcatf(dialog, "{f0f0f0}Combustible: {"#PRIMARY_COLOR"}%.1f/%.1fL\n", GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]);
    strcatf(dialog, "Estado de el motor: {"#PRIMARY_COLOR"}%.1f\n", GLOBAL_VEHICLES[vehicleid][gb_vehicle_HEALTH]);
    strcatf(dialog, "{000000}\n");
    if (PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != PI[playerid][pi_ID]) {
        strcatf(dialog, "{cccccc}Acceso al vehiculo\t{cccccc}%s\n", (GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS]) ? "Nadie" : "Todos");
    } else {
        strcatf(dialog, "{f0f0f0}Acceso al vehiculo\t%s\n", (GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_DOORS]) ? "{"#ERROR_COLOR"}Nadie" : "{"#GREEN_COLOR"}Todos");
    }
    strcatf(dialog, "{f0f0f0}Sacar a alguien\t-\n");
    strcatf(dialog, "{f0f0f0}Otras opciones\n");

    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK) {
        switch (WORK_VEHICLES[vehicleid][work_vehicle_WORK]) {
            case WORK_TAXI: {
                if (IsPlayerWorking(playerid, WORK_TAXI)) {
                    strcatf(dialog, "{000000}\n");
                    strcatf(dialog, "{f0f0f0}Taxímetro\t{"#YELLOW_COLOR"}Taxista\n");
                }
            }
        }
    }

    Dialog_Open(playerid, Dialog:VehicleInteractionMenu, DIALOG_STYLE_TABLIST, caption, dialog, "Cerrar", "Cerrar");
    return 1;
}
DialogResponse:VehicleInteractionMenu(playerid, response, listitem, inputtext[]) {
    if (response)  {
        switch (listitem) {
            case 0: { PC_EmulateCommand(playerid, "/motor"); Dialog_Show(playerid, Dialog:VehicleInteractionMenu); }
            case 1: { PC_EmulateCommand(playerid, "/luces"); Dialog_Show(playerid, Dialog:VehicleInteractionMenu); }
            case 6: { PC_EmulateCommand(playerid, "/seguro"); Dialog_Show(playerid, Dialog:VehicleInteractionMenu); }
            case 7: { Dialog_Show(playerid, Dialog:VehicleInteractionKick); }
            case 8: { Dialog_Show(playerid, Dialog:InteractionMenu); }
            case 10: {
                if (GLOBAL_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK) {
                    switch (WORK_VEHICLES[ PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] ][work_vehicle_WORK]) {
                        case WORK_TAXI: {
                            if (IsPlayerWorking(playerid, WORK_TAXI)) {
                                Dialog_Show(playerid, Dialog:VehicleTaximeterMenu);
                            }
                        }
                    }
                }
            }
            default: { Dialog_Show(playerid, Dialog:VehicleInteractionMenu); }
        }
    }
    return 1;
}

/* Kick player from vehicle */
DialogCreate:VehicleInteractionKick(playerid) {
    new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
    if (!Vehicle_IsValid(vehicleid)) return 0;

    new caption[120];
    format(caption, sizeof(caption), "Vehiculo · {%06x}%s [%d]", (VEHICLE_COLORS[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_COLOR_1] ]  >>> 8), VEHICLE_INFO[modelid-400][vehicle_info_NAME], vehicleid);
    Dialog_Open(playerid, Dialog:VehicleInteractionKick, DIALOG_STYLE_INPUT, caption, "Escribe la {"#SECONDARY_COLOR"}ID de el jugador{f0f0f0} que esta en el vehiculo:", "Sacar", "Cerrar");
    return 1;
}
DialogResponse:VehicleInteractionKick(playerid, response, listitem, inputtext[]) {
    Dialog_Show(playerid, Dialog:VehicleInteractionMenu);
    if (response) {
        new vehicleid = GetPlayerVehicleID(playerid);
        if (!Vehicle_IsValid(vehicleid)) return 0;

        if (sscanf(inputtext, "u", inputtext[0])) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/echar [PlayerID/Nombre]");
        if (!IsPlayerConnected(inputtext[0])) return ShowPlayerMessage(playerid, "El jugador no está conectado.");
        if (playerid == inputtext[0]) return ShowPlayerMessage(playerid, "No te eches a ti mismo.");

		if (PLAYER_VEHICLES[vehicleid][player_vehicle_VALID]) {
            if (PLAYER_VEHICLES[vehicleid][player_vehicle_OWNER_ID] != PI[playerid][pi_ID]) return SendPlayerClientMessage(playerid, "Este no es tú vehículo.");
            if (GetPlayerVehicleID(inputtext[0]) != vehicleid) return SendPlayerClientMessage(playerid, "Este jugador no está en tu vehículo.");
        }
        else {
            SendPlayerClientMessage(playerid, "Este no es tú vehículo.");
            return 0;
        }
		
		RemovePlayerFromVehicle(inputtext[0]);
		Auto_SendPlayerAction(playerid, "ha echado a %s de su vehículo.", PLAYER_TEMP[ inputtext[0] ][pt_NAME]);
    }
    return 1;
}

/* HOOKS */
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (newkeys & KEY_YES) {
        if (PI[playerid][pi_STATE] != ROLEPLAY_STATE_CRACK && PI[playerid][pi_STATE] != ROLEPLAY_STATE_JAIL && PI[playerid][pi_STATE] != ROLEPLAY_STATE_ARRESTED) {
            if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) Dialog_Show(playerid, Dialog:VehicleInteractionMenu);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
