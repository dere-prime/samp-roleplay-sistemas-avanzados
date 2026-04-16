/*

    Camionero
        Prime Roleplay
            Impl

    - l -> local
*/
#include <YSI_Coding\y_hooks>

enum {
    TRUCK_STATE_INVALID = -1,
    TRUCK_STATE_UNLOAD,
    TRUCK_STATE_RETURN
}

enum enum_v_truck_info {
    bool: v_loaded,
    bool: v_loading,
    bool: v_unloading,
    bool: v_delivered,
    v_truck_state
}

static Float:l_trucker_trips[][] = {
    {-1823.6847, -191.7480, 13.2491},
    {2121.9277, -1783.5508, 13.3877},
    {2668.7751, -1474.4067, 30.5611},
    {1980.8810, -1274.7607, 23.8203},
    {1025.2565, -1362.9026, 13.5642},
    {-2199.0808, -2349.2275, 30.6250},
    {-1417.7451, -1458.7130, 101.6840},
    {1727.6818, 973.9053, 10.8203},
    {1970.3521, 2436.8411, 10.8203},
    {1091.2904, 2380.5549, 10.8203}
};
static Float:l_truck_return_position[3] = {-61.3851, -307.3987, 5.6024};
static Float:l_truck_load_position[3] = {-130.5549, -314.1512, 2.7646};

static v_truck_info[MAX_VEHICLES][enum_v_truck_info];
static PlayerText:p_truck_loading_td[MAX_PLAYERS][5];
static p_truck_checkpoint[MAX_PLAYERS];
static Float:p_truck_loading_value[MAX_PLAYERS];
static bool: p_truck_box[MAX_PLAYERS];
static p_truck_index[MAX_PLAYERS];

CancelPlayerTruckerJob(playerid) {
	if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
		DestroyDynamicCP(p_truck_checkpoint[playerid]);
		p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
	}
    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
    HidePlayerTruckTd(playerid);
    p_truck_loading_value[playerid] = 0;
    p_truck_box[playerid] = false;
    p_truck_index[playerid] = 0;
    p_truck_loading_value[playerid] = 0;
    if (IsPlayerWorking(playerid, WORK_TRUCK)) CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
}

ResetTruckInfo(const vehicleid) { 
    v_truck_info[vehicleid][v_truck_state] = TRUCK_STATE_INVALID;
    v_truck_info[vehicleid][v_loaded] = false;
    v_truck_info[vehicleid][v_loading] = false;
    v_truck_info[vehicleid][v_unloading] = false;
    v_truck_info[vehicleid][v_delivered] = false;
    return 1; 
}

Truck_StartWork(const playerid, const vehicleid) {
    if (!Vehicle_IsValid(vehicleid)) return 0;
    switch (v_truck_info[vehicleid][v_truck_state]) {
        case TRUCK_STATE_INVALID: {
            SendPlayerClientMessage(playerid, "{"#PRIMARY_COLOR"}·{ffffff}Estacionate muy cerca de el punto rojo que te aparece en el mapa para continuar.");

            if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
                DestroyDynamicCP(p_truck_checkpoint[playerid]);
                p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
            }
            p_truck_checkpoint[playerid] = CreateDynamicCP(l_truck_load_position[0], l_truck_load_position[1], l_truck_load_position[2], 5.0, 0, 0, playerid, 9999999999.0);
            new info[1];
            info[0] = CHECKPOINT_TYPE_TRUCK_CARRY;
            Streamer_SetArrayData(STREAMER_TYPE_CP, p_truck_checkpoint[playerid], E_STREAMER_EXTRA_ID, info);
            Streamer_Update(playerid, STREAMER_TYPE_CP);
        }
        case TRUCK_STATE_UNLOAD, TRUCK_STATE_RETURN: {
            SetPlayerTruckCheckpoint(playerid, vehicleid);
        }
    }
    return 1;
}

Truck_EndWork(const playerid) {
    CancelPlayerTruckerJob(playerid);
    return 1;
}

SetPlayerTruckCheckpoint(const playerid, const vehicleid) {
	if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
		DestroyDynamicCP(p_truck_checkpoint[playerid]);
		p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
	}

    new const index = p_truck_index[playerid];
    new const Float:distance = GetDistanceBetweenPoints3D(l_truck_return_position[0], l_truck_return_position[1], l_truck_return_position[2], l_trucker_trips[index][0], l_trucker_trips[index][1], l_trucker_trips[index][2]);

    if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
        DestroyDynamicCP(p_truck_checkpoint[playerid]);
        p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
    }

    if (v_truck_info[vehicleid][v_truck_state] == TRUCK_STATE_UNLOAD) {
        p_truck_checkpoint[playerid] = CreateDynamicCP(l_trucker_trips[index][0], l_trucker_trips[index][1], l_trucker_trips[index][2], 5.0, 0, 0, playerid, 9999999999.0);
        new info[1];
        info[0] = CHECKPOINT_TYPE_TRUCK_UNLOAD;
        Streamer_SetArrayData(STREAMER_TYPE_CP, p_truck_checkpoint[playerid], E_STREAMER_EXTRA_ID, info);
        Streamer_Update(playerid, STREAMER_TYPE_CP);

        SendPlayerClientMessage(playerid, "{"#PRIMARY_COLOR"}·{ffffff}Lleva la mercancia al punto de entrega, pago: %s{"#GREEN_COLOR"}${ffffff}.", number_format_thousand(floatround(2.5 * distance)));
    }

    if (v_truck_info[vehicleid][v_truck_state] == TRUCK_STATE_RETURN) {
        p_truck_checkpoint[playerid] = CreateDynamicCP(l_truck_return_position[0], l_truck_return_position[1], l_truck_return_position[2], 5.0, 0, 0, playerid, 9999999999.0);
        new info[1];
        info[0] = CHECKPOINT_TYPE_TRUCK_RETURN;
        Streamer_SetArrayData(STREAMER_TYPE_CP, p_truck_checkpoint[playerid], E_STREAMER_EXTRA_ID, info);
        Streamer_Update(playerid, STREAMER_TYPE_CP);

        SendPlayerClientMessage(playerid, "{"#PRIMARY_COLOR"}·{ffffff}Regresa al trabajo para recibir tu pago: %s{"#GREEN_COLOR"}${ffffff}.", number_format_thousand(floatround(2.5 * distance)));
    }
	return 1;
}

CheckIfTruckCanBeUsable(const vehicleid) {
    if (v_truck_info[vehicleid][v_loading] || v_truck_info[vehicleid][v_unloading]) return 1;
    return 0;
}
bool:CheckIfPlayerHaveTruckBox(const playerid) { return p_truck_box[playerid]; }

ShowPlayerTruckTd(const playerid) {
    for (new i = 0; i != sizeof(p_truck_loading_td[]); i ++) {
	    PlayerTextDrawShow(playerid, p_truck_loading_td[playerid][i]);
    }
}

HidePlayerTruckTd(const playerid) {
    for (new i = 0; i != sizeof(p_truck_loading_td[]); i ++) {
	    PlayerTextDrawHide(playerid, p_truck_loading_td[playerid][i]);
    }
}

UpdatePlayerLoadingTruckSize(playerid) {
    new Float:size = 296.0 + (p_truck_loading_value[playerid] * 0.64);
	PlayerTextDrawTextSize(playerid, p_truck_loading_td[playerid][4], size, 0.000000);
	PlayerTextDrawShow(playerid, p_truck_loading_td[playerid][4]);
	return 1;
}

forward TruckLoadUp(const playerid, const vehicleid);
public TruckLoadUp(const playerid, const vehicleid) {
    p_truck_loading_value[playerid] += 10.0;
    if (p_truck_loading_value[playerid] > 100.0) p_truck_loading_value[playerid] = 100.0;
    
	if (p_truck_loading_value[playerid] < 100.0) {
		UpdatePlayerLoadingTruckSize(playerid);

        if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
            DestroyDynamicCP(p_truck_checkpoint[playerid]);
            p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
        }
        p_truck_checkpoint[playerid] = CreateDynamicCP(l_truck_load_position[0], l_truck_load_position[1], l_truck_load_position[2], 5.0, 0, 0, playerid, 9999999999.0);
        new info[1];
        info[0] = CHECKPOINT_TYPE_TRUCK_CARRY;
        Streamer_SetArrayData(STREAMER_TYPE_CP, p_truck_checkpoint[playerid], E_STREAMER_EXTRA_ID, info);
        Streamer_Update(playerid, STREAMER_TYPE_CP);

        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		return 1;
	}
	
	HidePlayerTruckTd(playerid);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 1;
    GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_BOOT] = 0;
	UpdateVehicleParams(vehicleid);
	
	v_truck_info[vehicleid][v_loaded] = true;
	v_truck_info[vehicleid][v_loading] = false;
	v_truck_info[vehicleid][v_delivered] = false;
	v_truck_info[vehicleid][v_unloading] = false;
	v_truck_info[vehicleid][v_truck_state] = TRUCK_STATE_UNLOAD;
	
    p_truck_index[playerid] = random(sizeof(l_trucker_trips));

	ShowPlayerMessage(playerid, "Has cargado correctamente el camion, subete y llevalo al punto de entrega.");
	return 1;
}

forward TruckUnLoadUp(const playerid, const vehicleid);
public TruckUnLoadUp(const playerid, const vehicleid) {
	if (p_truck_loading_value[playerid] < 100.0) {
		p_truck_loading_value[playerid] += 10.0;
		if (p_truck_loading_value[playerid] > 100.0) p_truck_loading_value[playerid] = 100.0;
		
		UpdatePlayerLoadingTruckSize(playerid);
		KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
		PLAYER_TEMP[playerid][pt_TIMERS][9] = SetTimerEx("TruckUnLoadUp", 1000, false, "ii", playerid, vehicleid);
		return 1;
	}
	
	HidePlayerTruckTd(playerid);
	
	GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 1;
	UpdateVehicleParams(vehicleid);
	
	v_truck_info[vehicleid][v_unloading] = false;
	v_truck_info[vehicleid][v_delivered] = true;
	v_truck_info[vehicleid][v_truck_state] = TRUCK_STATE_RETURN;

	SetPlayerTruckCheckpoint(playerid, vehicleid);
	return 1;
}

hook OnPlayerConnect(playerid) {
    if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
        DestroyDynamicCP(p_truck_checkpoint[playerid]);
        p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
    }
    p_truck_loading_value[playerid] = 0.0;
    p_truck_box[playerid] = false;

    p_truck_loading_td[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 340.000, "background");
    PlayerTextDrawLetterSize(playerid, p_truck_loading_td[playerid][0], 0.000, 2.500);
    PlayerTextDrawTextSize(playerid, p_truck_loading_td[playerid][0], 0.000, 90.000);
    PlayerTextDrawAlignment(playerid, p_truck_loading_td[playerid][0], 2);
    PlayerTextDrawColor(playerid, p_truck_loading_td[playerid][0], -1);
    PlayerTextDrawUseBox(playerid, p_truck_loading_td[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, p_truck_loading_td[playerid][0], 255);
    PlayerTextDrawSetShadow(playerid, p_truck_loading_td[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, p_truck_loading_td[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, p_truck_loading_td[playerid][0], 150);
    PlayerTextDrawFont(playerid, p_truck_loading_td[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, p_truck_loading_td[playerid][0], 1);

    p_truck_loading_td[playerid][1] = CreatePlayerTextDraw(playerid, 246.000, 313.000, "_");
    PlayerTextDrawTextSize(playerid, p_truck_loading_td[playerid][1], 70.000, 70.000);
    PlayerTextDrawAlignment(playerid, p_truck_loading_td[playerid][1], 1);
    PlayerTextDrawColor(playerid, p_truck_loading_td[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, p_truck_loading_td[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, p_truck_loading_td[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, p_truck_loading_td[playerid][1], 0);
    PlayerTextDrawFont(playerid, p_truck_loading_td[playerid][1], 5);
    PlayerTextDrawSetProportional(playerid, p_truck_loading_td[playerid][1], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_truck_loading_td[playerid][1], 456);
    PlayerTextDrawSetPreviewRot(playerid, p_truck_loading_td[playerid][1], -90.000000, 0.000000, 180.000000, 1.000000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_truck_loading_td[playerid][1], 1, 1);

    p_truck_loading_td[playerid][2] = CreatePlayerTextDraw(playerid, 323.000, 339.000, "CAMIONERO"); //status
    PlayerTextDrawLetterSize(playerid, p_truck_loading_td[playerid][2], 0.180, 0.900);
    PlayerTextDrawAlignment(playerid, p_truck_loading_td[playerid][2], 2);
    PlayerTextDrawColor(playerid, p_truck_loading_td[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, p_truck_loading_td[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, p_truck_loading_td[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, p_truck_loading_td[playerid][2], 150);
    PlayerTextDrawFont(playerid, p_truck_loading_td[playerid][2], 2);
    PlayerTextDrawSetProportional(playerid, p_truck_loading_td[playerid][2], 1);

    p_truck_loading_td[playerid][3] = CreatePlayerTextDraw(playerid, 299.000, 353.000, "bar_bg");
    PlayerTextDrawLetterSize(playerid, p_truck_loading_td[playerid][3], 0.000, 0.400);
    PlayerTextDrawTextSize(playerid, p_truck_loading_td[playerid][3], 360.000, 0.000);
    PlayerTextDrawAlignment(playerid, p_truck_loading_td[playerid][3], 1);
    PlayerTextDrawColor(playerid, p_truck_loading_td[playerid][3], -1);
    PlayerTextDrawUseBox(playerid, p_truck_loading_td[playerid][3], 1);
    PlayerTextDrawBoxColor(playerid, p_truck_loading_td[playerid][3], 0xF591278F);
    PlayerTextDrawSetShadow(playerid, p_truck_loading_td[playerid][3], 1);
    PlayerTextDrawSetOutline(playerid, p_truck_loading_td[playerid][3], 1);
    PlayerTextDrawBackgroundColor(playerid, p_truck_loading_td[playerid][3], 150);
    PlayerTextDrawFont(playerid, p_truck_loading_td[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, p_truck_loading_td[playerid][3], 1);

    p_truck_loading_td[playerid][4] = CreatePlayerTextDraw(playerid, 299.000, 353.000, "bar_progress");
    PlayerTextDrawLetterSize(playerid, p_truck_loading_td[playerid][4], 0.000, 0.400);
    PlayerTextDrawTextSize(playerid, p_truck_loading_td[playerid][4], 360.000, 0.000); //min 296.0
    PlayerTextDrawAlignment(playerid, p_truck_loading_td[playerid][4], 1);
    PlayerTextDrawColor(playerid, p_truck_loading_td[playerid][4], -1);
    PlayerTextDrawUseBox(playerid, p_truck_loading_td[playerid][4], 1);
    PlayerTextDrawBoxColor(playerid, p_truck_loading_td[playerid][4], 0xF59127FF);
    PlayerTextDrawSetShadow(playerid, p_truck_loading_td[playerid][4], 1);
    PlayerTextDrawSetOutline(playerid, p_truck_loading_td[playerid][4], 1);
    PlayerTextDrawBackgroundColor(playerid, p_truck_loading_td[playerid][4], 150);
    PlayerTextDrawFont(playerid, p_truck_loading_td[playerid][4], 1);
    PlayerTextDrawSetProportional(playerid, p_truck_loading_td[playerid][4], 1);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate) {
    if (newstate == PLAYER_STATE_DRIVER) {
		new vehicleid = GetPlayerVehicleID(playerid);
        if (IsPlayerWork(playerid, WORK_TRUCK)) {
            if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK) {
                if (WORK_VEHICLES[vehicleid][work_vehicle_WORK] == WORK_TRUCK) {
                    if (WORK_VEHICLES[vehicleid][work_vehicle_PID] != PI[playerid][pi_ID]) { ShowPlayerMessage(playerid, "No eres el conductor de este camion."); RemovePlayerFromVehicle(playerid); return 1; }

                    if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE && PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_TRUCK) {
                        ShowPlayerNotification(playerid, "Tienes que dejar de estar de servicio en tu otro trabajo para continuar.");
                        RemovePlayerFromVehicle(playerid); 
                        return 1;
                    } else {
                        CallLocalFunction("StartPlayerJob", "iii", playerid, WORK_TRUCK, vehicleid);
                    }
                }
            }
        }
    }
    if (oldstate == PLAYER_STATE_DRIVER) {
        new vehicleid = PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID];
        if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] == VEHICLE_TYPE_WORK) {
            if (WORK_VEHICLES[vehicleid][work_vehicle_WORK] == WORK_TRUCK) {
                switch (v_truck_info[vehicleid][v_truck_state]) {
                    case TRUCK_STATE_INVALID: {
                        if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
                            DestroyDynamicCP(p_truck_checkpoint[playerid]);
                            p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
                        }

                        if (GetVehicleDistanceFromPoint(vehicleid, l_truck_load_position[0], l_truck_load_position[1], l_truck_load_position[2]) >= 40.0) {
                            ShowPlayerNotification(playerid, "El camion esta muy lejos de el punto de carga, acercate mas para poder cargar el camion.");
                            CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
                            return 1;
                        }

                        if (GetVehicleDistanceFromPoint(vehicleid, l_truck_load_position[0], l_truck_load_position[1], l_truck_load_position[2]) <= 15.0) {
                            ShowPlayerNotification(playerid, "El camion esta muy cerca de el punto de carga, alejate mas para poder cargar el camion.");
                            CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
                            return 1;
                        }

                        p_truck_checkpoint[playerid] = CreateDynamicCP(l_truck_load_position[0], l_truck_load_position[1], l_truck_load_position[2], 5.0, 0, 0, playerid, 9999999999.0);
                        new info[1];
                        info[0] = CHECKPOINT_TYPE_TRUCK_CARRY;
                        Streamer_SetArrayData(STREAMER_TYPE_CP, p_truck_checkpoint[playerid], E_STREAMER_EXTRA_ID, info);
                        Streamer_Update(playerid, STREAMER_TYPE_CP);

                        ShowPlayerTruckTd(playerid);
		                UpdatePlayerLoadingTruckSize(playerid);

                        ShowPlayerNotification(playerid, "Ve al punto rojo conocido como la zona de descarga para que te den una caja. Cuanto tengas la caja en tus manos debes regresar al camion para depositarla dentro.~n~~n~ \
                        Debes completar los mismos pasos hasta llenar la barra de carga.");
                    }
                    default: {
                        HidePlayerTruckTd(playerid);
                        p_truck_loading_value[playerid] = 0;
                        KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][9]);
                        v_truck_info[vehicleid][v_loading] = false;
                        v_truck_info[vehicleid][v_unloading] = false;

                        CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], true);
                    }
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid) {
	new info[1];
	Streamer_GetArrayData(STREAMER_TYPE_CP, checkpointid, E_STREAMER_EXTRA_ID, info);

	switch(info[0]) {
        case CHECKPOINT_TYPE_TRUCK_CARRY: {
            if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
                DestroyDynamicCP(p_truck_checkpoint[playerid]);
                p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
            }

            if (IsPlayerNotWorking(playerid, WORK_TRUCK)) return ShowPlayerMessage(playerid, "No estas trabajando de camionero.");
            if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return ShowPlayerMessage(playerid, "Necesitas estar de pie.");
            if (p_truck_box[playerid]) return ShowPlayerMessage(playerid, "Ya tienes una caja.");

            new const vehicleid = GetPVarInt(playerid, "CurrentWorkVehicleid");
            if (!Vehicle_IsValid(vehicleid)) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] != VEHICLE_TYPE_WORK) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_TRUCK) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (WORK_VEHICLES[vehicleid][work_vehicle_PID] != PI[playerid][pi_ID]) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");

            ApplyAnimation(playerid, "CARRY", "liftup105", 4.1, false, false, false, false, false);
            p_truck_box[playerid] = true;

            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

			GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
            GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_BOOT] = 1;
			UpdateVehicleParams(vehicleid);

            p_truck_checkpoint[playerid] = CreateDynamicCP(GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][0], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][1], GLOBAL_VEHICLES[vehicleid][gb_vehicle_POS][2], 7.0, 0, 0, playerid, 9999999999.0);
            info[0] = CHECKPOINT_TYPE_TRUCK_PUT;
            Streamer_SetArrayData(STREAMER_TYPE_CP, p_truck_checkpoint[playerid], E_STREAMER_EXTRA_ID, info);
            Streamer_Update(playerid, STREAMER_TYPE_CP);
        }
        case CHECKPOINT_TYPE_TRUCK_PUT: {
            if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
                DestroyDynamicCP(p_truck_checkpoint[playerid]);
                p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
            }

            if (IsPlayerNotWorking(playerid, WORK_TRUCK)) return ShowPlayerMessage(playerid, "No estas trabajando de camionero.");
            if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return ShowPlayerMessage(playerid, "Necesitas estar de pie.");
            if (!p_truck_box[playerid]) return ShowPlayerMessage(playerid, "No tienes una caja.");

            new const vehicleid = GetPVarInt(playerid, "CurrentWorkVehicleid");
            if (!Vehicle_IsValid(vehicleid)) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] != VEHICLE_TYPE_WORK) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_TRUCK) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (WORK_VEHICLES[vehicleid][work_vehicle_PID] != PI[playerid][pi_ID]) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");

            ApplyAnimation(playerid, "CARRY", "putdwn105", 4.1, false, false, false, false, false);
            p_truck_box[playerid] = false;

	        v_truck_info[vehicleid][v_loading] = true;
            SetTimerEx("TruckLoadUp", 600, false, "ii", playerid, vehicleid);
        }
        case CHECKPOINT_TYPE_TRUCK_UNLOAD: {
            if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
                DestroyDynamicCP(p_truck_checkpoint[playerid]);
                p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
            }
            if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo.");
            if (IsPlayerNotWorking(playerid, WORK_TRUCK)) return ShowPlayerMessage(playerid, "No estas trabajando de camionero.");

            new const vehicleid = GetPVarInt(playerid, "CurrentWorkVehicleid");
            if (!Vehicle_IsValid(vehicleid)) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] != VEHICLE_TYPE_WORK) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_TRUCK) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (WORK_VEHICLES[vehicleid][work_vehicle_PID] != PI[playerid][pi_ID]) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");

            GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_ENGINE] = 0;
            GLOBAL_VEHICLES[vehicleid][gb_vehicle_PARAMS_BOOT] = 1;
            UpdateVehicleParams(vehicleid);

            v_truck_info[vehicleid][v_unloading] = true;

            ShowPlayerTruckTd(playerid);
            p_truck_loading_value[playerid] = 0;
            TruckUnLoadUp(playerid, vehicleid);
        }
        case CHECKPOINT_TYPE_TRUCK_RETURN: {
            if (IsValidDynamicCP(p_truck_checkpoint[playerid])) {
                DestroyDynamicCP(p_truck_checkpoint[playerid]);
                p_truck_checkpoint[playerid] = INVALID_STREAMER_ID;
            }
            if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo.");
            if (IsPlayerNotWorking(playerid, WORK_TRUCK)) return ShowPlayerMessage(playerid, "No estas trabajando de camionero.");

            new const vehicleid = GetPVarInt(playerid, "CurrentWorkVehicleid");
            if (!Vehicle_IsValid(vehicleid)) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (GLOBAL_VEHICLES[vehicleid][gb_vehicle_TYPE] != VEHICLE_TYPE_WORK) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (WORK_VEHICLES[vehicleid][work_vehicle_WORK] != WORK_TRUCK) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (WORK_VEHICLES[vehicleid][work_vehicle_PID] != PI[playerid][pi_ID]) return ShowPlayerMessage(playerid, "No tienes un camion para hacer esto.");
            if (!v_truck_info[vehicleid][v_delivered]) return 0;

            new const index = p_truck_index[playerid];
            new const Float:distance = GetDistanceBetweenPoints3D(l_truck_return_position[0], l_truck_return_position[1], l_truck_return_position[2], l_trucker_trips[index][0], l_trucker_trips[index][1], l_trucker_trips[index][2]);
            new const price = (floatround(2.5 * distance));

            GivePlayerCash(playerid, price, true, false);
            ShowPlayerNotification(playerid, "Has recibido un pago de %s"P_CURR" por el recorrido.", number_format_thousand(price));

            ShowPlayerNotification(playerid, "El camion ha sido regresado al garage, si queires volver a trabajar saca otro camion.");
            CancelPlayerTruckerJob(playerid);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}