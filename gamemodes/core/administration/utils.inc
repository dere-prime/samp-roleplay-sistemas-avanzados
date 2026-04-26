/* 

    Utils functions for administration gamemode.

*/

#define ADMIN_COLOR "C41E3A"
#define ADMIN_LOG_COLOR "D2042D"
#define CmdLog(%0) SendCmdLogToAdmins(playerid,%0,params)

/* Amdministration messages for all players */
stock SendAdminAd(const text[]) {
	foreach (new i : Player) {
		if (IsPlayerConnected(i)) {
			if (/*!IsPlayerMission(i) && */(PLAYER_TEMP[i][pt_GAME_STATE] == GAME_STATE_NORMAL) && PI[i][pi_CONFIG_ADMIN]) {
				SendPlayerClientMessage(i, text);
			}
		}
	}
	return 1;
}

stock SendAdminPlayerModerationAd(const admin_id, const to_playerid, const header[], const action[], const reason[], bool:use_time = false, time = 0) {
    new fmat_message[256];
    if (use_time) format(fmat_message, sizeof fmat_message, "{"#ADMIN_COLOR"}(%s) %s > %s (%d) - %s: %s (Tiempo: %d minutos)", header, PI[admin_id][pi_NAME], PI[to_playerid][pi_NAME], to_playerid, action, reason, time);
    else format(fmat_message, sizeof fmat_message, "{"#ADMIN_COLOR"}(%s) %s > %s (%d) - %s: %s", header, PI[admin_id][pi_NAME], PI[to_playerid][pi_NAME], to_playerid, action, reason);
    SendAdminAd(fmat_message);
    return 1;
}

/* Administration messages for administrators */
stock SendCmdLogToAdmins(playerid, cmdtext[], params[]) {
	foreach (new i : Player) {
        if (IsPlayerConnected(i) && PLAYER_TEMP[i][pt_USER_LOGGED]) {
            if (PI[i][pi_ADMIN_LEVEL] >= PI[playerid][pi_ADMIN_LEVEL] && PLAYER_TEMP[i][pt_SEE_ACMD_LOG]) {
                if (isnull(params)) SendPlayerClientMessage(i, "{"#ADMIN_LOG_COLOR"}(LOG - %s) - %s (%d) > /%s %s", ADMIN_LEVELS[ PI[i][pi_ADMIN_LEVEL] ], PI[playerid][pi_NAME], playerid, cmdtext, params);
                else SendPlayerClientMessage(i, "{"#ADMIN_LOG_COLOR"}(LOG - %s) - %s (%d) > /%s %s", ADMIN_LEVELS[ PI[i][pi_ADMIN_LEVEL] ], PI[playerid][pi_NAME], playerid, cmdtext, params);
            }
        }
    }
	return 1;
}