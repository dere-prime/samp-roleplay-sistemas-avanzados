/* 

    Player and global information commands
		Administration

*/

/* Get player number info */
CMD:infnum(playerid, params[]) {
	if (sscanf(params, "d", params[0])) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/infnum <numero>");
	
	inline OnInfoQueryLoad() {
		new rows;
		if (cache_get_row_count(rows)) {
			if (rows) {
				new id, name[24], connected, pid;
				cache_get_value_name_int(0, "id_player", id);
				cache_get_value_name(0, "name", name);
				cache_get_value_name_int(0, "connected", connected);
				cache_get_value_name_int(0, "pid", pid);
				SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Resultado ·{f0f0f0}PN: %d --- Nombre: '%s' DB-ID: '%d' Conectado: '%d' Playerid: '%d'", params[0], name, id, connected, pid);
			}
			else SendPlayerClientMessage(playerid, "{"#ADMIN_COLOR"}Aviso ·{f0f0f0}No se encontró ningún resultado para ese número.");
		}	
	}
	mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT player.id AS id_player, player.connected AS connected, player.playerid AS pid, player_inventory.id_player, player_inventory.type, player_inventory.data FROM player_inventory LEFT JOIN player ON player.id = player_inventory.id_player WHERE player_inventory.type = %d AND player_inventory.data = %d;", INVENTORY_PHONE, params[0]);
	MySQL_TQueryInline(handle_db, using inline OnInfoQueryLoad, QUERY_BUFFER);

    CmdLog("infnum");
	return 1;
}