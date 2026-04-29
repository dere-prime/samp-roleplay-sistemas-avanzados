/* 

	Player time and reputation management
		Prime Roleplay

*/
#include <YSI_Coding\y_hooks>

#define TIME_FOR_REP 3600 // 1 hora
#define TIME_FOR_PAYDAY 10800 // 3 horas

#define REP_MULTIPLIER 1850

#define PlayerTask_Interval_Seconds 10

/* VAR */
static p_time_update_timer[MAX_PLAYERS];
static p_hide_rep_bar_timer[MAX_PLAYERS];
static p_hide_levelup_timer[MAX_PLAYERS];
static PlayerText: p_rep_bar[MAX_PLAYERS][3];
static PlayerText: p_rep_levelup[MAX_PLAYERS][5];

/* ANIMATORS */
static p_rep_bar_animator[MAX_PLAYERS][3];
static p_rep_levelup_animator[MAX_PLAYERS][5];

/* PAYDAY */
PlayerPayday(playerid) {
	new date[24], money = 300 * PI[playerid][pi_LEVEL];
	if (money > 4500) money = 4500;
	money += minrand(100, 200);
	getDateTime(date);
	SendPlayerClientMessage(playerid, "------------------------------------");
	SendPlayerClientMessage(playerid, "Pago diario (%s)", date);
	SendPlayerClientMessage(playerid, "");
	SendPlayerClientMessage(playerid, "{FFFFFF}Paga del gobierno: %s{"#GREEN_COLOR"}$", number_format_thousand(money));
	SendPlayerClientMessage(playerid, "");
	if (!PI[playerid][pi_VIP]) {
		for(new i = 0, j = GetVehiclePoolSize(); i <= j; i ++) {
			if (!PLAYER_VEHICLES[i][player_vehicle_VALID]) continue;
			if (PLAYER_VEHICLES[i][player_vehicle_OWNER_ID] != PI[playerid][pi_ID]) continue;
			new veh_money = VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_PRICE] / 100;
			money -= veh_money;
			SendPlayerClientMessage(playerid, "{FFFFFF}Seguro del vehículo %s: -%s{"#WARNING_COLOR"}$", VEHICLE_INFO[GLOBAL_VEHICLES[i][gb_vehicle_MODELID] - 400][vehicle_info_NAME], number_format_thousand(veh_money));
		}
		for(new i = 0; i != MAX_PROPERTIES; i ++) {
			if (!PROPERTY_INFO[i][property_VALID]) continue;
			if (PROPERTY_INFO[i][property_OWNER_ID] != PI[playerid][pi_ID]) continue;
		
			if (PROPERTY_INFO[i][property_VIP_LEVEL]) SendPlayerClientMessage(playerid, "{FFFFFF}Seguro de la propiedad %s: 0{"#GREEN_COLOR"}$", PROPERTY_INFO[i][property_NAME]);
			else {
				new property_money = PROPERTY_INFO[i][property_PRICE] / 200;
				money -= property_money;
				SendPlayerClientMessage(playerid, "{FFFFFF}Seguro de la propiedad %s: -%s{"#WARNING_COLOR"}$", PROPERTY_INFO[i][property_NAME], number_format_thousand(property_money));
			}
		}
	}
	if (PLAYER_WORKS[playerid][WORK_POLICE][pwork_SET]) {
		new work_payment = (POLICE_PAYMENT_MULTIPLIER * PLAYER_WORKS[playerid][WORK_POLICE][pwork_LEVEL]);
		money += work_payment;
		SendPlayerClientMessage(playerid, "");
		SendPlayerClientMessage(playerid, "{FFFFFF}Paga por ser policía: %s{"#GREEN_COLOR"}$\n", number_format_thousand(work_payment));
		SendPlayerClientMessage(playerid, "");
	}
	if (PI[playerid][pi_CREW]) {
		new territories = GetCrewTerritories(PI[playerid][pi_CREW]);
		if (territories > 0) {
			SendPlayerClientMessage(playerid, "");
			money += territories * 70;
			if (territories > 0) SendPlayerClientMessage(playerid, "{FFFFFF}Paga por territorios de la banda (%d): %s{"#GREEN_COLOR"}$\n", territories, territories * 70);
			SendPlayerClientMessage(playerid, "");
		}
	}
	if (money + PI[playerid][pi_CASH] <= 0) {
		money = 0;
		PI[playerid][pi_CASH] = 0;
	}
	if (money > 0) SendPlayerClientMessage(playerid, "{FFFFFF}Has ganado: %s{"#GREEN_COLOR"}$", number_format_thousand(money));
	else SendPlayerClientMessage(playerid, "{FFFFFF}Has perdido: -%s{"#ERROR_COLOR"}$", number_format_thousand(money));
	SendPlayerClientMessage(playerid, "------------------------------------");
	GivePlayerCash(playerid, money, true, money < 0 ? true : false);
	return 1;
}

/* GLOBAL */
ptask PlayerTask[SecondsToMs(PlayerTask_Interval_Seconds)](playerid) {
	PI[playerid][pi_TIME_PLAYING] += PlayerTask_Interval_Seconds;
	PI[playerid][pi_TIME_FOR_REP] += PlayerTask_Interval_Seconds;
	PI[playerid][pi_TIME_FOR_PAYDAY] += PlayerTask_Interval_Seconds;
}

forward OnPlayerTimeUpdate(const playerid);
public OnPlayerTimeUpdate(const playerid) {
	if (p_time_update_timer[playerid] != -1) {
		KillTimer(p_time_update_timer[playerid]);
		p_time_update_timer[playerid] = -1;
	}

	if (PI[playerid][pi_TIME_FOR_PAYDAY] >= TIME_FOR_PAYDAY) {
		PI[playerid][pi_TIME_FOR_PAYDAY] = 0;
		PlayerPayday(playerid);
	}
	if (PI[playerid][pi_TIME_FOR_REP] >= TIME_FOR_REP) {
		PI[playerid][pi_TIME_FOR_REP] = 0;
		PI[playerid][pi_REP] += minrand(100, 500);
		UpdatePlayerRepBar(playerid);
	}

	PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL] = (PI[playerid][pi_LEVEL] * REP_MULTIPLIER);

	if (PI[playerid][pi_REP] >= PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL]) {
		PI[playerid][pi_REP] -= PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL];
		if (PI[playerid][pi_REP] <= 0) PI[playerid][pi_REP] = 0;
		PI[playerid][pi_LEVEL] ++;
		ShowPlayerLevelUp(playerid);
		UpdatePlayerRepBar(playerid);
		OnPlayerTimeUpdate(playerid);
		SetPlayerSkillLevels(playerid);
	}

	p_time_update_timer[playerid] = SetTimerEx("OnPlayerTimeUpdate", 60000, false, "i", playerid);
	return 1;
}

hook OnPlayerSpawn(playerid) {
	if (PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_CONNECTED) {
		PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL] = (PI[playerid][pi_LEVEL] * REP_MULTIPLIER);
		p_time_update_timer[playerid] = SetTimerEx("OnPlayerTimeUpdate", 60000, false, "i", playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid) {
    p_rep_bar[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 360.000, "bg_bar");
    PlayerTextDrawLetterSize(playerid, p_rep_bar[playerid][0], 0.000, 0.500);
    PlayerTextDrawTextSize(playerid, p_rep_bar[playerid][0], 0.000, 180.000);
    PlayerTextDrawAlignment(playerid, p_rep_bar[playerid][0], 2);
    PlayerTextDrawColor(playerid, p_rep_bar[playerid][0], -1);
    PlayerTextDrawUseBox(playerid, p_rep_bar[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, p_rep_bar[playerid][0], 252645375);
    PlayerTextDrawSetShadow(playerid, p_rep_bar[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, p_rep_bar[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, p_rep_bar[playerid][0], 150);
    PlayerTextDrawFont(playerid, p_rep_bar[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, p_rep_bar[playerid][0], 1);

    p_rep_bar[playerid][1] = CreatePlayerTextDraw(playerid, 230.000, 360.000, "bar");
    PlayerTextDrawLetterSize(playerid, p_rep_bar[playerid][1], -0.000, 0.500);
    PlayerTextDrawTextSize(playerid, p_rep_bar[playerid][1], 410.000, 0.000); //min 227
    PlayerTextDrawAlignment(playerid, p_rep_bar[playerid][1], 1);
    PlayerTextDrawColor(playerid, p_rep_bar[playerid][1], -1);
    PlayerTextDrawUseBox(playerid, p_rep_bar[playerid][1], 1);
    PlayerTextDrawBoxColor(playerid, p_rep_bar[playerid][1], -1523963137);
    PlayerTextDrawSetShadow(playerid, p_rep_bar[playerid][1], 1);
    PlayerTextDrawSetOutline(playerid, p_rep_bar[playerid][1], 1);
    PlayerTextDrawBackgroundColor(playerid, p_rep_bar[playerid][1], 150);
    PlayerTextDrawFont(playerid, p_rep_bar[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, p_rep_bar[playerid][1], 1);

    p_rep_bar[playerid][2] = CreatePlayerTextDraw(playerid, 320.000, 367.000, "_");
    PlayerTextDrawLetterSize(playerid, p_rep_bar[playerid][2], 0.200, 1.000);
    PlayerTextDrawAlignment(playerid, p_rep_bar[playerid][2], 2);
    PlayerTextDrawColor(playerid, p_rep_bar[playerid][2], -56);
    PlayerTextDrawSetShadow(playerid, p_rep_bar[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, p_rep_bar[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, p_rep_bar[playerid][2], 150);
    PlayerTextDrawFont(playerid, p_rep_bar[playerid][2], 1);
    PlayerTextDrawSetProportional(playerid, p_rep_bar[playerid][2], 1);

    p_rep_levelup[playerid][0] = CreatePlayerTextDraw(playerid, 557.000, 141.000, "BG");
    PlayerTextDrawLetterSize(playerid, p_rep_levelup[playerid][0], 0.000, 3.099);
    PlayerTextDrawTextSize(playerid, p_rep_levelup[playerid][0], 0.000, 80.000);
    PlayerTextDrawAlignment(playerid, p_rep_levelup[playerid][0], 2);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][0], 255);
    PlayerTextDrawUseBox(playerid, p_rep_levelup[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, p_rep_levelup[playerid][0], 168430335);
    PlayerTextDrawSetShadow(playerid, p_rep_levelup[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, p_rep_levelup[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, p_rep_levelup[playerid][0], 150);
    PlayerTextDrawFont(playerid, p_rep_levelup[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, p_rep_levelup[playerid][0], 1);

    p_rep_levelup[playerid][1] = CreatePlayerTextDraw(playerid, 575.000, 125.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, p_rep_levelup[playerid][1], 60.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_rep_levelup[playerid][1], 1);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][1], 336860415);
    PlayerTextDrawSetShadow(playerid, p_rep_levelup[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, p_rep_levelup[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, p_rep_levelup[playerid][1], 255);
    PlayerTextDrawFont(playerid, p_rep_levelup[playerid][1], 4);
    PlayerTextDrawSetProportional(playerid, p_rep_levelup[playerid][1], 1);

    p_rep_levelup[playerid][2] = CreatePlayerTextDraw(playerid, 605.000, 147.000, "0");
    PlayerTextDrawLetterSize(playerid, p_rep_levelup[playerid][2], 0.300, 1.500);
    PlayerTextDrawAlignment(playerid, p_rep_levelup[playerid][2], 2);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][2], -5963521);
    PlayerTextDrawSetShadow(playerid, p_rep_levelup[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, p_rep_levelup[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, p_rep_levelup[playerid][2], 150);
    PlayerTextDrawFont(playerid, p_rep_levelup[playerid][2], 2);
    PlayerTextDrawSetProportional(playerid, p_rep_levelup[playerid][2], 1);

    p_rep_levelup[playerid][3] = CreatePlayerTextDraw(playerid, 583.000, 147.000, "Has subido de nivel");
    PlayerTextDrawLetterSize(playerid, p_rep_levelup[playerid][3], 0.200, 0.899);
    PlayerTextDrawAlignment(playerid, p_rep_levelup[playerid][3], 3);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, p_rep_levelup[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, p_rep_levelup[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, p_rep_levelup[playerid][3], 150);
    PlayerTextDrawFont(playerid, p_rep_levelup[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, p_rep_levelup[playerid][3], 1);

    p_rep_levelup[playerid][4] = CreatePlayerTextDraw(playerid, 576.000, 157.000, "¡Sigue jugando!");
    PlayerTextDrawLetterSize(playerid, p_rep_levelup[playerid][4], 0.200, 0.899);
    PlayerTextDrawAlignment(playerid, p_rep_levelup[playerid][4], 3);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][4], -206);
    PlayerTextDrawSetShadow(playerid, p_rep_levelup[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, p_rep_levelup[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, p_rep_levelup[playerid][4], 150);
    PlayerTextDrawFont(playerid, p_rep_levelup[playerid][4], 1);
    PlayerTextDrawSetProportional(playerid, p_rep_levelup[playerid][4], 1);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason) {
	KillTimer(p_time_update_timer[playerid]);
	p_time_update_timer[playerid] = -1;

	if (p_hide_rep_bar_timer[playerid] != -1) {
		KillTimer(p_hide_rep_bar_timer[playerid]);
		p_hide_rep_bar_timer[playerid] = -1;
	}

	if (p_hide_levelup_timer[playerid] != -1) {
		KillTimer(p_hide_levelup_timer[playerid]);
		p_hide_levelup_timer[playerid] = -1;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

/* REPUTATION */
UpdatePlayerRepBar(const playerid) {
	if (p_hide_rep_bar_timer[playerid] != -1) KillTimer(p_hide_rep_bar_timer[playerid]);
	p_hide_rep_bar_timer[playerid] = -1;

	for (new i = 0; i != sizeof(p_rep_bar_animator[]); i ++) {
		if (!IsValidAnimator(p_rep_bar_animator[playerid][i])) continue;
		PlayerText_StopMove(p_rep_bar_animator[playerid][i]);
	}

	PlayerTextDrawBoxColor(playerid, p_rep_bar[playerid][0], 252645375);
    PlayerTextDrawBoxColor(playerid, p_rep_bar[playerid][1], -1523963137);
    PlayerTextDrawColor(playerid, p_rep_bar[playerid][2], -56);

	PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL] = (PI[playerid][pi_LEVEL] * REP_MULTIPLIER);
	va_PlayerTextDrawSetString(playerid, p_rep_bar[playerid][2], "Nivel_%d_%d/%d", PI[playerid][pi_LEVEL], PI[playerid][pi_REP], PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL]);

	new Float:sizeX = 227.0 + floatmul(floatdiv(PI[playerid][pi_REP], PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL]), 183.0);
	PlayerTextDrawTextSize(playerid, p_rep_bar[playerid][1], sizeX, 0.0);

	for (new i = 0; i != sizeof(p_rep_bar[]); i ++) {
		PlayerTextDrawShow(playerid, p_rep_bar[playerid][i]);
	}

	p_hide_rep_bar_timer[playerid] = SetTimerEx("_player_interpolate_hide_repbar", 4000, false, "i", playerid);
	return 1;
}

forward _player_interpolate_hide_repbar(const playerid);
public _player_interpolate_hide_repbar(const playerid) {
	KillTimer(p_hide_rep_bar_timer[playerid]);
	p_hide_rep_bar_timer[playerid] = -1;

	p_rep_bar_animator[playerid][0] = PlayerText_InterpolateBoxColor(playerid, p_rep_bar[playerid][0], 0x00000000, 1000, EASE_OUT_CIRC);
	p_rep_bar_animator[playerid][1] = PlayerText_InterpolateBoxColor(playerid, p_rep_bar[playerid][1], 0x00000000, 1000, EASE_OUT_CIRC);
	p_rep_bar_animator[playerid][2] = PlayerText_InterpolateColor(playerid, p_rep_bar[playerid][2], 0x00000000, 1000, EASE_OUT_CIRC);
	return 1;
}

/* LEVEL UP TD */
ShowPlayerLevelUp(const playerid) {
	if (p_hide_levelup_timer[playerid] != -1) KillTimer(p_hide_levelup_timer[playerid]);
	p_hide_levelup_timer[playerid] = -1;

	for (new i = 0; i != sizeof(p_rep_levelup_animator[]); i ++) {
		if (!IsValidAnimator(p_rep_levelup_animator[playerid][i])) continue;
		PlayerText_StopMove(p_rep_levelup_animator[playerid][i]);
	}

    PlayerTextDrawBoxColor(playerid, p_rep_levelup[playerid][0], 252645375);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][1], 336860415);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][2], -5963521);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][3], -1);
    PlayerTextDrawColor(playerid, p_rep_levelup[playerid][4], -206);

	PlayerPlaySoundEx(playerid, 5201, 0.0, 0.0, 0.0);
	va_PlayerTextDrawSetString(playerid, p_rep_levelup[playerid][2], "%d", PI[playerid][pi_LEVEL]);

	for(new i = 0; i != sizeof(p_rep_levelup[]); i ++) {
		PlayerTextDrawShow(playerid, p_rep_levelup[playerid][i]);
	}
	p_hide_levelup_timer[playerid] = SetTimerEx("_player_interpolate_hide_lvlup", 4000, false, "i", playerid);
	
	SetPlayerScore(playerid, PI[playerid][pi_LEVEL]);
	return 1;
}

forward _player_interpolate_hide_lvlup(const playerid);
public _player_interpolate_hide_lvlup(const playerid) {
	KillTimer(p_hide_levelup_timer[playerid]);
	p_hide_levelup_timer[playerid] = -1;

	p_rep_levelup_animator[playerid][0] = PlayerText_InterpolateBoxColor(playerid, p_rep_levelup[playerid][0], 0x00000000, 1000, EASE_OUT_CIRC);
	p_rep_levelup_animator[playerid][1] = PlayerText_InterpolateColor(playerid, p_rep_levelup[playerid][1], 0x00000000, 1000, EASE_OUT_CIRC);
	p_rep_levelup_animator[playerid][2] = PlayerText_InterpolateColor(playerid, p_rep_levelup[playerid][2], 0x00000000, 1000, EASE_OUT_CIRC);
	p_rep_levelup_animator[playerid][3] = PlayerText_InterpolateColor(playerid, p_rep_levelup[playerid][3], 0x00000000, 1000, EASE_OUT_CIRC);
	p_rep_levelup_animator[playerid][4] = PlayerText_InterpolateColor(playerid, p_rep_levelup[playerid][4], 0x00000000, 1000, EASE_OUT_CIRC);
	return 1;
}

/* ADD REPUTATION */
AddPlayerReputation(const playerid, const rep) {
	PI[playerid][pi_REP] += rep;
	UpdatePlayerRepBar(playerid);
	if (PI[playerid][pi_REP] >= PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL]) {
		PI[playerid][pi_REP] -= PLAYER_TEMP[playerid][pt_NEEDED_REP_FOR_NEXT_LEVEL];
		if (PI[playerid][pi_REP] <= 0) PI[playerid][pi_REP] = 0;
		PI[playerid][pi_LEVEL] ++;
		UpdatePlayerRepBar(playerid);
		ShowPlayerLevelUp(playerid);
		OnPlayerTimeUpdate(playerid);
	}
	return 1;
}

/* ANIMATOR CONFIG */
hook Animator_OnFinish(playerid, animator, type) {
	if (animator == p_rep_bar_animator[playerid][0]) { PlayerTextDrawHide(playerid, p_rep_bar[playerid][0]); }
	if (animator == p_rep_bar_animator[playerid][1]) { PlayerTextDrawHide(playerid, p_rep_bar[playerid][1]); }
	if (animator == p_rep_bar_animator[playerid][2]) { PlayerTextDrawHide(playerid, p_rep_bar[playerid][2]); }

	if (animator == p_rep_levelup_animator[playerid][0]) { PlayerTextDrawHide(playerid, p_rep_levelup[playerid][0]); }
	if (animator == p_rep_levelup_animator[playerid][1]) { PlayerTextDrawHide(playerid, p_rep_levelup[playerid][1]); }
	if (animator == p_rep_levelup_animator[playerid][2]) { PlayerTextDrawHide(playerid, p_rep_levelup[playerid][2]); }
	if (animator == p_rep_levelup_animator[playerid][3]) { PlayerTextDrawHide(playerid, p_rep_levelup[playerid][3]); }
	if (animator == p_rep_levelup_animator[playerid][4]) { PlayerTextDrawHide(playerid, p_rep_levelup[playerid][4]); }
	return Y_HOOKS_CONTINUE_RETURN_1;
}
