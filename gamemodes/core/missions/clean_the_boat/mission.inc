/* 

    Clean the boat
        Mission

    Name:CleanTheBoat

    Mision Jugable tipo secundaria.
    No lleva variables core.
*/

#include <YSI_Coding\y_hooks>

static CleanTheBoat_fisher_npc;
static CleanTheBoat_vehicle_boat;

static CleanTheBoat_fisher_curr_dlgs[][] = {
    "No creas que porque te encontre en el medio de el mar te la voy a dejar pasar. ¡A LIMPIAR!",
    "Oye no me molestes, si no querias limpiar solo tenias que decirme!",
    "No te pagare mal, pero dejame descansar un poco mientras tu limpias el barco.",
    "Ya, esta bueno, no me hagas perder el tiempo y ponte a limpiar el barco."
};

static CleanTheBoat_fisher_strt_dlgs[][] = {
    "Hey amigo! me ayudas a limpiar el barco? no me gusta ensuciarlo. Te dare una recompensa si me ayudas.",
    "Oye puedes ayudarme a limpiar el bote? te dare algo por ayudarme, recuerda que te encontre en el medio del mar y no me gusta ensuciar el bote.",
    "Hey, no me gusta ensuciar el bote, ayudame a limpiarlo y te dare una recompensa.",
    "Ven a limpiarme el bote, te dare algo ahi para que te compres un refresco."
};

static bool: cleanTheBoat_player_rag[MAX_PLAYERS] = false;
static Float: cleanTheBoat_player_progress[MAX_PLAYERS] = 0.0;
static cleanTheBoat_player_steps[MAX_PLAYERS] = 0;

forward UpdatePlayerCleaningTheBoat(const playerid);

forward CleanTheBoat_fisher_dialog(const playerid);
public CleanTheBoat_fisher_dialog(const playerid) {
    HidePlayerMissionDialog(playerid, true);
    Dialog_Open(playerid, Dialog:cleanTheBoat_Mission_Prompt, DIALOG_STYLE_MSGBOX, "{"#PRIMARY_COLOR"}·{ffffff}Limpiar el bote", "{ffffff}¿Te gustaria limpiar el bote de el pescador por una recompensa?", "Aceptar", "Rechazar");
    PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 8;
    return 1;
}

restoreMissionBoat() {
    if (IsValidVehicle(CleanTheBoat_vehicle_boat)) DestroyVehicle(CleanTheBoat_vehicle_boat);
    CleanTheBoat_vehicle_boat = AddStaticVehicle(453, 172.7958, -1892.4836, 0.3084, 20.2026, 1, 1);
    SetVehicleParamsEx(CleanTheBoat_vehicle_boat, 0, 0, 0, 1, 0, 0, 0);
    SetVehicleHealth(CleanTheBoat_vehicle_boat, 999999999.9);
    return 1;
}

adjustMissionBoat() {
    if (!IsValidVehicle(CleanTheBoat_vehicle_boat)) return restoreMissionBoat();
    SetVehicleParamsEx(CleanTheBoat_vehicle_boat, 0, 0, 0, 1, 0, 0, 0);
    SetVehicleHealth(CleanTheBoat_vehicle_boat, 999999999.9);
    SetVehiclePos(CleanTheBoat_vehicle_boat, 172.7958, -1892.4836, 0.3084);
    SetVehicleZAngle(CleanTheBoat_vehicle_boat, 20.2026);
    SetVehicleVelocity(CleanTheBoat_vehicle_boat, 0.000, 0.000, 0.000);
    return 1;
}

hook OnScriptInit() {
    /*
        Cargar preparativos
    */

    //Cubeta
    CreateDynamicObject(2713, 165.722625, -1884.927734, 0.466556, 0.000000, 0.000000, frandom(360.0, 0.0, 2));

    //NPC
    CleanTheBoat_fisher_npc = CreateDynamicActor(4, 173.1746, -1887.8440, 1.0492, 343.3241, .worldid = 0, .interiorid = 0);
    CreateDynamic3DTextLabel("Pescador", 0xCCCCCCCC, 173.1746, -1887.8440, 1.0492 + 1.1, 10.0, .testlos = true, .worldid = 0, .interiorid = 0);

    new info[3], talk_pickup = CreateDynamicPickup(0, 1, 173.1746, -1887.8440, 1.0492, 0, 0);
    info[0] = PICKUP_TYPE_FISHER_MISSION;
    Streamer_SetArrayData(STREAMER_TYPE_PICKUP, talk_pickup, E_STREAMER_EXTRA_ID, info);

    //bote
    restoreMissionBoat();
}

Mission:cleanTheBoat(playerid, index) {
    IsPlayerMission(playerid) = true;
    PlayerMissionId(playerid) = index;
    PlayerMissionStep(playerid) = MISSION_STEP_PLAYABLE;
    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
    PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 15;

    ShowPlayerNotificationModel(playerid, 2844, 0.0, 0.0, 0.0, "~n~~g~+1~w~~h~ Trapo seco~n~");

    MissionUpdates(playerid) = 0;
    cleanTheBoat_player_rag[playerid] = false;
    cleanTheBoat_player_progress[playerid] = 0.0;
    cleanTheBoat_player_steps[playerid] = 0;

    ShowPlayerMissionDialog(playerid, 4, "Pescador", "Generalmente el bote se ensucia en la parte de abajo, esa suciedad hace que mi bote sea lento y gaste mas gasolina, necesitamos arreglarlo. Humedece el trapo que te di en la cubeta y ve a limpiarlo");
    PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, false);

    createMissionCheckpoint(playerid, 165.722625, -1884.927734, 1.102549, 0, 0, 2.0);
    return 1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid) {
    new info[1];
	Streamer_GetArrayData(STREAMER_TYPE_CP, checkpointid, E_STREAMER_EXTRA_ID, info);
	if (info[0] == CHECKPOINT_TYPE_MISSION) {
        KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
        switch (MissionUpdates(playerid)) {
            case 0: {
                MissionUpdates(playerid) = 1;
                cleanTheBoat_player_rag[playerid] = true;
                ShowPlayerNotificationModel(playerid, 2844, 0.0, 0.0, 0.0, "~n~~r~-1~w~~h~ Trapo seco~n~");
                ShowPlayerNotificationModel(playerid, 2844, 0.0, 0.0, 0.0, "~n~~g~+1~w~~h~ Trapo humedo~n~");

                createMissionCheckpoint(playerid, 172.7958, -1892.4836, 0.3084, 0, 0);
                if (cleanTheBoat_player_steps[playerid] <= 0) ShowPlayerMissionDialog(playerid, 18631, "Informacion", "Tu trapo esta humedo, ahora debes ir al barco para limpiar el casco inferior.");
                else ShowPlayerMissionDialog(playerid, 18631, "Informacion", "Tu trapo esta humedo otra vez, sigue limpiando el barco.");
                PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 3000, false, "ib", playerid, false);

                ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 1, 1, 0, 0);
            }
            case 1: {
	            if (IsValidDynamicCP(mission_info[playerid][mission_checkpoint])) {
                    DestroyDynamicCP(mission_info[playerid][mission_checkpoint]);
                }

                MissionUpdates(playerid) = 0;
                cleanTheBoat_player_rag[playerid] = false;
                UpdatePlayerCleaningTheBoat(playerid);

                new Float:x,Float:y,Float:z,Float:ang;
                GetPlayerPos(playerid, x, y, z); GetPlayerFacingAngle(playerid, ang);
                SetPlayerStaticPos(playerid, x, y, z, ang);

                SetPlayerActiveStaticPos(playerid);
            }
            default: {
	            if (IsValidDynamicCP(mission_info[playerid][mission_checkpoint])) {
                    DestroyDynamicCP(mission_info[playerid][mission_checkpoint]);
                }
            }
        }
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

public UpdatePlayerCleaningTheBoat(const playerid) {
	if (mission_info[playerid][mission_updates_timer] != -1) { KillTimer(mission_info[playerid][mission_updates_timer]); mission_info[playerid][mission_updates_timer] = -1; }

    cleanTheBoat_player_progress[playerid] += frandom(8.0, 3.0, 2);
    if (cleanTheBoat_player_progress[playerid] > 100.0) cleanTheBoat_player_progress[playerid] = 100.0;
    ShowPlayerMessage(playerid, "Limpiando... %.1f", cleanTheBoat_player_progress[playerid]);

    if (cleanTheBoat_player_progress[playerid] >= 100.0) {
		ClearAnimations(playerid);
        TogglePlayerControllableEx(playerid, false);

        ShowPlayerNotificationModel(playerid, 2844, 0.0, 0.0, 0.0, "~n~~r~-1~w~~h~ Trapo humedo~n~");

        cleanTheBoat_player_progress[playerid] = 0.0;

        ShowPlayerMessage(playerid, "_");
        DisablePlayerActiveStaticPos(playerid);

        cleanTheBoat_player_steps[playerid] ++;
        if (cleanTheBoat_player_steps[playerid] >= 6) {
            ShowPlayerMissionDialog(playerid, 18631, "Informacion", "Haz terminado de limpiar el bote, vuelve con el pescador para recibir tu recompensa.");
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, true);
            
            MissionUpdates(playerid) = 3;
            createMissionCheckpoint(playerid, 173.1746, -1887.8440, 1.0492, 0, 0, 2.0);
        }
        else {
            ShowPlayerMissionDialog(playerid, 18631, "Informacion", "Haz limpiado una parte, pero debes continuar para finalizar con la limpieza de el barco, Ve a humedecer el trapo en la cubeta.");
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, true);
            
            createMissionCheckpoint(playerid, 165.722625, -1884.927734, 1.102549, 0, 0, 2.0);
        }
        return 1;
    }
	if (GetPlayerAnimationIndex(playerid) != 168) ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, 1, 0, 0, 0, 0, 0);

	mission_info[playerid][mission_updates_timer] = SetTimerEx("UpdatePlayerCleaningTheBoat", 1000, true, "i", playerid);
	return 1;
}

hook OnPlayerConnect(playerid) {
    cleanTheBoat_player_rag[playerid] = false;
    cleanTheBoat_player_progress[playerid] = 0.0;
    cleanTheBoat_player_steps[playerid] = 0;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason) {
    if (IsValidDynamicCP(mission_info[playerid][mission_checkpoint])) {
        DestroyDynamicCP(mission_info[playerid][mission_checkpoint]);
    }
    cleanTheBoat_player_rag[playerid] = false;
    cleanTheBoat_player_progress[playerid] = 0.0;
    cleanTheBoat_player_steps[playerid] = 0;

    if (mission_info[playerid][mission_id] == MISSION_CLEAN_THE_BOAT) {
        mission_info[playerid][mission_current] = false;
        mission_info[playerid][mission_id] = -1;
        mission_info[playerid][mission_step] = 0;
        mission_info[playerid][mission_updates] = 0;
        mission_info[playerid][mission_updates_timer] = -1;
        mission_info[playerid][mission_checkpoint] = -1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

DialogResponse:cleanTheBoat_Mission_Prompt(playerid, response, listitem, inputtext[]) {
    if (response) {
        PlayPlayerMission(playerid, "cleanTheBoat", MISSION_CLEAN_THE_BOAT, false);
    }
    return 1;
}

hook OnUnoccupiedVehicleUpd(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z) {
    if (vehicleid == CleanTheBoat_vehicle_boat) {
        new Float:pos_x, Float:pos_y, Float:pos_z;
        GetVehiclePos(vehicleid, pos_x, pos_y, pos_z);
        if(GetDistanceBetweenPoints3D(pos_x, pos_y, pos_z, 172.7958, -1892.4836, 0.3084) > 5.0) {
            adjustMissionBoat();
        }
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleDeath(vehicleid, killerid) {
    if (vehicleid == CleanTheBoat_vehicle_boat) {
        restoreMissionBoat();
        if (IsPlayerConnected(killerid)) {
            Kick(killerid);
        }
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleStreamIn(vehicleid, forplayerid) {
    if (vehicleid == CleanTheBoat_vehicle_boat) {
        new Float:pos_x, Float:pos_y, Float:pos_z;
        GetVehiclePos(vehicleid, pos_x, pos_y, pos_z);
        if(GetDistanceBetweenPoints3D(pos_x, pos_y, pos_z, 172.7958, -1892.4836, 0.3084) > 5.0) {
            adjustMissionBoat();
        }
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerPickUpDynPickup(playerid, pickupid) {
    if (pickupid == PICKUP_TYPE_NONE) return Y_HOOKS_CONTINUE_RETURN_1;
	if (PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] > gettime()) return Y_HOOKS_BREAK_RETURN_0;
	if (Dialog_IsOpen(playerid)) return Y_HOOKS_BREAK_RETURN_0;
	new info[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, info);
	if (info[0] == PICKUP_TYPE_FISHER_MISSION) {
        KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);

        if (IsPlayerMission(playerid) && (PlayerMissionId(playerid) != MISSION_CLEAN_THE_BOAT)) {
            ShowPlayerMessage(playerid, "~r~Ahora mismo estas en una mision.");
            return Y_HOOKS_BREAK_RETURN_0;
        }

        if (PlayerMissionId(playerid) == MISSION_CLEAN_THE_BOAT) {
            TogglePlayerControllableEx(playerid, false);
            ApplyDynamicActorAnimation(CleanTheBoat_fisher_npc, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);
            ApplyDynamicActorAnimation(CleanTheBoat_fisher_npc, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);

            if (MissionUpdates(playerid) == 3) {
                ShowPlayerMissionDialog(playerid, 4, "Pescador", "Eso es todo! muchas gracias, aqui esta tu recompensa!!!!");
                PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, true);

                mission_info[playerid][mission_current] = false;
                mission_info[playerid][mission_id] = -1;
                mission_info[playerid][mission_step] = 0;
                mission_info[playerid][mission_updates] = 0;
                mission_info[playerid][mission_updates_timer] = -1;
                mission_info[playerid][mission_checkpoint] = -1;

	            AddPlayerReputation(playerid, minrand(50, 100));

                new reward = minrand(0, 5);
                switch (reward) {
                    case 2: {
                        new slot = GetPlayerInventorySlot(playerid);
                        if (slot != -1) {
                            SetPlayerInventorySlotData(playerid, slot, INVENTORY_MEDICINE, minrand(5, 20), 0, 0);
                            InsertPlayerInventorySlot(playerid, slot);
                        } else {
                            new cash = 2500 + random(600);
                            GivePlayerCash(playerid, 2500, true, false);
                            ShowPlayerNotificationModel(playerid, 1212, 0.0, 0.0, 45.0, "~n~~g~+%s~w~~h~ Dinero~n~", number_format_thousand(cash));
                        }
                    }
                    default: {
                        new cash = 2500 + random(600);
                        GivePlayerCash(playerid, 2500, true, false);
                        ShowPlayerNotificationModel(playerid, 1212, 0.0, 0.0, 45.0, "~n~~g~+%s~w~~h~ Dinero~n~", number_format_thousand(cash));
                    }
                }
            } else {
                PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, true);
                ShowPlayerMissionDialog(playerid, 4, "Pescador", "%s", CleanTheBoat_fisher_curr_dlgs[random(sizeof(CleanTheBoat_fisher_curr_dlgs))]);
            }
        } else {
            TogglePlayerControllableEx(playerid, false);
            ApplyDynamicActorAnimation(CleanTheBoat_fisher_npc, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);
            ApplyDynamicActorAnimation(CleanTheBoat_fisher_npc, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);
            ShowPlayerMissionDialog(playerid, 4, "Pescador", "%s", CleanTheBoat_fisher_strt_dlgs[random(sizeof(CleanTheBoat_fisher_strt_dlgs))]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("CleanTheBoat_fisher_dialog", 4000, false, "ib", playerid, true);
        }

        PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 8;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
