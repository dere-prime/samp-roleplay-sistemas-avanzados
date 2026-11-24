/* 

    Comandos de moderacion de usuarios.

*/

/* DUTY */
CMD:duty(playerid) {
	if (PLAYER_TEMP[playerid][pt_ADMIN_SERVICE]) {
		PLAYER_TEMP[playerid][pt_ADMIN_SERVICE] = false;
		SendPlayerClientMessage(playerid, "Ahora no estás de servicio como %s.", ADMIN_LEVELS[ PI[playerid][pi_ADMIN_LEVEL] ]);
	} else {
		PLAYER_TEMP[playerid][pt_ADMIN_SERVICE] = true;
		SendPlayerClientMessage(playerid, "Ahora estás de servicio como %s.", ADMIN_LEVELS[ PI[playerid][pi_ADMIN_LEVEL] ]);
	}
	return 1;
}

/* DoubtMute */
CMD:dmute(playerid, params[]) {
    new to_playerid, reason[128], time;
    if (sscanf(params, "uds[128]", to_playerid, time, reason)) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/dmute <player_id> <minutos> <razon>");
	if (time < 0 || time > 1440) return SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Error ·{f0f0f0}Intervalo de minutos incorrecto.");
    if (!IsPlayerConnected(to_playerid)) return SendPlayerClientMessage(playerid, "{666666}Jugador desconectado");
    if (PI[to_playerid][pi_ADMIN_LEVEL] > PI[playerid][pi_ADMIN_LEVEL]) return SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Error ·{f0f0f0}El rango administrativo de este jugador es superior al tuyo.");
	if (gettime() < PI[to_playerid][pi_DOUBT_MUTE]) return SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Error ·{f0f0f0}El jugador ya está muteado.");
	
    new seconds = time * 60;
	PI[to_playerid][pi_DOUBT_MUTE] = gettime() + seconds;

	SendClientMessageEx(to_playerid, -1, "{"#ADMIN_COLOR"}Aviso ·{f0f0f0}Has sido silenciado del canal de dudas por {"#ADMIN_COLOR"}%d{f0f0f0} minutos, razon: {"#ADMIN_COLOR"}%s{f0f0f0}", time, reason);
    SendAdminPlayerModerationAd(playerid, to_playerid, "Admin - Canal Dudas", "Silenciado", reason, true, time);

    CmdLog("dmute");
	return 1;
}

CMD:dunmute(playerid, params[]) {
    new to_playerid, reason[128];
    if (sscanf(params, "us[128]", to_playerid, reason)) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/dunmute <player_id> <razon>");
    if (!IsPlayerConnected(to_playerid)) return SendPlayerClientMessage(playerid, "{666666}Jugador desconectado");
    if (PI[to_playerid][pi_ADMIN_LEVEL] > PI[playerid][pi_ADMIN_LEVEL]) return SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Error ·{f0f0f0}El rango administrativo de este jugador es superior al tuyo.");
	if (gettime() > PI[to_playerid][pi_DOUBT_MUTE]) return SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Error ·{f0f0f0}Este jugador no está silenciado.");
	
	PI[to_playerid][pi_DOUBT_MUTE] = 0;
	SendClientMessage(to_playerid, -1, "{"#ADMIN_COLOR"}Aviso ·{f0f0f0}Ya puedes volver a enviar dudas.");
	SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Aviso ·{f0f0f0}%s (%d) ha sido des-silenciado.", PI[to_playerid][pi_NAME], to_playerid);
    SendAdminPlayerModerationAd(playerid, to_playerid, "Admin - Canal Dudas", "Des-silenciado", reason);

    CmdLog("dunmute");
	return 1;
}

/* Work teleport */
CMD:worktp(playerid) {
	Dialog_Show(playerid, Dialog:AdminWorkTeleport);
	return 1;
}
DialogCreate:AdminWorkTeleport(playerid) {
    for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

    new dialog[85 * (sizeof(work_info) + 1)], listitem;
    for(new i = 1; i != sizeof work_info; i ++) {
        strcatf(dialog, "{"#SECONDARY_COLOR"}%c%s {f0f0f0}(id: %d)\t{"#SECONDARY_COLOR"}%d\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], i, work_info[i][work_info_LEVEL]);
        PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] = i;
        listitem ++;
    }
    Dialog_Open(playerid, Dialog:AdminWorkTeleport, DIALOG_STYLE_TABLIST, "Trabajos", dialog, "Ir", "Cerrar");
    return 1;
}
DialogResponse:AdminWorkTeleport(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return true;
        else {
            SetPlayerPosEx(playerid, obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_X], obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_Y], obtain_work_coords[ PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ][obtain_work_MAP_ICON_Z], 0.0, 0, 0, false);
	        SendClientMessage(playerid, -1, "{"#ADMIN_COLOR"}Aviso ·{f0f0f0}Te has teletransportado a la ubicación de un trabajo.");
        }
    }
    return 1;
}