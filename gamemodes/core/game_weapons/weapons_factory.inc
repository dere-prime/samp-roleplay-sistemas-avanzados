/* 

    Weapons Factory
        Prime Roleplay
*/

#include <YSI_Coding\y_hooks>

enum {
    PLAYER_CARGO_STATE_SPAWN_TRUCK,
    PLAYER_CARGO_STATE_SEARCH_WARE,
    PLAYER_CARGO_STATE_RETURN_WARE
}

enum wf_player_enum {
    wf_player_cargo_index,
    wf_player_cargo_checkpoint,
    wf_player_cargo_state,
    wf_cargo_timeout,
    wf_cargo_confirm_timeout,
    wf_cargo_timer
}

new WeaponsFactoryPlayer[MAX_PLAYERS][wf_player_enum];

new wf_current_wp_parts = 500;

static Text3D:wf_cargo_info_3dtextlabel;

static Float: wf_search_cargo_pos[3] = {-2148.1987, -241.8855, 36.5156};
static Float: wf_create_guns_pos[3] = {-2164.0850, -246.8508, 36.5156};

static Float: wf_spawn_truck_pos[3] = {-2177.9182, -236.1390, 36.5220};
static Float: wf_spawned_truck_pos[4] = {-2170.3462, -209.8348, 35.9451, 268.3435}; //DFT 578

static bool: wf_is_cargo_vehicle[MAX_VEHICLES];
static wf_cargo_vehicle_driver[MAX_VEHICLES];

static cargo_job_actor = INVALID_ACTOR_ID;
static cargo_job_actor_timer[MAX_PLAYERS] = -1;


static Float: wf_cargo_trips[][3] = {
    {-1440.1420, 2624.8223, 55.8359},
    {-2778.1990, 766.3895, 51.1916},
    {-822.0228, 1429.0643, 13.7891},
    {-1104.6510, -1642.2827, 76.3672},
    {2359.9580, -2019.7985, 13.5470},
    {2778.7117, -2441.8337, 13.6359}
};

ResetWeaponsFactoryData(playerid) {
    WeaponsFactoryPlayer[playerid][wf_player_cargo_index] = -1;
	if (IsValidDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint])) {
		DestroyDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint]);
	}
	WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint] = INVALID_STREAMER_ID;
    WeaponsFactoryPlayer[playerid][wf_player_cargo_state] = 0;
    WeaponsFactoryPlayer[playerid][wf_cargo_timeout] = 0;
    WeaponsFactoryPlayer[playerid][wf_cargo_confirm_timeout] = 0;
    if (WeaponsFactoryPlayer[playerid][wf_cargo_timer] != -1) {
        KillTimer(WeaponsFactoryPlayer[playerid][wf_cargo_timer]);
    }
    WeaponsFactoryPlayer[playerid][wf_cargo_timer] = -1;
    return 1;
}

SetWeaponsFactoryCheckpoint(const playerid) {
	if (IsValidDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint])) {
		DestroyDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint]);
		WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint] = INVALID_STREAMER_ID;
	}

    new info[1];
    switch (WeaponsFactoryPlayer[playerid][wf_player_cargo_state]) {
        case PLAYER_CARGO_STATE_SPAWN_TRUCK: {
            SendPlayerClientMessageEx(playerid, 120, "{"#PRIMARY_COLOR"}·{ffffff}Saca un camion del almacen para ir a buscar un {"#YELLOW_COLOR"}cargamento de piezas{ffffff}. Recuerda que la {"#SECONDARY_COLOR"}policia{ffffff} no puede saber lo que estas haciendo.");
            ShowPlayerNotification(playerid, "Ve al punto rojo marcado en el mapa para sacar un camion, si tardas 30 segundos vas a perder el viaje.");

            WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint] = CreateDynamicCP(wf_spawn_truck_pos[0], wf_spawn_truck_pos[1], wf_spawn_truck_pos[2], 5.0, 0, 0, playerid, 9999999999.0);
            info[0] = CHECKPOINT_TYPE_WF_SPAWN_TRUCK;
            Streamer_SetArrayData(STREAMER_TYPE_CP, WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint], E_STREAMER_EXTRA_ID, info);
            Streamer_Update(playerid, STREAMER_TYPE_CP);
        }
        case PLAYER_CARGO_STATE_SEARCH_WARE: {
            if (WeaponsFactoryPlayer[playerid][wf_player_cargo_index] == -1) return 0;
        }
    }
    return 1;
}

forward WFCargoTimeOut(playerid);
public WFCargoTimeOut(playerid) {
    ShowPlayerNotification(playerid, "Has tardado mucho en hacer el recorrido, intentalo de nuevo.");

    new WeaponsFactory_Cargo_Vehicleid = GetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid");
    if (WeaponsFactory_Cargo_Vehicleid != INVALID_PLAYER_ID) {
        if (wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid]) {
            wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid] = false;
            wf_cargo_vehicle_driver[WeaponsFactory_Cargo_Vehicleid] = -1;
            DestroyVehicleEx(WeaponsFactory_Cargo_Vehicleid);
        }
    }

    ResetWeaponsFactoryData(playerid);
    return 1;
}

forward CargoJobHelperTimer(playerid);
public CargoJobHelperTimer(playerid) {
    HidePlayerMissionDialog(playerid);
    TogglePlayerControllableEx(playerid, true);
    WeaponsFactoryPlayer[playerid][wf_cargo_confirm_timeout] = gettime() + 5;
    return 1;
}

hook OnScriptInit() {
    new label_str[256], info[2], areaid;
    format(label_str, 256, "{"#PRIMARY_COLOR"}·{ffffff}Acercate si quieres buscar un cargamento de piezas\nPiezas disponibles: {"#SECONDARY_COLOR"}%s", number_format_thousand(wf_current_wp_parts));
    wf_cargo_info_3dtextlabel = CreateDynamic3DTextLabel(label_str, -1, wf_search_cargo_pos[0], wf_search_cargo_pos[1], wf_search_cargo_pos[2], 15.0, .testlos = true, .worldid = 0, .interiorid = 0);
    CreateDynamicPickup(1314, 1, wf_search_cargo_pos[0], wf_search_cargo_pos[1], wf_search_cargo_pos[2], 0, 0);
    areaid = CreateDynamicSphere(wf_search_cargo_pos[0], wf_search_cargo_pos[1], wf_search_cargo_pos[2], 1.5, 0, 0);
    info[0] = AREA_TYPE_WF_SEARCH_CARGO;
    Streamer_SetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);
    cargo_job_actor = CreateDynamicActor(30, -2146.1792, -242.1016, 36.5156, 87.2972, .worldid = 0, .interiorid = 0);
    CreateDynamic3DTextLabel("Roy", 0xCCCCCCCC, -2146.1792, -242.1016, 36.5156 + 1.1, 10.0, .testlos = true, .worldid = 0, .interiorid = 0);

    CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}·{ffffff}Acercate para sacar vehiculo para buscar el cargamento", -1, wf_spawn_truck_pos[0], wf_spawn_truck_pos[1], wf_spawn_truck_pos[2], 15.0, .testlos = true, .worldid = 0, .interiorid = 0);
    areaid = CreateDynamicSphere(wf_spawn_truck_pos[0], wf_spawn_truck_pos[1], wf_spawn_truck_pos[2], 1.5, 0, 0);
    info[0] = AREA_TYPE_WF_SPAWN_TRUCK;
    Streamer_SetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);

    CreateDynamicMapIcon(wf_search_cargo_pos[0], wf_search_cargo_pos[1], wf_search_cargo_pos[2], 6, -1, 0, 0);
}

hook OnPlayerEnterDynamicArea(playerid, areaid) {
	new info[2];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);
	switch(info[0]) {
        case AREA_TYPE_WF_SEARCH_CARGO: {
            if (WeaponsFactoryPlayer[playerid][wf_player_cargo_index] != -1) return ShowPlayerMessage(playerid, "Roy: oye amigo ya te envie a buscar algo, porque no disimulas un poco?");
            
            if (WeaponsFactoryPlayer[playerid][wf_cargo_confirm_timeout] < gettime()) {

                if (cargo_job_actor_timer[playerid] != -1) {
                    KillTimer(cargo_job_actor_timer[playerid]);
                    cargo_job_actor_timer[playerid] = -1;
                }
                TogglePlayerControllableEx(playerid, false);
                ApplyDynamicActorAnimation(cargo_job_actor, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);
                ApplyDynamicActorAnimation(cargo_job_actor, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);

                ShowPlayerMissionDialog(playerid, 30, "Roy", "Oye amigo, ¿estas seguro de lo que quieres hacer? Esto es algo un poco peligroso~n~Debes traer una mercancia muy importante, alejate un poco para que lo pienses y regresa para confirmarme, si tardas mucho tendre que volver a recordartelo.");
                cargo_job_actor_timer[playerid] = SetTimerEx("CargoJobHelperTimer", 8000, false, "i", playerid);
            } else {
                Dialog_Open(
                    playerid, Dialog:ConfirmStartWFCargo, DIALOG_STYLE_MSGBOX, 
                    "{"#PRIMARY_COLOR"}·{ffffff}Cargamento de piezas", 
                    "{ffffff}¿Estas segur@ que quieres buscar un {"#YELLOW_COLOR"}cargamento de piezas{ffffff}?\n\n\
                    Si necesitas informacion, ve a la seccion de {"#SECONDARY_COLOR"}Fabrica de piezas{ffffff} en el {"#PRIMARY_COLOR"}menu de ayuda{ffffff}.", 
                    "Confirmar", "Cancelar"
                );
                WeaponsFactoryPlayer[playerid][wf_cargo_confirm_timeout] = 0;
            }
            return Y_HOOKS_BREAK_RETURN_1;
        }
        case AREA_TYPE_WF_SPAWN_TRUCK: {
            if (WeaponsFactoryPlayer[playerid][wf_player_cargo_index] == -1) return ShowPlayerMessage(playerid, "No tienes un viaje activo.");
            if (WeaponsFactoryPlayer[playerid][wf_player_cargo_state] != PLAYER_CARGO_STATE_SPAWN_TRUCK) return ShowPlayerMessage(playerid, "No puedes sacar otro camion hasta que termines tu viaje.");

            new vehicle_id = INVALID_VEHICLE_ID;
            vehicle_id = CreateVehicle(578, wf_spawned_truck_pos[0], wf_spawned_truck_pos[1], wf_spawned_truck_pos[2], wf_spawned_truck_pos[3], random(255), random(255), -1, false);
            if (!Vehicle_IsValid(vehicle_id)) return ShowPlayerMessage(playerid, "Error, intentalo de nuevo mas tarde.");
            GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_NONE; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = 578;
            format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "%04d %c%c%c", vehicle_id, getRandomLetter(), getRandomLetter(), getRandomLetter());
            GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = wf_spawned_truck_pos[0]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = wf_spawned_truck_pos[1]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = wf_spawned_truck_pos[2]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = wf_spawned_truck_pos[3];
            GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
            GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
            GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
            GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = random(255); GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = random(255); GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3;
            GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = VEHICLE_INFO[578 - 400][vehicle_info_MAX_GAS]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS];
            GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_INTERIOR] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = 0;
            SetVehicleVirtualWorldEx(vehicle_id, 0);
            SetVehicleToRespawnEx(vehicle_id);
            wf_is_cargo_vehicle[vehicle_id] = true;
            wf_cargo_vehicle_driver[vehicle_id] = PI[playerid][pi_ID];
            PutPlayerInVehicleEx(playerid, vehicle_id, 0);

            if (WeaponsFactoryPlayer[playerid][wf_cargo_timer] != -1) {
                KillTimer(WeaponsFactoryPlayer[playerid][wf_cargo_timer]);
            }
            WeaponsFactoryPlayer[playerid][wf_cargo_timer] = -1;

            WeaponsFactoryPlayer[playerid][wf_player_cargo_state] = PLAYER_CARGO_STATE_SEARCH_WARE;
            SetWeaponsFactoryCheckpoint(playerid);
            return Y_HOOKS_BREAK_RETURN_1;
        }
    }

    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid) {
	new info[1];
	Streamer_GetArrayData(STREAMER_TYPE_CP, checkpointid, E_STREAMER_EXTRA_ID, info);

	switch(info[0]) {
        case CHECKPOINT_TYPE_WF_SPAWN_TRUCK: {
            if (IsValidDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint])) {
                DestroyDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint]);
            }
            WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint] = INVALID_STREAMER_ID;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid) {
    SetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid", INVALID_VEHICLE_ID);
    ResetWeaponsFactoryData(playerid);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason) {
    ResetWeaponsFactoryData(playerid);

    new WeaponsFactory_Cargo_Vehicleid = GetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid");
    if (WeaponsFactory_Cargo_Vehicleid != INVALID_PLAYER_ID) {
        if (wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid]) {
            wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid] = false;
            wf_cargo_vehicle_driver[WeaponsFactory_Cargo_Vehicleid] = -1;
            DestroyVehicleEx(WeaponsFactory_Cargo_Vehicleid);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

DialogResponse:ConfirmStartWFCargo(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (PI[playerid][pi_WANTED_LEVEL] > 0) return ShowPlayerMessage(playerid, "No puedes estar siendo buscado por la policia.");
        if (IsPlayerMission(playerid)) return ShowPlayerMessage(playerid, "Esta funcion no esta disponible actualmente.");
        if (IsPlayerWork(playerid, WORK_POLICE)) return ShowPlayerMessage(playerid, "Esta funcion no esta disponible actualmente.");
        if (IsPlayerNotWorking(playerid, WORK_NONE)) return ShowPlayerMessage(playerid, "Esta funcion no esta disponible actualmente.");
        if (WeaponsFactoryPlayer[playerid][wf_player_cargo_index] != -1) return ShowPlayerMessage(playerid, "Ya estas buscando un cargamento.");
        ShowPlayerTimeOut(WeaponsFactoryPlayer[playerid][wf_cargo_timeout], 30, playerid, "Debes esperar ~b~%s~w~ para volver a buscar otro cargamento.");

        ResetWeaponsFactoryData(playerid);

        WeaponsFactoryPlayer[playerid][wf_player_cargo_index] = random(sizeof(wf_cargo_trips));
        WeaponsFactoryPlayer[playerid][wf_player_cargo_state] = PLAYER_CARGO_STATE_SPAWN_TRUCK;
        WeaponsFactoryPlayer[playerid][wf_cargo_timeout] = gettime() + 30;

        WeaponsFactoryPlayer[playerid][wf_cargo_timer] = SetTimerEx("WFCargoTimeOut", SecondsToMs(30), false, "d", playerid);

        SetWeaponsFactoryCheckpoint(playerid);
    }
    return 1;
}

