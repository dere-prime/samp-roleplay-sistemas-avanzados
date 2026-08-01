/* 

    The Apperance
        Mission

    Name:TheApperance
*/

#include <YSI_Coding\y_hooks>

enum TheApperance_Data {
    ta_boat,
    ta_actor,
    ta_npc
}

new
    ta_data[MAX_PLAYERS][TheApperance_Data];

forward UpdateTheApperanceMission(const playerid);
#define UpdateTAMission(%0,%1) KillTimer(mission_info[%0][mission_updates_timer]); mission_info[%0][mission_updates_timer]=SetTimerEx("UpdateTheApperanceMission",%1,false,"i",%0)

/* Mission */
clear_the_apperance_data(const playerid) {
    KillTimer(mission_info[playerid][mission_updates_timer]);
    if(Vehicle_IsValid(ta_data[playerid][ta_boat])) {
        DestroyVehicle(ta_data[playerid][ta_boat]);
    }
    ta_data[playerid][ta_boat] = INVALID_VEHICLE_ID;
    if (IsValidActor(ta_data[playerid][ta_actor])) {
        DestroyActor(ta_data[playerid][ta_actor]);
    }
    ta_data[playerid][ta_actor] = INVALID_ACTOR_ID;
    if (FCNPC_IsValid(ta_data[playerid][ta_npc])) {
        FCNPC_Destroy(ta_data[playerid][ta_npc]);
    }
    ta_data[playerid][ta_npc] = INVALID_PLAYER_ID;
    MissionUpdates(playerid) = 0;
    return 1;
}

hook FCNPC_OnCreate(npcid) {
    ta_data[npcid][ta_npc] = INVALID_PLAYER_ID;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook FCNPC_OnReachDestination(npcid) {
    if (ta_data[npcid][ta_npc] != INVALID_PLAYER_ID) {
        new playerid = ta_data[npcid][ta_npc];
        if (!IsPlayerConnected(playerid)) return Y_HOOKS_BREAK_RETURN_0;
        MissionUpdates(playerid) ++;
        UpdateTAMission(playerid, 600);
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook Animator_OnFinish(playerid, animator, type) {
    if (animator == PLAYER_TEMP[playerid][pt_BLACK_SCREEN_ANIMATOR]) {
        if (mission_info[playerid][mission_id] == MISSION_THE_APPERANCE && mission_info[playerid][mission_current] == true) {
            switch (MissionUpdates(playerid)) {
                case 0: {
                    FCNPC_GoTo(ta_data[playerid][ta_npc], 620.0936, -2831.3062, 0.0000, FCNPC_MOVE_TYPE_DRIVE, .pathfinding = FCNPC_MOVE_PATHFINDING_AUTO);
                    MissionUpdates(playerid) ++;
                    UpdateTAMission(playerid, 3600);
                }
                case 7: {
                    MissionUpdates(playerid) ++;
                    UpdateTAMission(playerid, 5000);
                }
                case 8: {
                    ShowPlayerMissionDialog(playerid, 0, "Informacion", "Un pescador te encontro cerca de la playa y te dejo en el faro, ahi el personal te cuido y te dejo ir Luego de tu recuperacion parcial, ya puedes moverte normalmente pero tu salud esta un poco baja.");
                    HidePlayerBlackScreen(playerid);
                    MissionUpdates(playerid) ++;
                    UpdateTAMission(playerid, 5000);
                }
            }
            return Y_HOOKS_BREAK_RETURN_1;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

public UpdateTheApperanceMission(const playerid) {
    switch (MissionUpdates(playerid)) {
        case 0: {
            EaseHidePlayerBlackScreen(playerid, 1000);
        }
        case 1: {
            ShowPlayerMissionDialog(playerid, 4, "Pescador", "Bingo! encontre algo que me sacara de la quiebra");
            InterpolateCameraPos(playerid, 640.726745, -2837.349609, 8.551156, 640.726745, -2837.349609, 8.551156, 1000, CAMERA_CUT);
            InterpolateCameraLookAt(playerid, 622.125061, -2833.709472, 1.061153, 622.125061, -2833.709472, 1.061153, 1000, CAMERA_CUT);
        }
        case 2: {
            ShowPlayerMissionDialog(playerid, 4, "Pescador", "Al parecer es una persona.... Creo que necesito unos cristales nuevos");
            InterpolateCameraPos(playerid, 626.351440, -2835.583984, 1.034456, 626.351440, -2835.583984, 1.034456, 1000, CAMERA_CUT);
            InterpolateCameraLookAt(playerid, 621.961364, -2827.313232, 0.394457, 621.961364, -2827.313232, 0.394457, 1000, CAMERA_CUT);
            MissionUpdates(playerid) ++;
            UpdateTAMission(playerid, 4000);
        }
        case 3: {
            ShowPlayerMissionDialog(playerid, 4, "Pescador", "Hey!!, Oye tu, estas bien?");
            MissionUpdates(playerid) ++;
            UpdateTAMission(playerid, 4000);
        }
        case 4: {
            ShowPlayerMissionDialog(playerid, PI[playerid][pi_SKIN], "Tu", "....");
            MissionUpdates(playerid) ++;
            UpdateTAMission(playerid, 6000);
        }
        case 5: {
            ShowPlayerMissionDialog(playerid, 4, "Pescador", "Solo yo me esperaba que alguien en ese estado respondiera... Ah espera!!, QUEDATE AHI, BUSCARE COMO SACARTE!!!");   
            MissionUpdates(playerid) ++;
            UpdateTAMission(playerid, 4000);
        }
        case 6: {
            ShowPlayerMissionDialog(playerid, 4, "Pescador", "Lo llevare al faro, ahi podran ayudar a esta pobre persona.");
            MissionUpdates(playerid) ++;
            UpdateTAMission(playerid, 4000);
        }
        case 7: {
            EaseShowPlayerBlackScreen(playerid);
            HidePlayerMissionDialog(playerid, false);
        }
        case 8: {  
            CallLocalFunction("OnPlayerFinishMission", "dd", playerid, MISSION_THE_APPERANCE);
        }
        case 9: {
            mission_info[playerid][mission_current] = false;
            mission_info[playerid][mission_id] = -1;
            PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS] = true;
            SetPlayerHud(playerid);
            TogglePlayerControllableEx(playerid, true);
            HidePlayerMissionDialog(playerid, false);
            clear_the_apperance_data(playerid);
            ShowPlayerWelcomeMessage(playerid);
        }
    }
    return 1;
}

missionTAFinish(playerid) {
    player_mission[playerid][MISSION_THE_APPERANCE][m_completed] = true;
    mission_info[playerid][mission_step] = -1;

    TogglePlayerSpectatingEx(playerid, false);

    PI[playerid][pi_STATE] = ROLEPLAY_STATE_NORMAL;
    PI[playerid][pi_LOCAL_INTERIOR] = 0;
    PI[playerid][pi_POS_X] = SERVER_MAIN_SPAWN_X;
    PI[playerid][pi_POS_Y] = SERVER_MAIN_SPAWN_Y;
    PI[playerid][pi_POS_Z] = SERVER_MAIN_SPAWN_Z;
    PI[playerid][pi_ANGLE] = SERVER_MAIN_SPAWN_ANGLE;
    PI[playerid][pi_INTERIOR] = 0;

    SetPlayerPosEx(playerid, PI[playerid][pi_POS_X], PI[playerid][pi_POS_Y], PI[playerid][pi_POS_Z], PI[playerid][pi_ANGLE], PI[playerid][pi_INTERIOR], 0);

    SetCameraBehindPlayer(playerid);
    TogglePlayerControllableEx(playerid, false);
    SetPlayerVirtualWorld(playerid, 0);

    EaseHidePlayerBlackScreen(playerid, 1000);
    return 1;
}

Mission:TheApperance(playerid, index) {
    mission_info[playerid][mission_step] = MISSION_STEP_CINEMATIC;
    clear_the_apperance_data(playerid);
    ShowPlayerBlackScreen(playerid);
	PlayerTextDrawBoxColor(playerid, PlayerTextdraws[playerid][ptextdraw_BLACK_SCREEN], 0x000000FF);
    PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS] = false;
    for(new i = 0; i < HUD_TEXTDRAWS_SIZE; i ++) {
        TextDrawHideForPlayer(playerid, Textdraws[textdraw_HUD][i]);
    }
    for(new i = 0; i < HUD_PTEXTDRAWS_SIZE; i ++) {
        PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_HUD][i]);
    }
    SetPlayerVirtualWorld(playerid, playerid + 10);
    TogglePlayerSpectatingEx(playerid, true);
    InterpolateCameraPos(playerid, 670.198730, -2895.891113, 9.143234, 670.198730, -2895.891113, 9.143234, 1000, CAMERA_CUT);
    InterpolateCameraLookAt(playerid, 647.967712, -2911.215820, 0.253232, 647.967712, -2911.215820, 0.253232, 1000, CAMERA_CUT);
    ta_data[playerid][ta_boat] = CreateVehicle(453, 648.1311, -2910.7832, 0.0000, 354.7684, 1, 1, -1, false);
    SetVehicleParamsEx(ta_data[playerid][ta_boat], 1, 0, 0, 1, 0, 0, 0);
    ta_data[playerid][ta_actor] = CreateDynamicActor(PI[playerid][pi_SKIN], 625.4974, -2828.7190, 2.0000, 0.0000, true, 1.0, playerid + 10);
    ApplyDynamicActorAnimation(ta_data[playerid][ta_actor], "CRACK", "crckdeth2", 4.0, 0, 0, 0, 1, 0);
    ApplyDynamicActorAnimation(ta_data[playerid][ta_actor], "CRACK", "crckdeth2", 4.0, 0, 0, 0, 1, 0);
    new fmat_text[64];
    format(fmat_text, 24, "%s_NPC_TA", PLAYER_TEMP[playerid][pt_NAME]);
    ta_data[playerid][ta_npc] = FCNPC_Create(fmat_text);
    FCNPC_Spawn(ta_data[playerid][ta_npc], 4, 648.1311, -2910.7832, 0.0000);
    SetVehicleVirtualWorld(ta_data[playerid][ta_boat], playerid + 10);
    FCNPC_SetVirtualWorld(ta_data[playerid][ta_npc], playerid + 10);
    FCNPC_PutInVehicle(ta_data[playerid][ta_npc], ta_data[playerid][ta_boat], 0);
    ta_data[ ta_data[playerid][ta_npc] ][ta_npc] = playerid;
    UpdateTAMission(playerid, 4000);
    return 1;
}
