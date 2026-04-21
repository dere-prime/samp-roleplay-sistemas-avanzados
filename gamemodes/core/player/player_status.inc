/* 
    Nombre y informacion
    Estados de trabajo, y mas.
        Prime Roleplay

*/

stock RemovePlayerNameTag(playerid) {
	if (IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_STATUS_LABEL])) {
		DestroyDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_STATUS_LABEL]);
		PLAYER_TEMP[playerid][pt_STATUS_LABEL] = Text3D:INVALID_STREAMER_ID;
	}
	return 1;
}

GetPlayerHealthIndicator(const playerid, str[]) {
    new name_armor_info[64], armor_start, name_health_info[64], health_start;
    if (PI[playerid][pi_ARMOUR] > 0.0) {
        armor_start = floatround( floatdiv(PI[playerid][pi_ARMOUR], floatdiv(100.0, 10.0)) );
        format(name_armor_info, 64, "{171717}[{E0E0E0}IIIIIIIIII{171717}]");
        if(armor_start < 10) strins(name_armor_info, "{4F4F4F}", 17 + armor_start);
        strcatf(str, "\n%s", name_armor_info);
    }
    health_start = floatround( floatdiv(PI[playerid][pi_HEALTH], floatdiv(100.0, 10.0)) );
    format(name_health_info, 64, "{171717}[{ff0000}IIIIIIIIII{171717}]");
    if(health_start < 10) strins(name_health_info, "{4F0000}", 17 + health_start);
    strcatf(str, "\n%s", name_health_info);
    return 1;
}

forward UpdatePlayerRoleplayNameInfo(const playerid);
public UpdatePlayerRoleplayNameInfo(const playerid) {
    if (!PLAYER_TEMP[playerid][pt_USER_LOGGED] && PLAYER_TEMP[playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return 0;

    if (PI[playerid][pi_WANTED_LEVEL] > 0) {
        SetWantedMarkerToPolice(playerid);
    }
    else if(PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) SendAlertToMedics(playerid);
    if (IsPlayerWorking(playerid, WORK_TAXI)) SetTaxiPlayerMarkers(playerid);

    new name_tag_info[240];
    if (PLAYER_TEMP[playerid][pt_ADMIN_SERVICE]) { strcatf(name_tag_info, "{"#ADMIN_COLOR"}%s\n", ADMIN_LEVELS[ PI[playerid][pi_ADMIN_LEVEL] ]); }
    else {
        if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) { strcatf(name_tag_info, "{"#RED_COLOR"}Abatido\n"); }
        else {
            if (PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] > 5.0) { strcatf(name_tag_info, "{"#SECONDARY_COLOR"}Plantando...\n"); }
            if (PLAYER_TEMP[playerid][pt_WORKING_IN] != WORK_NONE) {
                switch (PLAYER_TEMP[playerid][pt_WORKING_IN]) {
                    default: {
                        strcatf(name_tag_info, "{%s}%c%s\n", work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_COLOR], toupper(work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME][0]), work_info[ PLAYER_TEMP[playerid][pt_WORKING_IN] ][work_info_NAME][1]);
                    }
                }
            }
        }
    }

    strcatf(name_tag_info, "{CCCCCC}%s [%d]", PLAYER_TEMP[playerid][pt_NAME], playerid);

    if (PI[playerid][pi_STATE] != ROLEPLAY_STATE_CRACK && PLAYER_TEMP[playerid][pt_HEALTH_INDICATOR_TIMER] > gettime()) {
        GetPlayerHealthIndicator(playerid, name_tag_info);
    }

    if (IsValidDynamic3DTextLabel(PLAYER_TEMP[playerid][pt_STATUS_LABEL])) {
        UpdateDynamic3DTextLabelText(PLAYER_TEMP[playerid][pt_STATUS_LABEL], -1, name_tag_info);
    }
    else {
        PLAYER_TEMP[playerid][pt_STATUS_LABEL] = CreateDynamic3DTextLabel(name_tag_info, -1, 0.0, 0.0, 0.4, 20.0, playerid, .testlos = true);
    }
    return 1;
}