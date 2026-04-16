/*

    Title: Gas Stations Configuration
    By: Dere.Prime
    Description:
        This file contains the configuration settings for gas stations in the game.
        It defines the locations, fuel prices, and other relevant parameters.

*/
#include <YSI_Coding\y_hooks>

#define FUEL_PRICE_PER_LITER 10 //$ por 1 litro de gasolina
#define GAS_CANISTER_MAX_LITERS 50 //50 litros por bidon
#define GAS_STATION_COLOR "ED1C24"

enum fuel_stations_info_enum
{
	Float:fuel_station_pos_x,
	Float:fuel_station_pos_y,
	Float:fuel_station_pos_z
};

new 
    g_fuel_stations[][fuel_stations_info_enum] = {
        {1942.679443, -1771.374511, 13.390598},
        {654.678710, -570.627807, 16.335937},
        {1384.011962, 457.825012, 19.995258},
        {-1669.027221, 411.541076, 7.179687},
        {-2409.644042, 976.147644, 45.307708},
        {-1328.633666, 2675.913818, 50.062500},
        {612.890563, 1692.938720, 6.992187},
        {-1607.715820, -2717.499023, 48.539062},
        {2146.831542, 2743.855224, 10.820312},
        {2197.993408, 2475.015625, 10.820312},
        {2115.124267, 926.094909, 10.960937},
        {1595.974731, 2193.016845, 10.820312},
        {-1477.559326, 1858.962524, 32.639846},
        {-98.757804, -1172.726440, 2.428447}
    },

    g_vehicle_refueling_timer[MAX_VEHICLES] = {-1, ...};

hook OnScriptInit() {
    for (new i = 0; i != sizeof(g_fuel_stations); i++) {
        new info[3], onfoot_pickup = CreateDynamicPickup(1650, 1, g_fuel_stations[i][fuel_station_pos_x], g_fuel_stations[i][fuel_station_pos_y], g_fuel_stations[i][fuel_station_pos_z], 0, 0), vehicle_pickup = CreateDynamicPickup(0, 14, g_fuel_stations[i][fuel_station_pos_x], g_fuel_stations[i][fuel_station_pos_y], g_fuel_stations[i][fuel_station_pos_z], 0, 0);
        CreateDynamic3DTextLabel("Estacion de combustible\n{ffffff}Acercate para repostar {"#GAS_STATION_COLOR"}gasolina{ffffff} o comprar un bidon\nPrecio: "#FUEL_PRICE_PER_LITER"{"#GREEN_COLOR"}${ffffff}/Litro", 0xED1C24FF, g_fuel_stations[i][fuel_station_pos_x], g_fuel_stations[i][fuel_station_pos_y], g_fuel_stations[i][fuel_station_pos_z] + 1.0, 10.0, .testlos = true);
        info[0] = PICKUP_TYPE_GAS_STATION;
        info[1] = i; // Index
        info[2] = 0;
        Streamer_SetArrayData(STREAMER_TYPE_PICKUP, onfoot_pickup, E_STREAMER_EXTRA_ID, info);
        Streamer_SetArrayData(STREAMER_TYPE_PICKUP, vehicle_pickup, E_STREAMER_EXTRA_ID, info);
    }
}

hook OnPlayerPickUpDynPickup(playerid, pickupid) {
    if (pickupid == PICKUP_TYPE_NONE) return Y_HOOKS_CONTINUE_RETURN_1;

	new info[3], Float:pos[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, info);
	if (info[0] == PICKUP_TYPE_GAS_STATION) {
		Streamer_GetFloatData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_X, pos[0]);
		Streamer_GetFloatData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_Y, pos[1]);
		Streamer_GetFloatData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_Z, pos[2]);

		if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            new vehicleid = GetPlayerVehicleID(playerid);
            if (!Vehicle_IsValid(vehicleid)) return Y_HOOKS_BREAK_RETURN_0;
            if (!VEHICLE_INFO[ GLOBAL_VEHICLES[vehicleid][gb_vehicle_MODELID] - 400 ][vehicle_info_NORMAL_SPEEDO] || GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return Y_HOOKS_BREAK_RETURN_0;
            if (!IsPlayerInRangeOfPoint(playerid, 15.0, pos[0], pos[1], pos[2])) return Y_HOOKS_BREAK_RETURN_0;

            GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
            UpdateVehicleParams(vehicleid);
            SetVehicleVelocity(vehicleid, 0.0, 0.0, 0.0);

            Dialog_Show(playerid, Dialog:GasStationRefuelVehicle);
        } else {
            if (!IsPlayerInRangeOfPoint(playerid, 1.5, pos[0], pos[1], pos[2])) return Y_HOOKS_BREAK_RETURN_0;
            Dialog_Show(playerid, Dialog:GasStationBuyCanister);
        }

		PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 4;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

forward StartRefuelingVehicle(playerid, Float:target_fuel);
public StartRefuelingVehicle(playerid, Float:target_fuel) {
    new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
    if (!Vehicle_IsValid(vehicleid)) return 0;
    if (!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return ShowPlayerMessage(playerid, "No puedes repostar este vehículo.");
    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return ShowPlayerMessage(playerid, "Este vehículo no tiene depósito de gasolina.");
    if (g_vehicle_refueling_timer[vehicleid] != -1) return ShowPlayerMessage(playerid, "El vehículo ya está siendo repostado.");
    if (target_fuel <= 0.0) target_fuel = GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]; // Llenar tanque
    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] >= target_fuel) return ShowPlayerMessage(playerid, "El tanque ya tiene suficiente gasolina.");

	Auto_SendPlayerAction(playerid, "ha empezado a repostar el vehiculo.");
    g_vehicle_refueling_timer[vehicleid] = SetTimerEx("RefuelVehicleTimer", 2500, false, "iif", playerid, vehicleid, target_fuel);
    return 1;
}

forward RefuelVehicleTimer(playerid, vehicleid, Float:target_fuel);
public RefuelVehicleTimer(playerid, vehicleid, Float:target_fuel) {
    KillTimer(g_vehicle_refueling_timer[vehicleid]);
    g_vehicle_refueling_timer[vehicleid] = -1;

    if (!IsPlayerConnected(playerid)) return 0;
    new to_vehicle = GetPlayerVehicleID(playerid);
    if (!Vehicle_IsValid(vehicleid)) return 0;
    if (!Vehicle_IsValid(to_vehicle)) return 0;
    if (vehicleid != to_vehicle) return 0;
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 0;

    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] > GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]) { GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]; }
    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] >= target_fuel) { ShowPlayerMessage(playerid, "El tanque ya ha sido repostado hasta la cantidad deseada."); Auto_SendPlayerAction(playerid, "Ha finalizado de repostar el vehiculo."); return 1;  }

    if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE]) {
        SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}El repostaje ha sido cancelado al encender el motor del vehiculo.");
        Auto_SendPlayerAction(playerid, "ha dejado de repostar el vehiculo.");
        ShowPlayerMessage(playerid, "_");
        return 0;
    }

    if (GivePlayerCash(playerid, -FUEL_PRICE_PER_LITER, true, true)) {
        GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] ++;
        if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] >= GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]) { GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]; }
        ShowPlayerMessage(playerid, "Repostando...~n~~y~Enciende el motor para cancelar.");
    } else {
        ShowPlayerMessage(playerid, "~r~No tienes suficiente dinero para seguir repostando.");
        return 1;
    }

    g_vehicle_refueling_timer[vehicleid] = SetTimerEx("RefuelVehicleTimer", 1000, false, "iif", playerid, vehicleid, target_fuel);
    return 1;
}

DialogCreate:GasStationBuyCanister(playerid) {
    new dialog[180], gas_canister_price = FUEL_PRICE_PER_LITER * GAS_CANISTER_MAX_LITERS;
    format(dialog, sizeof(dialog), "{"#GAS_STATION_COLOR"}·{ffffff}Deseas comprar un {"#GAS_STATION_COLOR"}bidon de gasolina{ffffff} de "#GAS_CANISTER_MAX_LITERS" Litros por %d{"#GREEN_COLOR"}${ffffff}?", gas_canister_price);
    Dialog_Open(playerid, Dialog:GasStationBuyCanister, DIALOG_STYLE_MSGBOX, "{"#GAS_STATION_COLOR"}Estacion de Combustible", dialog, "Comprar", "Cancelar");
    return 1;
}
DialogResponse:GasStationBuyCanister(playerid, response, listitem, inputtext[]) {
    PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 4;
    if (response) {
        new slot = GetPlayerInventorySlot(playerid);
        if (slot == -1) { SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}No tienes espacio en el inventario."); return 1; }

        new gas_canister_price = FUEL_PRICE_PER_LITER * GAS_CANISTER_MAX_LITERS;
        if (GivePlayerCash(playerid, -gas_canister_price, true, true)) {
			SetPlayerInventorySlotData(playerid, slot, INVENTORY_GAS_CANISTER, GAS_CANISTER_MAX_LITERS, 0, 0);
            InsertPlayerInventorySlot(playerid, slot);
            SendPlayerClientMessage(playerid, "{"#GAS_STATION_COLOR"}·{ffffff}Has comprado un {"#GAS_STATION_COLOR"}bidon de gasolina{ffffff} por {"#GREEN_COLOR"}%d${ffffff}.", gas_canister_price);
        } else {
            SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}No tienes suficiente dinero para comprar el bidon de gasolina.");
        }
    }
    return 1;
}

DialogCreate:GasStationRefuelVehicle(playerid) {
    Dialog_Open(playerid, Dialog:GasStationRefuelVehicle, DIALOG_STYLE_TABLIST, "{"#GAS_STATION_COLOR"}Estacion de Combustible", "LLenar tanque\nHasta la mitad\nElegir cantidad\nComprar bidon", "Continuar", "Cancelar");
    return 1;
}
DialogResponse:GasStationRefuelVehicle(playerid, response, listitem, inputtext[]) {
    PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 4;
    if (response) {
        switch (listitem) {
            case 0: {
                new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
                if (!Vehicle_IsValid(vehicleid)) return 0;
	            if (!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return ShowPlayerMessage(playerid, "No puedes repostar este vehículo.");
                if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return ShowPlayerMessage(playerid, "Este vehículo no tiene depósito de gasolina.");

                new Float:amount = floatsub(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]), Float:fuel_cost = FUEL_PRICE_PER_LITER;
                ShowPlayerMessage(playerid, "Vas a ~r~llenar el tanque~w~~h~ de gasolina por %s~g~$~w~~h~.", number_format_thousand(floatround(floatmul(amount, fuel_cost))));
                StartRefuelingVehicle(playerid, 0.0); // Llenar tanque
            }
            case 1: {
                new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
                if (!Vehicle_IsValid(vehicleid)) return 0;
	            if (!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return ShowPlayerMessage(playerid, "No puedes repostar este vehículo.");
                if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return ShowPlayerMessage(playerid, "Este vehículo no tiene depósito de gasolina.");

                new Float:half_fuel = (GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] / 2), Float:fuel_cost = FUEL_PRICE_PER_LITER;
                if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] >= half_fuel) { SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}El tanque ya tiene al menos la mitad de gasolina."); return 1; }

                ShowPlayerMessage(playerid, "Vas a repostar ~r~%.2f litros~w~~h~ de gasolina por %s~g~$~w~~h~.", half_fuel, number_format_thousand(floatround(floatmul(half_fuel, fuel_cost))));
                StartRefuelingVehicle(playerid, half_fuel); //Tanque a la mitad
            }
            case 2: {
                new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
                if (!Vehicle_IsValid(vehicleid)) return 0;
	            if (!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return ShowPlayerMessage(playerid, "No puedes repostar este vehículo.");
                if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return ShowPlayerMessage(playerid, "Este vehículo no tiene depósito de gasolina.");

                Dialog_Show(playerid, Dialog:GasStationRefuelAmount);
            }
            case 3: { Dialog_Show(playerid, Dialog:GasStationBuyCanister); }
        }
    }
    return 1;
}

DialogCreate:GasStationRefuelAmount(playerid) {
    Dialog_Open(playerid, Dialog:GasStationRefuelAmount, DIALOG_STYLE_INPUT, "{"#GAS_STATION_COLOR"}Estacion de Combustible", "{"#GAS_STATION_COLOR"}·{ffffff}¿Cuantos litros deseas repostar? (Ingresa un valor numérico):", "Repostar", "Cancelar");
    return 1;
}
DialogResponse:GasStationRefuelAmount(playerid, response, listitem, inputtext[]) {
    PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 4;
    if (response) {
        new vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
        if (!Vehicle_IsValid(vehicleid)) return 0;
        if (!VEHICLE_INFO[modelid - 400][vehicle_info_NORMAL_SPEEDO]) return ShowPlayerMessage(playerid, "No puedes repostar este vehículo.");
        if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS] <= 0.0) return ShowPlayerMessage(playerid, "Este vehículo no tiene depósito de gasolina.");

        new Float:amount, Float:fuel_cost = FUEL_PRICE_PER_LITER;
        if (sscanf(inputtext, "f", amount)) return ShowPlayerMessage(playerid, "Debes ingresar una cantidad válida de litros para repostar.");
        if (amount <= 0.0) return ShowPlayerMessage(playerid, "Debes ingresar una cantidad válida de litros para repostar.");

        if (amount > GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]) amount = GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS];
        if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] >= amount) { SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}El tanque ya tiene suficiente gasolina."); return 1;  }

        ShowPlayerMessage(playerid, "Vas a repostar ~r~%.2f litros~w~~h~ de gasolina por %s~g~$~w~~h~.", amount, number_format_thousand(floatround(floatmul(amount, fuel_cost))));
        StartRefuelingVehicle(playerid, amount);
    }
    return 1;
}

//Sistema de bidones
DialogCreate:GasCanisterSelectVehicle(playerid) {
    new dialog_body[2000] = "{ffffff}Selecciona el vehiculo al que quieres repostar gasolina desde el bidon\n\n{cccccc}Vehiculos Cercanos:\n", counter = 0;
    for(new i = 0; i != MAX_VEHICLES; i++) {
        if (Vehicle_IsValid(i)) {
            new Float:pos[3];
            GetVehiclePos(i, pos[0], pos[1], pos[2]);
            if(IsPlayerInRangeOfPoint(playerid, 4.0, pos[0], pos[1], pos[2])) {
                new vehicle_modelid = GetVehicleModel(i);
                if (!VEHICLE_INFO[ GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400 ][vehicle_info_NORMAL_SPEEDO] || GLOBAL_VEHICLES[i][gb_vehicle_MAX_GAS] <= 0.0) continue;
                if (PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] != PI[playerid][pi_ID]) strcatf(dialog_body, "- {%06x}[{ffffff}ID: %d{%06x}] %s\n", (VEHICLE_COLORS[ GLOBAL_VEHICLES[i][gb_vehicle_COLOR_1] ]  >>> 8), i, (VEHICLE_COLORS[ GLOBAL_VEHICLES[i][gb_vehicle_COLOR_1] ]  >>> 8), VEHICLE_INFO[vehicle_modelid - 400][vehicle_info_NAME]);
                else strcatf(dialog_body, "- {%06x}[{ffffff}ID: %d{%06x}] %s {"#WARNING_COLOR"}(Personal)\n", (VEHICLE_COLORS[ GLOBAL_VEHICLES[i][gb_vehicle_COLOR_1] ]  >>> 8), i, (VEHICLE_COLORS[ GLOBAL_VEHICLES[i][gb_vehicle_COLOR_1] ]  >>> 8), VEHICLE_INFO[vehicle_modelid - 400][vehicle_info_NAME]);
                counter ++;
                if(counter > 10) break;
            }
        }
    }
    if (!counter) strcat(dialog_body, "{666666}No hay vehiculos cerca.");
    Dialog_Open(playerid, Dialog:GasCanisterSelectVehicle, DIALOG_STYLE_INPUT, "{"#GAS_STATION_COLOR"}Bidon de Gasolina", dialog_body, "Continuar", "Cancelar");
    return 1;
}
DialogResponse:GasCanisterSelectVehicle(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (sscanf(inputtext, "d", PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_VEHICLE])) return ShowPlayerMessage(playerid, "ID de vehiculo invalida.");
        if (!Vehicle_IsValid(PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_VEHICLE])) return ShowPlayerMessage(playerid, "El vehiculo no está disponible.");
        if (GetPlayerNearVehicle(playerid) != PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_VEHICLE]) return ShowPlayerMessage(playerid, "El vehiculo ya no está disponible.");
        if (GLOBAL_VEHICLES[PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_VEHICLE]][gb_vehicle_GAS] >= GLOBAL_VEHICLES[PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_VEHICLE]][gb_vehicle_MAX_GAS]) return ShowPlayerMessage(playerid, "El vehiculo tiene el tanque lleno.");

        Dialog_Show(playerid, Dialog:GasCanisterRefuelAmount);
    }
    return 1;
}

DialogCreate:GasCanisterRefuelAmount(playerid) {
    Dialog_Open(playerid, Dialog:GasCanisterRefuelAmount, DIALOG_STYLE_INPUT, "{"#GAS_STATION_COLOR"}Bidon de Gasolina", "{"#GAS_STATION_COLOR"}·{ffffff}¿Cuantos litros deseas repostar desde el bidon? (Ingresa un valor numérico):", "Repostar", "Cancelar");
    return 1;
}
DialogResponse:GasCanisterRefuelAmount(playerid, response, listitem, inputtext[]) {
    if (response) {
        new vehicleid = PLAYER_TEMP[playerid][pt_SELECTED_DIALOG_VEHICLE];
        if (!Vehicle_IsValid(vehicleid)) return ShowPlayerMessage(playerid, "El vehiculo ya no está disponible.");
        if (GetPlayerNearVehicle(playerid) != vehicleid) return ShowPlayerMessage(playerid, "El vehiculo ya no está disponible.");
        if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] >= GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS]) return ShowPlayerMessage(playerid, "El vehiculo tiene el tanque lleno.");
        if (g_vehicle_refueling_timer[vehicleid] != -1) return ShowPlayerMessage(playerid, "El vehiculo ya está siendo repostado.");

        new slot = GetValidPlayerInventoryType(playerid, INVENTORY_GAS_CANISTER);
        if (slot == -1) return ShowPlayerMessage(playerid, "No tienes un bidon de gasolina en el inventario.");

        new Float:canister_fuel = p_inventory[playerid][slot][p_inventory_amount];
        if (canister_fuel <= 0.0) {
            return ShowPlayerMessage(playerid, "Tu bidon de gasolina está vacío.");
        }

        new Float:amount;
        if (sscanf(inputtext, "f", amount)) return ShowPlayerMessage(playerid, "Debes ingresar una cantidad válida de litros para repostar.");
        if (amount <= 0.0) return ShowPlayerMessage(playerid, "Debes ingresar una cantidad válida de litros para repostar.");

        if (amount > canister_fuel) amount = canister_fuel;
        new Float:needed_fuel = floatsub(GLOBAL_VEHICLES[vehicleid][gb_vehicle_MAX_GAS], GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS]);
        if (needed_fuel <= 0.0) return ShowPlayerMessage(playerid, "El vehiculo tiene el tanque lleno.");
        if (amount > needed_fuel) amount = needed_fuel;

        GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS] = floatadd(GLOBAL_VEHICLES[vehicleid][gb_vehicle_GAS], amount);
        p_inventory[playerid][slot][p_inventory_amount] = floatround(floatsub(canister_fuel, amount));
        ShowPlayerMessage(playerid, "Has repostado ~r~%.2f litros~w~~h~ de gasolina al vehiculo desde el bidon.", amount);

        if (p_inventory[playerid][slot][p_inventory_amount] <= 0.0) {
            RemovePlayerInventorySlot(playerid, slot);
            ShowPlayerMessage(playerid, "Tu bidon de gasolina se ha vaciado.");
        }
    }
    return 1;
}

//GPS
DialogCreate:PlayerGPSSite_6(playerid) {
    new dialog[90 * (sizeof(g_fuel_stations) + 1)], sites;
    for(new i = 0; i != sizeof(g_fuel_stations); i++ ) {
        PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][sites] = i;
        strcatf(dialog, "{FFFFFF}Gasolinera %d\t{"#GAS_STATION_COLOR"}%.2f Km\n", sites + 1, (GetPlayerDistanceFromPoint(playerid, g_fuel_stations[i][fuel_station_pos_x], g_fuel_stations[i][fuel_station_pos_y], g_fuel_stations[i][fuel_station_pos_z]) * 0.01));
        sites ++;
    }
    Dialog_Open(playerid, Dialog:PlayerGPSSite_6, DIALOG_STYLE_TABLIST, "GPS - {"#GAS_STATION_COLOR"}Gasolineras", dialog, "Continuar", "Atrás");
    return 1;
}
DialogResponse:PlayerGPSSite_6(playerid, response, listitem, inputtext[]) {
    if (response) SetPlayer_GPS_Checkpoint(playerid, g_fuel_stations[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][fuel_station_pos_x], g_fuel_stations[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][fuel_station_pos_y], g_fuel_stations[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][fuel_station_pos_z], 0, 0);
    else ShowDialog(playerid, DIALOG_PLAYER_GPS_SITES);
    return 1;
}

hook OnVehicleSpawn(vehicleid) {
    if (g_vehicle_refueling_timer[vehicleid] != -1) {
        KillTimer(g_vehicle_refueling_timer[vehicleid]);
        g_vehicle_refueling_timer[vehicleid] = -1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleDeath(vehicleid, killerid) {
    if (g_vehicle_refueling_timer[vehicleid] != -1) {
        KillTimer(g_vehicle_refueling_timer[vehicleid]);
        g_vehicle_refueling_timer[vehicleid] = -1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
