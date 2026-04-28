/*

	Login
		Module

*/

/* LOAD PLAYER DATA */
forward LoadPlayerDataAndLogin(const playerid);
public LoadPlayerDataAndLogin(const playerid) {
	KillTimer(PLAYER_TEMP[playerid][pt_AUTO_LOGIN_TIMER]);
	PLAYER_TEMP[playerid][pt_AUTO_LOGIN_TIMER] = -1;
    
    inline OnPlayerDataLoad() {
        new rows;
        if (cache_get_row_count(rows)) {
            if (rows) {
                new bool:isnull_crew;
                cache_get_value_name(0, "reg_date", PI[playerid][pi_REG_DATE], 24);
                cache_get_value_name(0, "last_connection", PI[playerid][pi_LAST_CONNECTION], 24);
                cache_get_value_name_int(0, "last_connection_timestamp", PI[playerid][pi_LAST_CONNECTION_TIMESTAMP]);
                cache_get_value_name_int(0, "time_playing", PI[playerid][pi_TIME_PLAYING]);
                cache_get_value_name_int(0, "level", PI[playerid][pi_LEVEL]);
                cache_get_value_name_int(0, "rep", PI[playerid][pi_REP]);
                cache_get_value_name_int(0, "time_for_rep", PI[playerid][pi_TIME_FOR_REP]);
                cache_get_value_name_int(0, "time_for_payday", PI[playerid][pi_TIME_FOR_PAYDAY]);
                cache_get_value_name_int(0, "doubt_channel", PI[playerid][pi_DOUBT_CHANNEL]);
                cache_get_value_name_int(0, "admin_level", PI[playerid][pi_ADMIN_LEVEL]);
                cache_get_value_name_int(0, "vip", PI[playerid][pi_VIP]);
                cache_get_value_name(0, "vip_expire_date", PI[playerid][pi_VIP_EXPIRE_DATE], 24);
                cache_get_value_name_int(0, "coins", PI[playerid][pi_COINS]);
                cache_get_value_name_int(0, "skin", PI[playerid][pi_SKIN]);
                cache_get_value_name_int(0, "cash", PI[playerid][pi_CASH]);
                cache_get_value_name_float(0, "pos_x", PI[playerid][pi_POS_X]);
                cache_get_value_name_float(0, "pos_y", PI[playerid][pi_POS_Y]);
                cache_get_value_name_float(0, "pos_z", PI[playerid][pi_POS_Z]);
                cache_get_value_name_float(0, "angle", PI[playerid][pi_ANGLE]);
                cache_get_value_name_int(0, "state", PI[playerid][pi_STATE]);
                cache_get_value_name_int(0, "interior", PI[playerid][pi_INTERIOR]);
                cache_get_value_name_int(0, "local_interior", PI[playerid][pi_LOCAL_INTERIOR]);
                cache_get_value_name_int(0, "fight_style", PI[playerid][pi_FIGHT_STYLE]);
                cache_get_value_name_float(0, "health", PI[playerid][pi_HEALTH]);
                cache_get_value_name_float(0, "armour", PI[playerid][pi_ARMOUR]);
                cache_get_value_name_int(0, "gender", PI[playerid][pi_GENDER]);
                cache_get_value_name_float(0, "hungry", PI[playerid][pi_HUNGRY]);
                cache_get_value_name_float(0, "thirst", PI[playerid][pi_THIRST]);
                cache_get_value_name_int(0, "wanted_level", PI[playerid][pi_WANTED_LEVEL]);
                cache_get_value_name_int(0, "police_jail_time", PI[playerid][pi_POLICE_JAIL_TIME]);
                cache_get_value_name_int(0, "police_duty", PI[playerid][pi_POLICE_DUTY]);
                cache_get_value_name_int(0, "police_jail_id", PI[playerid][pi_POLICE_JAIL_ID]);
                cache_get_value_name_int(0, "bank_account", PI[playerid][pi_BANK_ACCOUNT]);
                cache_get_value_name_int(0, "bank_money", PI[playerid][pi_BANK_MONEY]);
                cache_get_value_name_int(0, "phone_state", PI[playerid][pi_PHONE_STATE]);
                cache_get_value_name_int(0, "phone_visible_number", PI[playerid][pi_PHONE_VISIBLE_NUMBER]);
                cache_get_value_name_int(0, "config_sounds", PI[playerid][pi_CONFIG_SOUNDS]);
                cache_get_value_name_int(0, "config_audio", PI[playerid][pi_CONFIG_AUDIO]);
                cache_get_value_name_int(0, "config_time", PI[playerid][pi_CONFIG_TIME]);
                cache_get_value_name_int(0, "config_hud", PI[playerid][pi_CONFIG_HUD]);
                cache_get_value_name_int(0, "config_admin", PI[playerid][pi_CONFIG_ADMIN]);
                cache_get_value_name_int(0, "doubt_mute", PI[playerid][pi_DOUBT_MUTE]);
                cache_get_value_name_int(0, "placa_pd", PI[playerid][pi_PLACA_PD]);
                cache_is_value_name_null(0, "crew", isnull_crew);
                if (!isnull_crew) cache_get_value_name_int(0, "crew", PI[playerid][pi_CREW]);
                cache_get_value_name_int(0, "crew_rank", PI[playerid][pi_CREW_RANK]);
                cache_get_value_name(0, "current_health_bar_config", PI[playerid][pi_CURRENT_HEALTH_BAR_CONFIG], 64);
                cache_get_value_name(0, "current_armour_bar_config", PI[playerid][pi_CURRENT_ARMOUR_BAR_CONFIG], 64);
                cache_get_value_name_int(0, "health_bar_colour", PI[playerid][pi_HEALTH_BAR_COLOUR]);
                cache_get_value_name_int(0, "armour_bar_colour", PI[playerid][pi_ARMOUR_BAR_COLOUR]);
                CallLocalFunction("OnPlayerLogin", "i", playerid);
            }
            else Kick(playerid);
        }
        else Kick(playerid);
    }
    GameTextForPlayer(playerid, "cargando...", 10000, 3);
    mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT * FROM player WHERE id = %d;", PI[playerid][pi_ID]);
    MySQL_TQueryInline(handle_db, using inline OnPlayerDataLoad, QUERY_BUFFER);
    return 1;
}

public OnPlayerLogin(playerid) {
	PLAYER_TEMP[playerid][pt_USER_LOGGED] = true;

	LoadPlayerVehicles(playerid);
	LoadPlayerPhoneBook(playerid);
	LoadPlayerToys(playerid);
	LoadPlayerGPSData(playerid);
	LoadPlayerWorks(playerid);
	LoadPlayerCrewInfo(playerid);
	LoadPlayerMissions(playerid);
	LoadPlayerInventory(playerid);
    SetPlayerHealthyBarsConfig(playerid);

	SetPlayerScore(playerid, PI[playerid][pi_LEVEL]);
	PLAYER_TEMP[playerid][pt_DOUBT_CHANNEL_TIME] = gettime();
	ResetPlayerWeapons(playerid);
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, PI[playerid][pi_CASH]);
	SetPlayerFightingStyle(playerid, PI[playerid][pi_FIGHT_STYLE]);
	SetPlayerHealthEx(playerid, PI[playerid][pi_HEALTH]);
	SetPlayerArmourEx(playerid, PI[playerid][pi_ARMOUR]);
	SetPlayerVirtualWorld(playerid, 0);
	StopAudioStreamForPlayer(playerid);
	SetSpawnInfo(playerid, NO_TEAM, PI[playerid][pi_SKIN], PI[playerid][pi_POS_X], PI[playerid][pi_POS_Y], PI[playerid][pi_POS_Z], PI[playerid][pi_ANGLE], 0, 0, 0, 0, 0, 0);
	SetPlayerInterior(playerid, PI[playerid][pi_INTERIOR]);
	
	PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] = 0;
    SendPlayerClientMessage(playerid, "--------------------------------------------------------------------------------------------------------------------------------");
	SendPlayerClientMessage(playerid, "{"#PRIMARY_COLOR"}»{ffffff} Bienvenido {"#PRIMARY_COLOR"}%s{ffffff}. te conectaste por ultima vez el {"#SECONDARY_COLOR"}%s{ffffff}.", PLAYER_TEMP[playerid][pt_NAME], PI[playerid][pi_LAST_CONNECTION]);
    SendPlayerClientMessage(playerid, "--------------------------------------------------------------------------------------------------------------------------------");

    TogglePlayerSpectatingEx(playerid, false);
	TogglePlayerControllableEx(playerid, false);
}

/* LOGIN DIALOG */
DialogCreate:PlayerLoginInputPass(playerid) {
    Dialog_Open(
        playerid, Dialog:PlayerLoginInputPass, DIALOG_STYLE_INPUT, 
        ""SERVER_NAME_FORMAT" · Iniciar sesion", 
        "\
            {ffffff}Bienvenido de nuevo, Escriba su clave para {"#SECONDARY_COLOR"}ingresar a su cuenta{ffffff}.\n\
            \n\
            Si {"#ERROR_COLOR"}no tiene una cuenta registrada{ffffff} con este nombre:\n\
            {"#SECONDARY_COLOR"}•{ffffff} Cierre el juego y vuelva a ingresar con otro nombre que no este en uso.\n\
        ",
        "»", "-"
    );
}
DialogResponse:PlayerLoginInputPass(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (!strlen(inputtext)) { ShowPlayerMessage(playerid, "La contraseña no puede estar vacía, intente de nuevo."); Dialog_Show(playerid, Dialog:PlayerLoginInputPass); return 1;}
       
        new password[64 + 1];
        SHA256_PassHash(inputtext, PI[playerid][pi_SALT], password, sizeof password);

        if (!strcmp(password, PI[playerid][pi_PASS], false)) {
            LoadPlayerDataAndLogin(playerid);
        } else {
            PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] ++;
            if (PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP] > MAX_BAD_LOGIN_ATTEMPS) return Kick(playerid);
            Dialog_Show(playerid, Dialog:PlayerLoginInputPass);
            ShowPlayerMessage(playerid, "Contraseña incorrecta, intente de nuevo. %d/%d intentos fallidos.", PLAYER_TEMP[playerid][pt_BAD_LOGIN_ATTEMP], MAX_BAD_LOGIN_ATTEMPS);
        }
    } else {
        Dialog_Message(playerid, DIALOG_STYLE_MSGBOX, ""SERVER_NAME_FORMAT" · Boton de salir", "{FFFFFF}Ha cancelado el inicio de sesion de su cuenta.\n\nSi desea ingresar en otro momento\npuede hacerlo reiniciando el proceso de conexion al servidor.", "Salir");
        KickEx(playerid, 500);
    }
    return 1;
}