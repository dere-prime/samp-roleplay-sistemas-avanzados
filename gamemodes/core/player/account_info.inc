/* 

    Informacion de la cuenta
        y otros.

    Prime Roleplay

*/

#include <mdialog>

/* Player Stats Dialog */
stock ShowPlayerStats(playerid, pid) {
	if (!IsPlayerConnected(pid)) return 0;
	if (PI[pid][pi_ID] == 0) return 0;
	new caption[48], dialog[800 + (27 * sizeof(work_info))];
	format(caption, sizeof caption, "Estadisticas · {"#PRIMARY_COLOR"}%s", PLAYER_TEMP[pid][pt_NAME]);
	strcatf(dialog, "{f0f0f0}ID: {"#GREEN_COLOR"}%d\n", PI[pid][pi_ID]);
	strcatf(dialog, "{f0f0f0}Nombre de la Cuenta: {"#GREEN_COLOR"}%s\n", PI[pid][pi_NAME]);
	strcatf(dialog, "{f0f0f0}Nombre InGame: {"#GREEN_COLOR"}%s\n", PLAYER_TEMP[pid][pt_NAME]);
	strcatf(dialog, "{f0f0f0}Correo electronico: {"#GREEN_COLOR"}%s\n", PI[pid][pi_EMAIL]);
	strcat(dialog, "{000000}\n");
	strcatf(dialog, "{f0f0f0}Ultima conexion: {"#GREEN_COLOR"}%s\n", PI[pid][pi_LAST_CONNECTION]);
	strcatf(dialog, "{f0f0f0}Tiempo jugado: {"#GREEN_COLOR"}%s\n", TimeConvertAsTextEx(PI[pid][pi_TIME_PLAYING]));
	strcatf(dialog, "{f0f0f0}Nivel: {"#SECONDARY_COLOR"}%d {f0f0f0}- {"#SECONDARY_COLOR"}%d{f0f0f0}/{"#SECONDARY_COLOR"}%d\n", PI[pid][pi_LEVEL], PI[pid][pi_REP], (PI[pid][pi_LEVEL] * REP_MULTIPLIER));
	strcat(dialog, "{000000}\n");
	strcatf(dialog, "{f0f0f0}Trabajo(s): %s\n", getPlayerWorks(pid));
	strcatf(dialog, "{f0f0f0}%s\n", getPlayerCrew(pid));
	strcatf(dialog, "{f0f0f0}Nivel de permisos: {"#ADMIN_COLOR"}%s\n", ADMIN_LEVELS[ PI[pid][pi_ADMIN_LEVEL] ]);
	strcat(dialog, "{000000}\n");
	strcatf(dialog, "{f0f0f0}Dinero Efectivo: %s{"#GREEN_COLOR"}$\n", number_format_thousand(PI[pid][pi_CASH]));
	strcatf(dialog, "{f0f0f0}Dinero en el Banco: %s{"#BLUE_COLOR"}$\n", number_format_thousand(PI[pid][pi_BANK_MONEY]));
	strcatf(dialog, "{f0f0f0}"SERVER_COIN": {"#PRIMARY_COLOR"}%d\n", PI[pid][pi_COINS]);
	strcatf(dialog, "{f0f0f0}Membresia: %s\n", getPlayerVip(pid));
	Dialog_Message(playerid, DIALOG_STYLE_TABLIST, caption, dialog, "Cerrar");
	return 1;
}

ShowPlayerSkills(playerid, pid) {
	if (!IsPlayerConnected(pid)) return 0;
	if (PI[pid][pi_ID] == 0) return 0;
	new caption[48], dialog[80 * (sizeof(work_info) + 5)];
	format(caption, sizeof caption, "Experiencia · {"#PRIMARY_COLOR"}%s", PLAYER_TEMP[pid][pt_NAME]);
	strcat(dialog, "{"#PRIMARY_COLOR"}·{f0f0f0}TRABAJOS\n");
	for(new i = 1; i != sizeof work_info; i ++) {
		if (i == WORK_POLICE) {
			if (PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) {
				strcatf(dialog, "{f0f0f0}%c%s: {"#SECONDARY_COLOR"}%s\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], POLICE_RANKS[PLAYER_WORKS[pid][i][pwork_LEVEL]]);
			}
		} else {
			strcatf(dialog, "{f0f0f0}%c%s: {"#SECONDARY_COLOR"}%s\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], number_format_thousand(PLAYER_WORKS[pid][i][pwork_LEVEL]));
		}
	}
	Dialog_Message(playerid, DIALOG_STYLE_TABLIST, caption, dialog, "Cerrar");
	return 1;
}