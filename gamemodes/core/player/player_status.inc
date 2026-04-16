/* 

    Estados de trabajo, y mas.
        Prime Roleplay

*/

SetPlayerNormalColor(playerid) {
	SetPlayerColorEx(playerid, PLAYER_COLOR);
	return 1;
}

Text3D:SetPlayerStatusLabel(playerid, const status[], GLOBAL_TAG_TYPES:...) {
	DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_STATUS_LABEL]);
	if (YSI_CheckNumargs__(2))
		return Text3D:PLAYER_TEMP[playerid][pt_STATUS_LABEL] = CreateDynamic3DTextLabel(__const(status), -1, 0.0, 0.0, 0.3, 20.0, playerid, .testlos = true);
	return
		format(YSI_UNSAFE_HUGE_STRING, YSI_UNSAFE_HUGE_LENGTH, status, ___(2)),
		Text3D:PLAYER_TEMP[playerid][pt_STATUS_LABEL] = CreateDynamic3DTextLabel(YSI_UNSAFE_HUGE_STRING, -1, 0.0, 0.0, 0.3, 20.0, playerid, .testlos = true);
}

RemovePlayerStatusLabel(playerid) {
	if (IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_STATUS_LABEL])) {
		DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_STATUS_LABEL]);
		PLAYER_TEMP[playerid][pt_STATUS_LABEL] = Text3D:INVALID_STREAMER_ID;
	}
	return 1;
}

forward UpdatePlayerRoleplayStatus(const playerid);
public UpdatePlayerRoleplayStatus(const playerid) {
    if (!PLAYER_TEMP[playerid][pt_USER_LOGGED] && PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return 0;

    if (PI[playerid][pi_WANTED_LEVEL] > 0) {
        SetWantedMarkerToPolice(playerid);
    }
    else if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) SendAlertToMedics(playerid);
    if (IsPlayerWorking(playerid, WORK_TAXI)) SetTaxiPlayerMarkers(playerid);

    if (PLAYER_TEMP[playerid][pt_ADMIN_SERVICE]) { SetPlayerStatusLabel(playerid, "{"#ADMIN_COLOR"}%s", ADMIN_LEVELS[ PI[playerid][pi_ADMIN_LEVEL] ]); return 1; }
    if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) { SetPlayerStatusLabel(playerid, "{"#RED_COLOR"}Abatido"); return 1; }
    if (PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] > 5.0) { SetPlayerStatusLabel(playerid, "{"#SECONDARY_COLOR"}Plantando..."); return 1; }
    if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE) {
        switch (PLAYER_TEMP[playerid][pt_WORKING_IN]) {
            default: {
                SetPlayerStatusLabel(playerid, "{%s}%c%s", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_COLOR], toupper(work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME][0]), work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME][1]);
            }
        }
        return 1;
    }

    RemovePlayerStatusLabel(playerid);
    return 1;
}