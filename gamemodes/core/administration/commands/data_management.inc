/* 

    Player and global data managemente
        Administration Commands

*/

/* Inv Objects list */
DialogCreate:ServerInvObjList(playerid) {
	new dialog[80 * 10 + 24], InvObjListPageList = GetPVarInt(playerid, "InvObjListPage");
	for (new i = InvObjListPageList; i < sizeof(g_inventory_data); i++) {
		if (i == 0) continue;
		strcatf(dialog, "{d1d1d1}#%d - %s [%d, %d]\n", i, g_inventory_data[i][g_info_name], g_inventory_data[i][g_info_extra], g_inventory_data[i][g_info_model]);

		if (i >= InvObjListPageList + 9 && sizeof(g_inventory_data) > InvObjListPageList + 10) {
			strcat(dialog, "{666666}Siguiente pag.\n");
			break;
		}
	}
	Dialog_Open(playerid, Dialog:ServerInvObjList, DIALOG_STYLE_TABLIST, "Lista de objetos de el inventario", dialog, "Continuar", "Cerrar");
}
DialogResponse:ServerInvObjList(playerid, response, listitem, inputtext[]) {
	if (response) {
		if (listitem >= 9) {
			new InvObjListPageList = GetPVarInt(playerid, "InvObjListPage");
			SetPVarInt(playerid, "InvObjListPage", (InvObjListPageList + 10));
		}
		Dialog_Show(playerid, Dialog:ServerInvObjList);
	}
	return 1;
}
CMD:objlist(playerid, params[]) {
	SetPVarInt(playerid, "InvObjListPage", 0);
	Dialog_Show(playerid, Dialog:ServerInvObjList);
	CmdLog("objlist");
	return 1;
}

/* Give Obj */
CMD:giveobj(playerid, params[]) {
	new to_playerid, objid, amnt, data, extra;
	if (sscanf(params, "udddd", to_playerid, objid, amnt, data, extra)) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/giveobj <playerid> <objid> <amnt> <data> <extra>");

	if (objid <= 0 || objid >= sizeof(g_inventory_data)) {
		return SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Error ·{ffffff}El ID de objeto '%d' no es válido.", objid);
	}

	new slot = GetPlayerInventorySlot(to_playerid);
	if (slot == -1) {
		return SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Error ·{ffffff}No tienes espacio en tu inventario.");
	}

	SetPlayerInventorySlotData(to_playerid, slot, objid, amnt, data, extra);
	InsertPlayerInventorySlot(to_playerid, slot);
	SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Inv ·{ffffff}Has dado el objeto '%s' (ID: %d) a %s (%d).", g_inventory_data[objid][g_info_name], objid, PLAYER_TEMP[to_playerid][pt_NAME], to_playerid);
	SendPlayerClientMessage(to_playerid, "{"#ADMIN_COLOR"}Inv ·{ffffff}Has recibido el objeto '%s' (ID: %d) de un administrador.", g_inventory_data[objid][g_info_name], objid);
	CmdLog("giveobj");
	return 1;
}

/* Rep and payday */
CMD:addrep(playerid, params[]) {
	new to_playerid, rep;
	if (sscanf(params, "ud", to_playerid, rep)) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/addrep <usuario> <cantidad>");

	AddPlayerReputation(to_playerid, rep);
	SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Inv ·{ffffff}Has dado +%d de experiencia a %s (%d).", rep, PLAYER_TEMP[to_playerid][pt_NAME], to_playerid);
	SendPlayerClientMessage(to_playerid, "{"#ADMIN_COLOR"}Inv ·{ffffff}Has recibido +%d de experiencia de un administrador.", rep);
	CmdLog("addrep");
	return 1;
}

CMD:payday(playerid, params[]) {
	new to_player;
	if (sscanf(params, "u", to_player)) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/payday [Player/Name]");
	PI[to_player][pi_TIME_FOR_PAYDAY] = TIME_FOR_PAYDAY;
	OnPlayerTimeUpdate(to_player);
	CmdLog("payday");
	return 1;
}

/* Works */
CMD:setwork(playerid, params[]) {
	new to_player, work, set;
	if (sscanf(params, "udd", to_player, work, set)) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/setwork <player_id> <work> <set>");
	if (!IsPlayerConnected(to_player)) return SendPlayerClientMessage(playerid, "Jugador (%d) desconectado", to_player);
	if (PI[to_player][pi_ADMIN_LEVEL] > PI[playerid][pi_ADMIN_LEVEL]) return SendPlayerClientMessage(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	if (work < 0 || work >= sizeof work_info) return SendPlayerClientMessage(playerid, "El trabajo no es válido, para verlos usa /works.");
	if (PLAYER_TEMP[to_player][pt_WORKING_IN]) return SendPlayerClientMessage(playerid, "No se puede cambiar el trabajo del jugador porque está de servicio en su trabajo.");
	if (set > 1) set = 1;
	new player_jobs = CountPlayerJobs(to_player);
	if (PI[to_player][pi_VIP]) {
		if (player_jobs >= MAX_SU_WORKS && set) {
			SendPlayerClientMessage(playerid, "El limite de trabajos para este jugador es %d.", player_jobs);
			return 1;
		}
	} else {
		if (player_jobs >= MAX_NU_WORKS && set) {
			SendPlayerClientMessage(playerid, "El limite de trabajos para este jugador es %d.", player_jobs);
			return 1;
		}
	}
	if (set) {
		if (work == WORK_POLICE) {
			mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pworks (id_player, id_work, `set`, level) VALUES(%d, %d, 1, 1) ON DUPLICATE KEY UPDATE `set` = 1, level = 1;", PI[to_player][pi_ID], work);
			mysql_tquery(handle_db, QUERY_BUFFER);
			PI[to_player][pi_PLACA_PD] = random(10000000);
			PLAYER_WORKS[to_player][work][pwork_LEVEL] = 1;
			mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET placa_pd = %d WHERE id = %d;", PI[to_player][pi_PLACA_PD], PI[to_player][pi_ID]);
			mysql_tquery(handle_db, QUERY_BUFFER);
		} else {
			mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pworks (id_player, id_work, `set`, level) VALUES(%d, %d, 1, 1) ON DUPLICATE KEY UPDATE `set` = 1;", PI[to_player][pi_ID], work);
			mysql_tquery(handle_db, QUERY_BUFFER);
		}
		PLAYER_WORKS[to_player][work][pwork_SET] = 1;
		SendClientMessageEx(to_player, -1, "{"#ADMIN_COLOR"}Aviso·{ffffff} Ahora eres {"#ADMIN_COLOR"}%s{ffffff}.", work_info[work][work_info_NAME]);
	} else {
		if (work == WORK_POLICE) {
			mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET `set` = 0, level = 0 WHERE id_player = %d AND id_work = %d;", PI[to_player][pi_ID], work);
			mysql_tquery(handle_db, QUERY_BUFFER);
			PI[to_player][pi_PLACA_PD] = 0;
			PLAYER_WORKS[to_player][work][pwork_LEVEL] = 0;
			mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE player SET placa_pd = %d WHERE id = %d;", PI[to_player][pi_PLACA_PD], PI[to_player][pi_ID]);
			mysql_tquery(handle_db, QUERY_BUFFER);
		} else {
			mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET `set` = 0 WHERE id_player = %d AND id_work = %d;", PI[to_player][pi_ID], work);
			mysql_tquery(handle_db, QUERY_BUFFER);
		}

		PLAYER_WORKS[to_player][work][pwork_SET] = 0;
		SendClientMessageEx(to_player, -1, "{"#ADMIN_COLOR"}Aviso·{ffffff} Se te ha removido el trabajo de {"#ADMIN_COLOR"}%s{ffffff}.", work_info[work][work_info_NAME]);
	}
	SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Aviso·{ffffff} El trabajo %s de %s (%d) es: '%d'.", work_info[ work ][work_info_NAME], PI[to_player][pi_NAME], to_player, set);
	SendCmdLogToAdmins(playerid, "setwork", params);
	return 1;
}

CMD:setworkexp(playerid, params[]) {
	new to_player, work, exp;
	if (sscanf(params, "udd", to_player, work, exp)) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/setworkexp <player_id> <work, para verlos /works> <exp>");
	if (!IsPlayerConnected(to_player)) return SendPlayerClientMessage(playerid, "Jugador (%d) desconectado", to_player);
	if (PI[to_player][pi_ADMIN_LEVEL] > PI[playerid][pi_ADMIN_LEVEL]) return SendPlayerClientMessage(playerid, "El rango administrativo de este jugador es superior al tuyo.");
	
	if (work < 0 || work >= sizeof work_info) return SendPlayerClientMessage(playerid, "El trabajo no es válido, para verlos usa /works.");
	
	if (PLAYER_WORKS[to_player][ work ][pwork_SET]) {
		mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "UPDATE pworks SET level = %d WHERE id_player = %d AND id_work = %d;", exp, PI[to_player][pi_ID], work);
		mysql_tquery(handle_db, QUERY_BUFFER);

		PLAYER_WORKS[to_player][ work ][pwork_LEVEL] = exp;
		SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Aviso·{ffffff} La experiencia de {"#ADMIN_COLOR"}%s (%d){ffffff} en el trabajo '%s' ahora es %d.", PI[to_player][pi_NAME], to_player, work_info[ work ][work_info_NAME], exp);
		SendCmdLogToAdmins(playerid, "setworkexp", params);
	}
	else SendPlayerClientMessage(playerid, "El jugador no tiene el trabajo");
	return 1;
}
