#include <YSI_Coding\y_hooks>

new pInjuredTimer[MAX_PLAYERS] = {-1, ...};

hook OnPlayerDisconnect(playerid, reason) {
    if(pInjuredTimer[playerid] != -1) {
        KillTimer(pInjuredTimer[playerid]);
        pInjuredTimer[playerid] = -1;
    }
}

public OnPlayerPrepareDeath(playerid, a_lib[32], a_name[32], &anim_lock, killerid, reason) {
	if (PLAYER_TEMP[playerid][pt_KICKED] || !PLAYER_TEMP[playerid][pt_USER_LOGGED]) return 0;

	DropEquippedCurrentWeapon(playerid);
	OnPlayerDeathAFK(playerid);

	PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 5;
	
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][16]);
	PLAYER_AC_INFO[playerid][CHEAT_POS][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_STATE_SPAMMER][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_PLAYER_HEALTH][p_ac_info_IMMUNITY] = gettime() + 3;
	PLAYER_AC_INFO[playerid][CHEAT_VEHICLE_NOFUEL][p_ac_info_IMMUNITY] = gettime() + 15;
	if (!PI[playerid][pi_VIP]) SetPlayerArmourEx(playerid, 0.0);

	foreach (new admin : Player) {
		if (PI[admin][pi_ADMIN_LEVEL] < 2) continue;
		if (!PLAYER_TEMP[admin][pt_SEE_ADM_LOG]) continue;
		SendDeathMessageToPlayer(admin, killerid, playerid, reason);
	}
	
	if (ac_Info[CHEAT_DEATH_SPAMMER][ac_Enabled]) {
		if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_IMMUNITY]) {
			new interval = GetTickDiff(GetTickCount(), PLAYER_TEMP[playerid][pt_ANTIFLOOD_DEATH]);
			if (interval < 50) {
				if (!ac_Info[CHEAT_DEATH_SPAMMER][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_DEATH_SPAMMER, float(interval));
				else {
					if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_DEATH_SPAMMER][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_DETECTIONS] = 0;
					else PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_DETECTIONS] ++;
					
					PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_LAST_DETECTION] = gettime();
					if (PLAYER_AC_INFO[playerid][CHEAT_DEATH_SPAMMER][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_DEATH_SPAMMER][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_DEATH_SPAMMER, float(interval));
				}
			}
			PLAYER_TEMP[playerid][pt_ANTIFLOOD_DEATH] = GetTickCount();
		}
	}
	
	HidePlayerMessage(playerid);
	
	if (IsPlayerConnected(killerid) && PI[killerid][pi_STATE] == ROLEPLAY_STATE_NORMAL && PI[playerid][pi_STATE] == ROLEPLAY_STATE_NORMAL) {
		if (!PLAYER_WORKS[killerid][WORK_POLICE][pwork_SET]) {
			if (IsPlayerInSafeZone(killerid)) {
				SendClientMessage(killerid, -1, "Estás en una zona segura, la policía ha sido avisada del homicidio.");
				
				new level = PI[killerid][pi_WANTED_LEVEL] + 3;
				if (level > 6) level = 6;

				SetPlayerWantedLevelEx(killerid, level);
				
				new city[45], zone[45];
				GetPlayerZones(killerid, city, zone);

				new message[145];
				format(message, sizeof message, "{"#PRIMARY_COLOR"}[Central policía] {FFFFFF}%s ha causado un asesinato en {"#PRIMARY_COLOR"}%s, %s.", PLAYER_TEMP[killerid][pt_NAME], city, zone);
				SendPoliceRadioMessage(-1, -1, message);
				
				PLAYER_TEMP[killerid][pt_LAST_SAFE_ZONE_WARNING] = gettime();
			}
		}
	}
	
	if (PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW]) {
		if (PLAYER_TEMP[playerid][pt_CLOTHING_SHOP] != -1) ClosePlayerClothingMenu(playerid);
		if (PLAYER_TEMP[playerid][pt_TOYS_SHOP]) ClosePlayerToysMenu(playerid);
	}
	
	StopAudioStreamForPlayer(playerid);
	SetPlayerDrunkLevel(playerid, 0);
	KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][3]);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	GetPlayerPos(playerid, PI[playerid][pi_POS_X], PI[playerid][pi_POS_Y], PI[playerid][pi_POS_Z]);
	GetPlayerFacingAngle(playerid, PI[playerid][pi_ANGLE]);
	PI[playerid][pi_INTERIOR] = GetPlayerInterior(playerid);
	CancelEdit(playerid);
	HidePlayerDialog(playerid);
	SetNormalPlayerMarkers(playerid);
	if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_POLICE) CallLocalFunction("EndPlayerJob", "iib", playerid, PLAYER_TEMP[playerid][pt_WORKING_IN], false);
	PLAYER_TEMP[playerid][pt_HUNGRY_MESSAGE] = false;
	PLAYER_TEMP[playerid][pt_THIRST_MESSAGE] = false;
	PLAYER_TEMP[playerid][pt_PLAYER_IN_ATM] = false;
	PLAYER_TEMP[playerid][pt_CUFFED] = false;
	PLAYER_TEMP[playerid][pt_CUFFING] = false;
	if (PLAYER_TEMP[playerid][pt_WANT_TAXI]) {
		PLAYER_TEMP[playerid][pt_WANT_TAXI] = false;
		DisablePlayerTaxiMark(playerid);
	}
	if (PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) EndPhoneCall(playerid);
	if (PI[playerid][pi_WANTED_LEVEL] > 0) DisablePlayerPoliceMark(playerid);
	if (PLAYER_TEMP[playerid][pt_TUNING_GARAGE_SHOP]) CancelPlayerTuningShop(playerid);
	if (PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE]) {
		PLAYER_TEMP[playerid][pt_IN_TUNING_GARAGE] = false;
		SetVehicleVirtualWorldEx(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 0);
		SetVehiclePosEx(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 246.551910, 17.474195, 2.363841);
		SetVehicleZAngle(PLAYER_TEMP[playerid][pt_TUNING_GARAGE_VEHICLEID], 0.0);
	}

	if (PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] != INVALID_VEHICLE_ID) {
		if (TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_JOB_STARTED]) {
			if (PLAYER_TEMP[playerid][pt_TRASH_DRIVER]) {
				ShowPlayerMessage(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], "El trabajo se ha cancelado porque tu compañero ha dejado de trabajar.");
				CancelTrashWork(playerid, TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_PASSENGER_ID], PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
			}
			if (PLAYER_TEMP[playerid][pt_TRASH_PASSENGER]) {
				ShowPlayerMessage(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], "El trabajo se ha cancelado porque tu compañero ha dejado de trabajar.");
				CancelTrashWork(TRASH_VEHICLES[ PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID] ][trash_vehicle_DRIVER_ID], playerid, PLAYER_TEMP[playerid][pt_TRASH_VEHICLE_ID]);
			}
		}
	}
	
    PLAYER_TEMP[playerid][pt_ALLOW_DEATH] = false;
    PLAYER_TEMP[playerid][pt_DEATH_TIME] = gettime();
    PI[playerid][pi_STATE] = ROLEPLAY_STATE_CRACK;

    SetPlayerHud(playerid);
    SendAlertToMedics(playerid);
    
    if(pInjuredTimer[playerid] != -1) {
        KillTimer(pInjuredTimer[playerid]);
        pInjuredTimer[playerid] = -1;
    }
    pInjuredTimer[playerid] = SetTimerEx("UpdatePlayerInjured", 3000, false, "i", playerid);
    
    if(PI[playerid][pi_WANTED_LEVEL] > 0) SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Has sido abatido, no podras hacer nada hasta que la {"#ERROR_COLOR"}policia te arreste{ffffff}.");
    else {
        SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Estas abatido. Espera a un {"#ERROR_COLOR"}medico{ffffff} para recibir atencion.");
        SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Pulsa {"#ERROR_COLOR"}Y{ffffff} para morir pagando 5000{"#ERROR_COLOR"}${ffffff} si no quieres esperar.");
    }

	UpdatePlayerRoleplayNameInfo(playerid);
   	return 1;
}

hook OnPlayerSpawn(playerid) {
    if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) {
        SetPlayerHud(playerid);
        SetPlayerHealthEx(playerid, 100.0);
        SetCameraBehindPlayer(playerid);
        SendAlertToMedics(playerid);
        
        if(pInjuredTimer[playerid] != -1) {
            KillTimer(pInjuredTimer[playerid]);
            pInjuredTimer[playerid] = -1;
        }
        pInjuredTimer[playerid] = SetTimerEx("UpdatePlayerInjured", 3000, false, "i", playerid);
        
        if(PI[playerid][pi_WANTED_LEVEL] > 0) SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Has sido abatido, no podras hacer nada hasta que la {"#ERROR_COLOR"}policia te arreste{ffffff}.");
        else {
            SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Estas abatido. Espera a un {"#ERROR_COLOR"}medico{ffffff} para recibir atencion.");
            SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Pulsa {"#ERROR_COLOR"}Y{ffffff} para morir pagando 5000{"#ERROR_COLOR"}${ffffff} si no quieres esperar.");
        }

        FreezePlayer(playerid, 500);
    }
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) {
        if(newkeys & KEY_YES) {
            if (gettime() > PLAYER_TEMP[playerid][pt_DEATH_TIME] + 5) {
                Dialog_Show(playerid, Dialog:DeathConfirmation);
            } else {
                ShowPlayerTimeOut(PLAYER_TEMP[playerid][pt_DEATH_TIME], 5, playerid, "Debes esperar ~b~%s~w~ para poder ser enviado al hospital.");
            }
            return Y_HOOKS_BREAK_RETURN_1;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

DialogCreate:DeathConfirmation(playerid) {
    Dialog_Open(playerid, Dialog:DeathConfirmation, DIALOG_STYLE_MSGBOX, "{"#WARNING_COLOR"}·{ffffff}Confirmacion de muerte", "{"#WARNING_COLOR"}·{ffffff}¿Estas seguro que quieres morir?\n\nSe te cobrara 5000{"#ERROR_COLOR"}${ffffff}\nPerderas todas tus {"#ERROR_COLOR"}pertenencias ilegales{ffffff}.", "Morir", "Esperar");
}
DialogResponse:DeathConfirmation(playerid, response, listitem, inputtext[]) {
    if(response) {
        if(GivePlayerCash(playerid, -5000, true, true)) {
            Auto_SendPlayerAction(playerid, "ha aceptado su muerte.");
            PLAYER_TEMP[playerid][pt_ALLOW_DEATH] = true;
            SendPlayerClientMessage(playerid, "{"#GREEN_COLOR"}·{ffffff}Has decidido morir y pagar 5000{"#GREEN_COLOR"}${ffffff}.");

            if (PLAYER_TEMP[playerid][pt_ALLOW_DEATH] && PI[playerid][pi_WANTED_LEVEL] <= 0 && (PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK || GetPlayerInterior(playerid) != 0 || GetPlayerVirtualWorld(playerid) != 0)) {
                PLAYER_TEMP[playerid][pt_HOSPITAL] = GetNearestHospitalForPlayer(playerid);

                PI[playerid][pi_STATE] = ROLEPLAY_STATE_HOSPITAL;
                DisablePlayerInjuredMark(playerid);

                new Float:pos[4];
                GetHospitalSpawnPosition(PLAYER_TEMP[playerid][pt_HOSPITAL], pos[0], pos[1], pos[2], pos[3], PI[playerid][pi_INTERIOR], PI[playerid][pi_LOCAL_INTERIOR]);

                DisablePlayerActiveStaticPos(playerid);
                PI[playerid][pi_LOCAL_INTERIOR] = 0;
                PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = -1;
                SetSpawnInfo(playerid, NO_TEAM, PI[playerid][pi_SKIN], pos[0], pos[1], pos[2], pos[3], 0, 0, 0, 0, 0, 0);
                SetPlayerPosEx(playerid, pos[0], pos[1], pos[2], pos[3], PI[playerid][pi_INTERIOR], 0);

                TogglePlayerSpectatingEx(playerid, true);
                SetPlayerInterior(playerid, 0);
                SetPlayerVirtualWorld(playerid, 0);
                PLAYER_TEMP[playerid][pt_PLAYER_FINISH_HOSPITAL] = false;
                PI[playerid][pi_STATE] = ROLEPLAY_STATE_HOSPITAL;
                PI[playerid][pi_LOCAL_INTERIOR] = 0;
                if (PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] != -1) {
                    CallLocalFunction("OnPlayerExitInterior", "ii", playerid, PLAYER_TEMP[playerid][pt_INTERIOR_INDEX]);
                }
                PLAYER_TEMP[playerid][pt_INTERIOR_INDEX] = -1;
                PLAYER_TEMP[playerid][pt_PROPERTY_INDEX] = -1;
                
                PLAYER_TEMP[playerid][pt_HOSPITAL_LIFE] = 35.0;
                UpdateHospitalSizeTextdrawLife(playerid);
                KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][4]);
                PLAYER_TEMP[playerid][pt_TIMERS][4] = SetTimerEx("HealthUp", 1000, false, "i", playerid);

                DeleteIlegalInv(playerid);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][0]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][1]);
                PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_HOSPITAL][2]);
                
                ClearPlayerChat(playerid);
                SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Has sido trasladado al {"#ERROR_COLOR"}centro medico{ffffff} mas cercano. Espera tu recuperacion.");
                
                InterpolateCameraPos(playerid, Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][0], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][1], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][2], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][0], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][1], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][2], 1000, CAMERA_CUT);
                InterpolateCameraLookAt(playerid, Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][3], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][4], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][5], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][3], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][4], Hospital_Cameras_Positions[PLAYER_TEMP[playerid][pt_HOSPITAL]][5], 1000, CAMERA_CUT);
            }
        } else {
            SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}No tienes suficiente dinero para morir. Necesitas 5000{"#ERROR_COLOR"}${ffffff}.");
        }
    }
    return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) {
    if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) {
		new Float:sx, Float:sy, Float:sz;
	    GetPlayerPos(playerid, sx, sy, sz);
        RemovePlayerFromVehicle(playerid);
		SetPlayerPos(playerid, sx, sy, sz);
		if(GetPlayerAnimationIndex(playerid) != PLAYER_TEMP[playerid][pt_DEATH_ANIMINDEX]) ApplyAnimation(playerid, PLAYER_TEMP[playerid][pt_DEATH_ANIMLIB], PLAYER_TEMP[playerid][pt_DEATH_ANIMNAME], 4.1, false, false, false, true, 0, 1);
		return Y_HOOKS_BREAK_RETURN_1;
	}
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerUpdate(playerid) {
    if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK && PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL) {
        if(GetPlayerAnimationIndex(playerid) != PLAYER_TEMP[playerid][pt_DEATH_ANIMINDEX]) UpdatePlayerInjured(playerid);
        if (!IsPlayerActiveStaticPos(playerid)) SetPlayerActiveStaticPos(playerid);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

forward UpdatePlayerInjured(playerid);
public UpdatePlayerInjured(playerid) {
    if(pInjuredTimer[playerid] != -1) {
        KillTimer(pInjuredTimer[playerid]);
        pInjuredTimer[playerid] = -1;
    }
    
	if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) {
        if(GetPlayerAnimationIndex(playerid) != PLAYER_TEMP[playerid][pt_DEATH_ANIMINDEX]) ApplyAnimation(playerid, PLAYER_TEMP[playerid][pt_DEATH_ANIMLIB], PLAYER_TEMP[playerid][pt_DEATH_ANIMNAME], 4.1, false, false, false, true, 0, 1);
        pInjuredTimer[playerid] = SetTimerEx("UpdatePlayerInjured", 3000, false, "i", playerid);
    }
	return 1;
}