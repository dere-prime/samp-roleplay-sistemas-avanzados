#include <YSI_Coding\y_hooks>

new pInjuredTimer[MAX_PLAYERS] = {-1, ...};

hook OnPlayerDisconnect(playerid, reason) {
    if(pInjuredTimer[playerid] != -1) {
        KillTimer(pInjuredTimer[playerid]);
        pInjuredTimer[playerid] = -1;
    }
}

forward OnPlayerDeathRefresh(playerid);
public OnPlayerDeathRefresh(playerid) {
    if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) {
        SetPlayerHud(playerid);
        if(PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_DEAD) SetPlayerHealthEx(playerid, 100.0);
        SetCameraBehindPlayer(playerid);
        SendAlertToMedics(playerid);
        
        if(pInjuredTimer[playerid] != -1) {
            KillTimer(pInjuredTimer[playerid]);
            pInjuredTimer[playerid] = -1;
        }
        pInjuredTimer[playerid] = SetTimerEx("UpdatePlayerInjured", 3000, false, "i", playerid);
        
        if(GetPlayerAnimationIndex(playerid) != PLAYER_TEMP[playerid][pt_DEATH_ANIMINDEX]) ApplyAnimation(playerid, PLAYER_TEMP[playerid][pt_DEATH_ANIMLIB], PLAYER_TEMP[playerid][pt_DEATH_ANIMNAME], 4.1, false, false, false, true, 0, 1);
        if(PI[playerid][pi_WANTED_LEVEL] > 0) SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Has sido abatido, no podras hacer nada hasta que la {"#ERROR_COLOR"}policia te arreste{ffffff}.");
        else {
            SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Estas abatido. Espera a un {"#ERROR_COLOR"}medico{ffffff} para recibir atencion.");
            SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}·{ffffff}Pulsa {"#ERROR_COLOR"}Y{ffffff} para morir pagando 5000{"#ERROR_COLOR"}${ffffff} si no quieres esperar.");
        }
    }
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) {
        if(newkeys & KEY_YES) {
            if (gettime() > PLAYER_TEMP[playerid][pt_DEATH_TIME] + 30) {
                Dialog_Show(playerid, Dialog:DeathConfirmation);
            } else {
                ShowPlayerMessage(playerid, "Debes esperar 30 segundos desde la ultima vez que moriste.");
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
            SetPlayerHealthEx(playerid, 0.0);
            SendPlayerClientMessage(playerid, "{"#GREEN_COLOR"}·{ffffff}Has decidido morir y pagar 5000{"#GREEN_COLOR"}${ffffff}.");
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