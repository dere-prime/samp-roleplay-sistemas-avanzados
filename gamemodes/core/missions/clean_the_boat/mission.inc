/* 

    Clean the boat
        Mission

    Name:CleanTheBoat
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

hook OnScriptInit() {
    /*
        Cargar preparativos
    */

    //Cubeta
    CreateDynamicObject(2713, 163.964828, -1886.073364, 1.102549, 0.000000, 0.000000, 45.000000);
    CreateDynamic3DTextLabel("Pulsa {"#YELLOW_COLOR"}Y{ffffff} para humedecer el trapo en la cubeta", 0xffffffff, 163.964828, -1886.073364, (1.102549 + 0.2), 4.0, .testlos = 1, .worldid = 0, .interiorid = 0);

    //NPC
    CleanTheBoat_fisher_npc = CreateDynamicActor(4, 173.1746, -1887.8440, 1.0492, 343.3241, .worldid = 0, .interiorid = 0);
    CreateDynamic3DTextLabel("Pescador", 0xCCCCCCCC, 173.1746, -1887.8440, 1.0492 + 1.1, 10.0, .testlos = true, .worldid = 0, .interiorid = 0);

    new info[3], talk_pickup = CreateDynamicPickup(0, 1, 173.1746, -1887.8440, 1.0492, 0, 0);
    info[0] = PICKUP_TYPE_FISHER_MISSION;
    Streamer_SetArrayData(STREAMER_TYPE_PICKUP, talk_pickup, E_STREAMER_EXTRA_ID, info);

    //bote
    CleanTheBoat_vehicle_boat = AddStaticVehicle(453, 172.7958, -1892.4836, 0.3084, 20.2026, 1, 1);
    SetVehicleParamsEx(CleanTheBoat_vehicle_boat, 1, 0, 0, 1, 0, 0, 0);
    SetVehicleHealth(CleanTheBoat_vehicle_boat, 999999999.9);
}

hook OnUnoccupiedVehicleUpd(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z) {
    if (vehicleid == CleanTheBoat_vehicle_boat) {
        new Float:pos_x, Float:pos_y, Float:pos_z;
        GetVehiclePos(vehicleid, pos_x, pos_y, pos_z);
        if(GetDistanceBetweenPoints3D(pos_x, pos_y, pos_z, 172.7958, -1892.4836, 0.3084) > 5.0) {
            SetVehiclePos(vehicleid, 172.7958, -1892.4836, 0.3084);
            SetVehicleZAngle(vehicleid, 20.2026);
            SetVehicleVelocity(vehicleid, 0.000, 0.000, 0.000);
            SetVehicleHealth(vehicleid, 99999999.9);
        }
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleDeath(vehicleid, killerid) {
    if (vehicleid == CleanTheBoat_vehicle_boat) {
        DestroyVehicle(CleanTheBoat_vehicle_boat);
        CleanTheBoat_vehicle_boat = AddStaticVehicle(453, 172.7958, -1892.4836, 0.3084, 20.2026, 1, 1);
        SetVehicleParamsEx(CleanTheBoat_vehicle_boat, 1, 0, 0, 1, 0, 0, 0);
        SetVehicleHealth(CleanTheBoat_vehicle_boat, 99999999.9);
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
            SetVehiclePos(vehicleid, 172.7958, -1892.4836, 0.3084);
            SetVehicleZAngle(vehicleid, 20.2026);
            SetVehicleVelocity(vehicleid, 0.000, 0.000, 0.000);
            SetVehicleHealth(vehicleid, 99999999.9);
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
            ShowPlayerMissionDialog(playerid, 4, "Pescador", "%s", CleanTheBoat_fisher_curr_dlgs[random(sizeof(CleanTheBoat_fisher_curr_dlgs))]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, true);
        } else {
            TogglePlayerControllableEx(playerid, false);
            ApplyDynamicActorAnimation(CleanTheBoat_fisher_npc, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);
            ApplyDynamicActorAnimation(CleanTheBoat_fisher_npc, "GANGS", "prtial_gngtlkH", 4.1, false, false, false, false, 0);
            ShowPlayerMissionDialog(playerid, 4, "Pescador", "%s", CleanTheBoat_fisher_strt_dlgs[random(sizeof(CleanTheBoat_fisher_strt_dlgs))]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, true);
        }

        PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 8;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
