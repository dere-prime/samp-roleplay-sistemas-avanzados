/* 

    Weapons Factory
        Prime Roleplay
*/

#include <YSI_Coding\y_hooks>

#define WEAPONS_FACTORY_PAYMENT 4800 // + random (1500)

/* 

    Enumeraciones

*/

enum {
    PLAYER_CARGO_STATE_SPAWN_TRUCK,
    PLAYER_CARGO_STATE_SEARCH_WARE,
    PLAYER_CARGO_STATE_RETURN_WARE
}
enum wf_player_enum {
    wf_player_cargo_checkpoint,
    wf_player_cargo_state,
    wf_cargo_timeout,
    wf_cargo_confirm_timeout,
    wf_cargo_timer,
    wf_cargo_talked_to_roy_time
}

/* 

    Variables globales

*/

new WeaponsFactoryPlayer[MAX_PLAYERS][wf_player_enum];
new wf_current_wp_parts = 500;
static Text3D:wf_cargo_info_3dtextlabel;
static Float: wf_search_cargo_pos[3] = {-2148.1987, -241.8855, 36.5156};
static Float: wf_spawn_truck_pos[3] = {-2177.9182, -236.1390, 36.5220};
static Float: wf_spawned_truck_pos[4] = {-2170.3462, -209.8348, 35.9451, 268.3435}; //DFT 578
static Float: wf_cargo_return_pos[3] = {-2134.716552, -237.888732, 35.320312};
new bool: wf_is_cargo_vehicle[MAX_VEHICLES];
new wf_cargo_vehicle_driver[MAX_VEHICLES];
static cargo_job_actor = INVALID_ACTOR_ID;
static cargo_job_actor_timer[MAX_PLAYERS] = -1;

/*

    Funciones definidas

*/

ResetWeaponsFactoryData(playerid) {
	if (IsValidDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint])) {
		DestroyDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint]);
	}
	WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint] = INVALID_STREAMER_ID;
    WeaponsFactoryPlayer[playerid][wf_player_cargo_state] = 0;
    WeaponsFactoryPlayer[playerid][wf_cargo_timeout] = gettime() + 30;
    WeaponsFactoryPlayer[playerid][wf_cargo_talked_to_roy_time] = 0;
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
            WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint] = CreateDynamicCP(-333.9665, 2516.0408, 35.2259, 5.0, 0, 0, playerid, 9999999999.0);
            info[0] = CHECKPOINT_TYPE_WF_MISSION;
            Streamer_SetArrayData(STREAMER_TYPE_CP, WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint], E_STREAMER_EXTRA_ID, info);
            Streamer_Update(playerid, STREAMER_TYPE_CP);
        }
        case PLAYER_CARGO_STATE_RETURN_WARE: {
            WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint] = CreateDynamicCP(wf_cargo_return_pos[0], wf_cargo_return_pos[1], wf_cargo_return_pos[2], 5.0, 0, 0, playerid, 9999999999.0);
            info[0] = CHECKPOINT_TYPE_WF_RETURN_WARE;
            Streamer_SetArrayData(STREAMER_TYPE_CP, WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint], E_STREAMER_EXTRA_ID, info);
            Streamer_Update(playerid, STREAMER_TYPE_CP);
        }
    }
    return 1;
}

stock updateWeaponsFactoryParts3DTxt() {
    new label_str[256];
    format(label_str, 256, "{"#PRIMARY_COLOR"}·{ffffff}Acercate si quieres buscar un cargamento de piezas\nPiezas disponibles: {"#SECONDARY_COLOR"}%s", number_format_thousand(wf_current_wp_parts));
    UpdateDynamic3DTextLabelText(wf_cargo_info_3dtextlabel, -1, label_str);
}

stock DestroyCargoVehicle(const vehicleid) {
    if (vehicleid != INVALID_VEHICLE_ID) {
        if (wf_is_cargo_vehicle[vehicleid]) {
            wf_is_cargo_vehicle[vehicleid] = false;
            wf_cargo_vehicle_driver[vehicleid] = -1;
            DestroyVehicleEx(vehicleid);
        }
    }
    return 1;
}

/*

    Callbacks definidos

*/

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
    HidePlayerMissionDialog(playerid, false);
    TogglePlayerControllableEx(playerid, true);
    WeaponsFactoryPlayer[playerid][wf_cargo_confirm_timeout] = gettime() + 15;
    return 1;
}

forward EstablishRoyCall(playerid, type);
public EstablishRoyCall(playerid, type) {
    EndPhoneCall(playerid);
    PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = true; /* Bloquear para que otros jugadores no puedan llamar */
    PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_STATE_INCOMING_CALL;
    PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = INVALID_PLAYER_ID;
    PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_NUMBER] = 616103;

    UpdatePlayerCallingCard(playerid, 616103);
    ShowPlayerCallingCard(playerid);

    new str_text[64];
    format(str_text, sizeof(str_text), "* El telefono de %s esta sonando...", PLAYER_TEMP[playerid][pt_NAME]);
    ProxDetector(playerid, 15.0, str_text, 0xADFF2FAA, 0xADFF2FAA, 0xADFF2FAA, 0xADFF2FAA, 0xADFF2FAA, 85);
    PlayerPlaySoundEx(playerid, 20804);

    ShowPlayerNotification(playerid, "La llamada de Roy se respondera automaticamente, pero puedes cancelarla usando ~r~/colgar~w~~h~ antes de que sea contestada. Una vez respondas la llamada, ya no podras cancelarla.");

    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][6]);
    PLAYER_TEMP[playerid][pt_TIMERS][6] = SetTimerEx("AutoRoyCallResponse", 5000, false, "ii", playerid, type);
    return 1;
}
forward AutoRoyCallResponse(playerid, type);
public AutoRoyCallResponse(playerid, type) {
    switch (type) {
        case 0: ShowPlayerMissionDialog(playerid, 30, "Roy", "Hola, amigo. ¿Como estas?~n~Me alegra que la ultima mision haya salido bien. Espero verte mas seguido por aqui. Solo queria llamarte para darte las gracias. Y recuerda... no le cuentes a nadie lo que hiciste.");
        case 1: ShowPlayerMissionDialog(playerid, 30, "Roy", "Bien, amigo. Esto es lo que haras: al llegar no te alejes de la zona o el trabajo se cancelara y tendras que volver a pie. Tampoco empujes ni jodas los vehiculos, o esos tipos se iran y te dejaran tirado. ¡Suerte!");
    }

    if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
        temporarilyHidePlayerSpeedoMetr(playerid, 11000);
    }

    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
    PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 6000, false, "ib", playerid, false);

    PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_STATE_ESTABLISHED;
    UpdatePlayerCallingCard(playerid, 616103);

    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][6]);
    PLAYER_TEMP[playerid][pt_TIMERS][6] = SetTimerEx("EndRoyPhoneCall", 8000, false, "i", playerid);
    return 1;
}
forward EndRoyPhoneCall(playerid);
public EndRoyPhoneCall(playerid) {
    PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL] = false;
    PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_STATE] = CALL_GAME_STATE_NONE;
    PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_PLAYERID] = INVALID_PLAYER_ID;
	PLAYER_TEMP[playerid][pt_PLAYER_PHONE_CALL_NUMBER] = 0;
    
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
    UpdatePlayerCallingCard(playerid);

    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][6]);

	SendPlayerClientMessage(playerid, "{"#PRIMARY_COLOR"}Llamada finalizada.");
    return 1;
}

/* 

    Callbacks y otros

*/

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

    CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}·{ffffff}Mision Busqueda de Cargamento\nAcercate para ver mas informacion", -1, wf_spawn_truck_pos[0], wf_spawn_truck_pos[1], wf_spawn_truck_pos[2], 15.0, .testlos = true, .worldid = 0, .interiorid = 0);
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
            if (WeaponsFactoryPlayer[playerid][wf_player_cargo_state] == PLAYER_CARGO_STATE_SEARCH_WARE) {
                KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
                ShowPlayerMissionDialog(playerid, 30, "Roy", "Ya te envie a buscar algo, porque no puedes disimular un poco?");
                PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, false);
                return 1;
            }
            
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
            Dialog_Open(
                playerid, Dialog:WFCargoMissionStart, DIALOG_STYLE_MSGBOX, 
                "{"#PRIMARY_COLOR"}·{ffffff}Cargamento de piezas", 
                "\
                    {"#PRIMARY_COLOR"}Misión: {FFFFFF}Búsqueda de cargamento de piezas\n\
                    \n\
                    {"#YELLOW_COLOR"}· Nivel requerido: {FFFFFF}5\n\
                    {"#YELLOW_COLOR"}· Objetos necesarios: {FFFFFF}Teléfono\n\
                    \n\
                    {"#PRIMARY_COLOR"}Detalles:{FFFFFF}\n\
                    · Roy cerró un trato con unos mafiosos por un cargamento de piezas.\n\
                    · Tu trabajo será recoger la mercancía y cargarla en el camión.\n\
                    · Evita hablar con ellos y no llames la atención.\n\
                    · Cuando termines, abandona la zona lo más rápido posible.\n\
                    \n\
                    {"#PRIMARY_COLOR"}Recompensa:\n\
                    {"#GREEN_COLOR"}· ${FFFFFF}4,800 a {"#GREEN_COLOR"}${FFFFFF}6,300\n\
                    {"#BLUE_COLOR"}· {FFFFFF}50 a 100 de experiencia\n\
                    \n\
                    ¿Aceptas el encargo?\n\
                ", 
                "Aceptar", "Rechazar"
            );
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
        case CHECKPOINT_TYPE_WF_MISSION: {
            if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            new WeaponsFactory_Cargo_Vehicleid = GetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid");
            if (GetPlayerVehicleID(playerid) != WeaponsFactory_Cargo_Vehicleid) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            if (WeaponsFactory_Cargo_Vehicleid == INVALID_VEHICLE_ID) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            if (!wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid]) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            if (wf_cargo_vehicle_driver[WeaponsFactory_Cargo_Vehicleid] != PI[playerid][pi_ID]) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            if (GetVehicleOccupants(WeaponsFactory_Cargo_Vehicleid) > 1) return ShowPlayerMessage(playerid, "El camion debe estar sin pasajeros.");
            
            if (IsValidDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint])) {
                DestroyDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint]);
            }
            WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint] = INVALID_STREAMER_ID;

            PlayPlayerMission(playerid, "factorySearchWp", MISSION_WEAPONS_FACTORY, false);
        }
        case CHECKPOINT_TYPE_WF_RETURN_WARE: {
            if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            new WeaponsFactory_Cargo_Vehicleid = GetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid");
            if (GetPlayerVehicleID(playerid) != WeaponsFactory_Cargo_Vehicleid) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            if (WeaponsFactory_Cargo_Vehicleid == INVALID_VEHICLE_ID) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            if (!wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid]) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            if (wf_cargo_vehicle_driver[WeaponsFactory_Cargo_Vehicleid] != PI[playerid][pi_ID]) return ShowPlayerMessage(playerid, "Necesitas estar conduciendo tu unidad asignada.");
            if (GetVehicleOccupants(WeaponsFactory_Cargo_Vehicleid) > 1) return ShowPlayerMessage(playerid, "El camion debe estar sin pasajeros.");

            DestroyCargoVehicle(WeaponsFactory_Cargo_Vehicleid);
            ResetWeaponsFactoryData(playerid);
            SetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid", INVALID_VEHICLE_ID);

            WeaponsFactoryPlayer[playerid][wf_cargo_timeout] = gettime() + 30;
            
            new payment = WEAPONS_FACTORY_PAYMENT + random(1500);
            new parts_cargo = 200 + random(300);
            new rep = minrand(50, 200);
            AddPlayerReputation(playerid, rep);

            if (GivePlayerCash(playerid, payment, true, false)) {
                ShowPlayerNotificationModel(playerid, 1212, 0.0, 0.0, 45.0, "Has entregado el cargamento de piezas y has recibido ~g~$%s~w~~h~.", number_format_thousand(payment));
                wf_current_wp_parts += parts_cargo;
                updateWeaponsFactoryParts3DTxt();
                ShowPlayerNotification(playerid, "El almacen de piezas ha recibido ~y~%s~w~~h~ piezas de armas.", number_format_thousand(parts_cargo));
            }

            ShowPlayerNotification(playerid, "~g~+%s~w~~h~ de experiencia.", number_format_thousand(rep));
            
            EndPhoneCall(playerid);

            KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][6]);
            PLAYER_TEMP[playerid][pt_TIMERS][6] = SetTimerEx("EstablishRoyCall", 4000, false, "dd", playerid, 0);
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
    DestroyCargoVehicle(WeaponsFactory_Cargo_Vehicleid);
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnVehicleDeath(vehicleid, killerid) {
    if (wf_is_cargo_vehicle[vehicleid]) {
        new cargo_playerid = GetPlayerIdFromAccountId(wf_cargo_vehicle_driver[vehicleid]);
        if (IsPlayerConnected(cargo_playerid)) {
            if (PLAYER_TEMP[cargo_playerid][pt_USER_LOGGED]) {
                ResetWeaponsFactoryData(cargo_playerid);
                ShowPlayerNotification(cargo_playerid, "Tu unidad asignada desde la fabrica de armas ha sido destruida.");
            }
        }

        DestroyCargoVehicle(vehicleid);
    }
}
hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) {
    if (wf_is_cargo_vehicle[vehicleid]) {
        new Float:sx, Float:sy, Float:sz;
        GetPlayerPos(playerid, sx, sy, sz);

        if (wf_cargo_vehicle_driver[vehicleid] != PI[playerid][pi_ID]) {	
            RemovePlayerFromVehicle(playerid);
			SetPlayerPos(playerid, sx, sy, sz);
            ShowPlayerMessage(playerid, "No puedes conducir este vehiculo porque no eres su conductor designado.");
            return Y_HOOKS_BREAK_RETURN_1;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerStateChange(playerid, newstate, oldstate) {
    if (newstate == PLAYER_STATE_DRIVER) {
        new vehicleid = GetPlayerVehicleID(playerid);
        new WeaponsFactory_Cargo_Vehicleid = GetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid");
        if (WeaponsFactory_Cargo_Vehicleid != INVALID_VEHICLE_ID) {
            if (wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid]) {
                if (vehicleid == WeaponsFactory_Cargo_Vehicleid) {
                    if (IsValidVehicle(vehicleid)) {
                        SetWeaponsFactoryCheckpoint(playerid);

                        if (WeaponsFactoryPlayer[playerid][wf_cargo_timer] != -1) {
                            KillTimer(WeaponsFactoryPlayer[playerid][wf_cargo_timer]);
                        }
                    }
                }
            }
        }
    }

    if (oldstate == PLAYER_STATE_DRIVER) {
        new vehicleid = PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID];
        new WeaponsFactory_Cargo_Vehicleid = GetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid");
        if (WeaponsFactory_Cargo_Vehicleid != INVALID_VEHICLE_ID) {
            if (wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid]) {
                if (vehicleid == WeaponsFactory_Cargo_Vehicleid) {
                    if (IsValidDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint])) {
                        DestroyDynamicCP(WeaponsFactoryPlayer[playerid][wf_player_cargo_checkpoint]);
                    }

                    KillTimer(WeaponsFactoryPlayer[playerid][wf_cargo_timer]);
                    WeaponsFactoryPlayer[playerid][wf_cargo_timer] = SetTimerEx("WFCargoTimeOut", SecondsToMs(30), false, "d", playerid);
                    ShowPlayerNotification(playerid, "Tienes ~y~30 segundos~w~~h~ para volver al camion, si no vuelves el viaje se cancelara.");
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

/* 

    Dialog Responses

*/

DialogResponse:ConfirmStartWFCargo(playerid, response, listitem, inputtext[]) {
    if (response) {
        ResetWeaponsFactoryData(playerid);
        WeaponsFactoryPlayer[playerid][wf_player_cargo_state] = PLAYER_CARGO_STATE_SPAWN_TRUCK;
        WeaponsFactoryPlayer[playerid][wf_cargo_talked_to_roy_time] = gettime() + 15;
        WeaponsFactoryPlayer[playerid][wf_cargo_timer] = SetTimerEx("WFCargoTimeOut", SecondsToMs(30), false, "d", playerid);
        SetWeaponsFactoryCheckpoint(playerid);
    }
    return 1;
}
DialogResponse:WFCargoMissionStart(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (gettime() > WeaponsFactoryPlayer[playerid][wf_cargo_talked_to_roy_time]) return ShowPlayerMessage(playerid, "Para hacer esta mision necesitas hablar con Roy primero.");
        ShowPlayerTimeOut(WeaponsFactoryPlayer[playerid][wf_cargo_timeout], 30, playerid, "Debes esperar ~b~%s~w~ para volver a buscar otro cargamento.");
        if (WeaponsFactoryPlayer[playerid][wf_player_cargo_state] != PLAYER_CARGO_STATE_SPAWN_TRUCK) return ShowPlayerMessage(playerid, "No puedes sacar otro camion hasta que termines tu viaje.");
        if (PI[playerid][pi_WANTED_LEVEL] > 0) return ShowPlayerMessage(playerid, "No puedes estar siendo buscado por la policia.");
        if (IsPlayerMission(playerid)) return ShowPlayerMessage(playerid, "Esta funcion no esta disponible actualmente.");
        if (IsPlayerWork(playerid, WORK_POLICE)) return ShowPlayerMessage(playerid, "Esta funcion no esta disponible actualmente.");
        if (IsPlayerNotWorking(playerid, WORK_NONE)) return ShowPlayerMessage(playerid, "Esta funcion no esta disponible actualmente.");
        if (PI[playerid][pi_LEVEL] < 5) return ShowPlayerMessage(playerid, "Necesitas ser nivel 5 para poder hacer esta mision.");
        if (!GetValidPlayerPhoneNumber(playerid)) return ShowPlayerMessage(playerid, "Necesitas tener un telefono para poder hacer esta mision.");        

        new vehicle_id = INVALID_VEHICLE_ID;
        vehicle_id = CreateVehicle(578, wf_spawned_truck_pos[0], wf_spawned_truck_pos[1], wf_spawned_truck_pos[2], wf_spawned_truck_pos[3], 1, 1, -1, false);
        if (!Vehicle_IsValid(vehicle_id)) return ShowPlayerMessage(playerid, "Error, intentalo de nuevo mas tarde.");
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_NONE; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = 578;
        format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "%04d %c%c%c", vehicle_id, getRandomLetter(), getRandomLetter(), getRandomLetter());
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = wf_spawned_truck_pos[0]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = wf_spawned_truck_pos[1]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = wf_spawned_truck_pos[2]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = wf_spawned_truck_pos[3];
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = 1; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = 1; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3;
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = VEHICLE_INFO[578 - 400][vehicle_info_MAX_GAS]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS];
        GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_INTERIOR] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = 0;
        SetVehicleVirtualWorldEx(vehicle_id, 0);
        SetVehicleToRespawnEx(vehicle_id);
        wf_is_cargo_vehicle[vehicle_id] = true;
        wf_cargo_vehicle_driver[vehicle_id] = PI[playerid][pi_ID];
        PutPlayerInVehicleEx(playerid, vehicle_id, 0);
        SetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid", vehicle_id);

        if (WeaponsFactoryPlayer[playerid][wf_cargo_timer] != -1) {
            KillTimer(WeaponsFactoryPlayer[playerid][wf_cargo_timer]);
        }
        WeaponsFactoryPlayer[playerid][wf_cargo_timer] = -1;

        WeaponsFactoryPlayer[playerid][wf_player_cargo_state] = PLAYER_CARGO_STATE_SEARCH_WARE;
        SetWeaponsFactoryCheckpoint(playerid);

        KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][6]);
        PLAYER_TEMP[playerid][pt_TIMERS][6] = SetTimerEx("EstablishRoyCall", 8000, false, "dd", playerid, 1);
    }
    return 1;
}
