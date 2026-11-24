/*

    Healthy Bars
        Prime Roleplay

*/

#include <YSI_Coding\y_hooks>

#define HEALTHY_BARS_TD 16

#define HB_BUTTON_POS_UP 7
#define HB_BUTTON_POS_DOWN 5
#define HB_BUTTON_POS_LEFT 4
#define HB_BUTTON_POS_RIGHT 6

#define HB_BUTTON_SIZE_UP 11
#define HB_BUTTON_SIZE_DOWN 9
#define HB_BUTTON_SIZE_LEFT 8
#define HB_BUTTON_SIZE_RIGHT 10

#define HB_VECTOR_POS_X 0
#define HB_VECTOR_POS_Y 1

#define HB_VECTOR_SIZE_X 2
#define HB_VECTOR_SIZE_Y 3

#define MOBILE_ARMOUR_POS "517.0,76.0,49.0,10.0"
#define MOBILE_HEALTH_POS "517.0,61.0,49.0,10.0"

#define PC_ARMOUR_POS "546.0,46.0,63.0,10.0"
#define PC_HEALTH_POS "546.0,65.0,63.0,10.0"

#define DEFAULT_HEALTH_BAR_COLOR 2
#define DEFAULT_ARMOUR_BAR_COLOR 1

static PlayerText: healthy_bars_ptd[MAX_PLAYERS][HEALTHY_BARS_TD] = {PlayerText:INVALID_TEXT_DRAW, ...}; //healthy bars editor
static bool: player_editing_healthy_bar[MAX_PLAYERS];
static editor_healthy_bar_select[MAX_PLAYERS]; // 0 - chaleco | 1 - vida

static Float: player_health_bar_config[MAX_PLAYERS][4];
static Float: player_armour_bar_config[MAX_PLAYERS][4];

new PlayerBar:PlayerBar_Health[MAX_PLAYERS];
new PlayerBar:PlayerBar_Armour[MAX_PLAYERS];

UpdatePlayerHealthBar(const playerid) {
    if (IsValidPlayerProgressBar(playerid, PlayerBar_Health[playerid])) {
        SetPlayerProgressBarPos(playerid, PlayerBar_Health[playerid], player_health_bar_config[playerid][0], player_health_bar_config[playerid][1]);
        SetPlayerProgressBarHeight(playerid, PlayerBar_Health[playerid], player_health_bar_config[playerid][3]);
        SetPlayerProgressBarWidth(playerid, PlayerBar_Health[playerid], player_health_bar_config[playerid][2]);
        SetPlayerProgressBarColour(playerid, PlayerBar_Health[playerid], RandomColors[ PI[playerid][pi_HEALTH_BAR_COLOUR] ]);
        return 1;
    }
    return 0;
}

UpdatePlayerArmourBar(const playerid) {
    if (IsValidPlayerProgressBar(playerid, PlayerBar_Armour[playerid])) {
        SetPlayerProgressBarPos(playerid, PlayerBar_Armour[playerid], player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1]);
        SetPlayerProgressBarHeight(playerid, PlayerBar_Armour[playerid], player_armour_bar_config[playerid][3]);
        SetPlayerProgressBarWidth(playerid, PlayerBar_Armour[playerid], player_armour_bar_config[playerid][2]);
        SetPlayerProgressBarColour(playerid, PlayerBar_Armour[playerid],RandomColors[ PI[playerid][pi_ARMOUR_BAR_COLOUR] ]);
        return 1;
    }
    return 0;
}

ShowPlayerHealthyBarsEditor(const playerid) {
    for (new i = 0; i != HEALTHY_BARS_TD; i ++) {
        PlayerTextDrawShow(playerid, healthy_bars_ptd[playerid][i]);
    }
    player_editing_healthy_bar[playerid] = true;
    SelectTextDrawEx(playerid, -1);
    return 1;
}

CancelPlayerHealthyBarsEditor(const playerid) {    
    for (new i = 0; i != HEALTHY_BARS_TD; i ++) {
        PlayerTextDrawHide(playerid, healthy_bars_ptd[playerid][i]);
    }
    player_editing_healthy_bar[playerid] = false;
    CancelSelectTextDrawEx(playerid);
    return 1;
}

stock IsPlayerEditingHealthyBars(const playerid) {
    return player_editing_healthy_bar[playerid];
}

CreatePlayerHealthyBars(const playerid) {
    if(PLAYER_TEMP[playerid][pt_PLAYER_PLATFORM]) {
        sscanf(MOBILE_HEALTH_POS, "p<,>ffff", player_health_bar_config[playerid][0], player_health_bar_config[playerid][1], player_health_bar_config[playerid][2], player_health_bar_config[playerid][3]);
        sscanf(MOBILE_ARMOUR_POS, "p<,>ffff", player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1], player_armour_bar_config[playerid][2], player_armour_bar_config[playerid][3]);

        format(PI[playerid][pi_CURRENT_HEALTH_BAR_CONFIG], 64, MOBILE_HEALTH_POS);
        format(PI[playerid][pi_CURRENT_ARMOUR_BAR_CONFIG], 64, MOBILE_ARMOUR_POS);
    } else {
        sscanf(PC_HEALTH_POS, "p<,>ffff", player_health_bar_config[playerid][0], player_health_bar_config[playerid][1], player_health_bar_config[playerid][2], player_health_bar_config[playerid][3]);
        sscanf(PC_ARMOUR_POS, "p<,>ffff", player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1], player_armour_bar_config[playerid][2], player_armour_bar_config[playerid][3]);

        format(PI[playerid][pi_CURRENT_HEALTH_BAR_CONFIG], 64, PC_HEALTH_POS);
        format(PI[playerid][pi_CURRENT_ARMOUR_BAR_CONFIG], 64, PC_ARMOUR_POS);
    }

    PI[playerid][pi_HEALTH_BAR_COLOUR] = DEFAULT_HEALTH_BAR_COLOR;
    PI[playerid][pi_ARMOUR_BAR_COLOUR] = DEFAULT_ARMOUR_BAR_COLOR;

    PlayerBar_Health[playerid] = CreatePlayerProgressBar(playerid, player_health_bar_config[playerid][0], player_health_bar_config[playerid][1], player_health_bar_config[playerid][2], player_health_bar_config[playerid][3], RandomColors[ PI[playerid][pi_HEALTH_BAR_COLOUR] ]);
    PlayerBar_Armour[playerid] = CreatePlayerProgressBar(playerid, player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1], player_armour_bar_config[playerid][2], player_armour_bar_config[playerid][3], RandomColors[ PI[playerid][pi_ARMOUR_BAR_COLOUR] ]);
    return 1;
}

UpdateShowPlayerHealthyBars(const playerid, bool: update_config = true) {
    if (update_config) {
        UpdatePlayerHealthBar(playerid);
        UpdatePlayerArmourBar(playerid);
    }

    SetPlayerProgressBarValue(playerid, PlayerBar_Armour[playerid], PI[playerid][pi_ARMOUR]);
    SetPlayerProgressBarValue(playerid, PlayerBar_Health[playerid], PI[playerid][pi_HEALTH]);

    if (PI[playerid][pi_ARMOUR] > 0 || player_editing_healthy_bar[playerid]) {
        ShowPlayerProgressBar(playerid, PlayerBar_Armour[playerid]);
    }
    ShowPlayerProgressBar(playerid, PlayerBar_Health[playerid]);
    return 1;
}

HidePlayerHealthyBars(const playerid) {
    HidePlayerProgressBar(playerid, PlayerBar_Health[playerid]);
    HidePlayerProgressBar(playerid, PlayerBar_Armour[playerid]);
    return 1;
}

SetPlayerHealthyBarsConfig(const playerid) {
    sscanf(PI[playerid][pi_CURRENT_HEALTH_BAR_CONFIG], "p<,>ffff", player_health_bar_config[playerid][0], player_health_bar_config[playerid][1], player_health_bar_config[playerid][2], player_health_bar_config[playerid][3]);
    sscanf(PI[playerid][pi_CURRENT_ARMOUR_BAR_CONFIG], "p<,>ffff", player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1], player_armour_bar_config[playerid][2], player_armour_bar_config[playerid][3]);
    UpdatePlayerHealthBar(playerid);
    UpdatePlayerArmourBar(playerid);
    return 1;
}

DialogCreate:HealthyBarsEditor(playerid) {
    Dialog_Open(
        playerid, Dialog:HealthyBarsEditor, DIALOG_STYLE_TABLIST, "{"#PRIMARY_COLOR"}·{f0f0f0}Hud Barra de vida", 
        "Restablecer valores predeterminados\nEditar barra de vida\nEditar barra de chaleco\nCambiar color barra de vida\nCambiar color barra de chaleco", 
        "Continuar", "Cerrar"
    );
    return 1;
}

DialogResponse:HealthyBarsEditor(playerid, response, listitem, inputtext[]) {
    if (response) {
        switch(listitem) {
            case 0: {
                if(PLAYER_TEMP[playerid][pt_PLAYER_PLATFORM]) {
                    sscanf(MOBILE_HEALTH_POS, "p<,>ffff", player_health_bar_config[playerid][0], player_health_bar_config[playerid][1], player_health_bar_config[playerid][2], player_health_bar_config[playerid][3]);
                    sscanf(MOBILE_ARMOUR_POS, "p<,>ffff", player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1], player_armour_bar_config[playerid][2], player_armour_bar_config[playerid][3]);

                    format(PI[playerid][pi_CURRENT_HEALTH_BAR_CONFIG], 64, MOBILE_HEALTH_POS);
                    format(PI[playerid][pi_CURRENT_ARMOUR_BAR_CONFIG], 64, MOBILE_ARMOUR_POS);
                } else {
                    sscanf(PC_HEALTH_POS, "p<,>ffff", player_health_bar_config[playerid][0], player_health_bar_config[playerid][1], player_health_bar_config[playerid][2], player_health_bar_config[playerid][3]);
                    sscanf(PC_ARMOUR_POS, "p<,>ffff", player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1], player_armour_bar_config[playerid][2], player_armour_bar_config[playerid][3]);

                    format(PI[playerid][pi_CURRENT_HEALTH_BAR_CONFIG], 64, PC_HEALTH_POS);
                    format(PI[playerid][pi_CURRENT_ARMOUR_BAR_CONFIG], 64, PC_ARMOUR_POS);
                }

                PI[playerid][pi_HEALTH_BAR_COLOUR] = DEFAULT_HEALTH_BAR_COLOR;
                PI[playerid][pi_ARMOUR_BAR_COLOUR] = DEFAULT_ARMOUR_BAR_COLOR;

                UpdateShowPlayerHealthyBars(playerid);
                ShowPlayerMessage(playerid, "Configuracion restaurada correctamente.");
            }
            case 1: { editor_healthy_bar_select[playerid] = 1; ShowPlayerHealthyBarsEditor(playerid); }
            case 2: { editor_healthy_bar_select[playerid] = 0; ShowPlayerHealthyBarsEditor(playerid); }
            case 3: { Dialog_Show(playerid, Dialog:HealthBarColorSelect); }
            case 4: { Dialog_Show(playerid, Dialog:ArmourBarColorSelect); }
        }
    }
    return 1;
}

DialogCreate:ArmourBarColorSelect(playerid) { Dialog_Open(playerid, Dialog:ArmourBarColorSelect, DIALOG_STYLE_TABLIST, "{"#PRIMARY_COLOR"}·{f0f0f0}Color barra chaleco", DIALOG_CREATE_CREW_COLOR_String, "Seleccionar", "Cancelar"); }
DialogCreate:HealthBarColorSelect(playerid) { Dialog_Open(playerid, Dialog:HealthBarColorSelect, DIALOG_STYLE_TABLIST, "{"#PRIMARY_COLOR"}·{f0f0f0}Color barra vida", DIALOG_CREATE_CREW_COLOR_String, "Seleccionar", "Cancelar"); }

DialogResponse:ArmourBarColorSelect(playerid, response, listitem, inputtext[]) { 
    if (response) { PI[playerid][pi_ARMOUR_BAR_COLOUR] = listitem; UpdateShowPlayerHealthyBars(playerid); } 
    return 1; 
}
DialogResponse:HealthBarColorSelect(playerid, response, listitem, inputtext[]) { 
    if (response) { PI[playerid][pi_HEALTH_BAR_COLOUR] = listitem; UpdateShowPlayerHealthyBars(playerid); } 
    return 1; 
}

hook OnPlayerClickPlayerTD(playerid, PlayerText:playertextid) {
    if (player_editing_healthy_bar[playerid]) {

        if (editor_healthy_bar_select[playerid]) {
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_POS_UP]) player_health_bar_config[playerid][HB_VECTOR_POS_Y] -= 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_POS_DOWN]) player_health_bar_config[playerid][HB_VECTOR_POS_Y] += 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_POS_LEFT]) player_health_bar_config[playerid][HB_VECTOR_POS_X] -= 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_POS_RIGHT]) player_health_bar_config[playerid][HB_VECTOR_POS_X] += 1.0;

            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_SIZE_UP]) player_health_bar_config[playerid][HB_VECTOR_SIZE_Y] -= 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_SIZE_DOWN]) player_health_bar_config[playerid][HB_VECTOR_SIZE_Y] += 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_SIZE_LEFT]) player_health_bar_config[playerid][HB_VECTOR_SIZE_X] -= 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_SIZE_RIGHT]) player_health_bar_config[playerid][HB_VECTOR_SIZE_X] += 1.0;

            va_PlayerTextDrawSetString(playerid, healthy_bars_ptd[playerid][3], "%.3f, %.3f, %.3f, %.3f", player_health_bar_config[playerid][0], player_health_bar_config[playerid][1], player_health_bar_config[playerid][2], player_health_bar_config[playerid][3]);
        } else {
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_POS_UP]) player_armour_bar_config[playerid][HB_VECTOR_POS_Y] -= 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_POS_DOWN]) player_armour_bar_config[playerid][HB_VECTOR_POS_Y] += 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_POS_LEFT]) player_armour_bar_config[playerid][HB_VECTOR_POS_X] -= 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_POS_RIGHT]) player_armour_bar_config[playerid][HB_VECTOR_POS_X] += 1.0;

            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_SIZE_UP]) player_armour_bar_config[playerid][HB_VECTOR_SIZE_Y] -= 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_SIZE_DOWN]) player_armour_bar_config[playerid][HB_VECTOR_SIZE_Y] += 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_SIZE_LEFT]) player_armour_bar_config[playerid][HB_VECTOR_SIZE_X] -= 1.0;
            if (playertextid == healthy_bars_ptd[playerid][HB_BUTTON_SIZE_RIGHT]) player_armour_bar_config[playerid][HB_VECTOR_SIZE_X] += 1.0;

            va_PlayerTextDrawSetString(playerid, healthy_bars_ptd[playerid][3], "%.3f, %.3f, %.3f, %.3f", player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1], player_armour_bar_config[playerid][2], player_armour_bar_config[playerid][3]);
        }

        format(PI[playerid][pi_CURRENT_HEALTH_BAR_CONFIG], 64, "%f,%f,%f,%f", player_health_bar_config[playerid][0], player_health_bar_config[playerid][1], player_health_bar_config[playerid][2], player_health_bar_config[playerid][3]);
        format(PI[playerid][pi_CURRENT_ARMOUR_BAR_CONFIG], 64, "%f,%f,%f,%f", player_armour_bar_config[playerid][0], player_armour_bar_config[playerid][1], player_armour_bar_config[playerid][2], player_armour_bar_config[playerid][3]);

        if (editor_healthy_bar_select[playerid]) UpdatePlayerHealthBar(playerid);
        else UpdatePlayerArmourBar(playerid);

        if (playertextid == healthy_bars_ptd[playerid][15]) {
            CancelPlayerHealthyBarsEditor(playerid);
            UpdateShowPlayerHealthyBars(playerid);
            ShowPlayerMessage(playerid, "Configuracion guardada correctamente.");
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid) {
    player_editing_healthy_bar[playerid] = false;
    CreatePlayerHealthyBars(playerid);

    healthy_bars_ptd[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 375.000, "_");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][0], 0.000, 3.000);
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][0], 0.000, 115.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][0], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][0], -1);
    PlayerTextDrawUseBox(playerid, healthy_bars_ptd[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, healthy_bars_ptd[playerid][0], 252645375);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][0], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][0], 1);

    healthy_bars_ptd[playerid][1] = CreatePlayerTextDraw(playerid, 201.000, 375.000, "_");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][1], 0.000, 3.000);
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][1], 0.000, 115.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][1], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][1], -1);
    PlayerTextDrawUseBox(playerid, healthy_bars_ptd[playerid][1], 1);
    PlayerTextDrawBoxColor(playerid, healthy_bars_ptd[playerid][1], 252645375);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][1], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][1], 1);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][1], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][1], 1);

    healthy_bars_ptd[playerid][2] = CreatePlayerTextDraw(playerid, 439.000, 375.000, "_");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][2], 0.000, 3.000);
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][2], 0.000, 115.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][2], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][2], -1);
    PlayerTextDrawUseBox(playerid, healthy_bars_ptd[playerid][2], 1);
    PlayerTextDrawBoxColor(playerid, healthy_bars_ptd[playerid][2], 252645375);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][2], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][2], 1);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][2], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][2], 1);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][2], 1);

    healthy_bars_ptd[playerid][3] = CreatePlayerTextDraw(playerid, 320.000, 383.000, "0.000, 0.000, 0.000, 0.000");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][3], 0.150, 1.049);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][3], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][3], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][3], 1);

    healthy_bars_ptd[playerid][4] = CreatePlayerTextDraw(playerid, 385.000, 379.000, "LD_BEAT:left");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][4], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][4], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][4], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][4], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][4], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][4], 1);

    healthy_bars_ptd[playerid][5] = CreatePlayerTextDraw(playerid, 467.000, 379.000, "LD_BEAT:down");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][5], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][5], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][5], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][5], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][5], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][5], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][5], 1);

    healthy_bars_ptd[playerid][6] = CreatePlayerTextDraw(playerid, 415.000, 379.000, "LD_BEAT:right");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][6], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][6], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][6], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][6], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][6], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][6], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][6], 1);

    healthy_bars_ptd[playerid][7] = CreatePlayerTextDraw(playerid, 440.000, 379.000, "LD_BEAT:up");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][7], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][7], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][7], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][7], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][7], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][7], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][7], 1);

    healthy_bars_ptd[playerid][8] = CreatePlayerTextDraw(playerid, 150.000, 379.000, "LD_BEAT:left");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][8], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][8], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][8], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][8], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][8], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][8], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][8], 1);

    healthy_bars_ptd[playerid][9] = CreatePlayerTextDraw(playerid, 232.000, 379.000, "LD_BEAT:down");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][9], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][9], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][9], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][9], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][9], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][9], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][9], 1);

    healthy_bars_ptd[playerid][10] = CreatePlayerTextDraw(playerid, 180.000, 379.000, "LD_BEAT:right");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][10], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][10], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][10], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][10], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][10], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][10], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][10], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][10], 1);

    healthy_bars_ptd[playerid][11] = CreatePlayerTextDraw(playerid, 205.000, 379.000, "LD_BEAT:up");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][11], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][11], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][11], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][11], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][11], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][11], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][11], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][11], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][11], 1);

    healthy_bars_ptd[playerid][12] = CreatePlayerTextDraw(playerid, 438.000, 364.000, "Posicion");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][12], 0.300, 1.500);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][12], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][12], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][12], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][12], 1);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][12], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][12], 3);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][12], 1);

    healthy_bars_ptd[playerid][13] = CreatePlayerTextDraw(playerid, 200.000, 364.000, "Proporcion");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][13], 0.300, 1.500);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][13], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][13], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][13], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][13], 1);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][13], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][13], 3);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][13], 1);

    healthy_bars_ptd[playerid][14] = CreatePlayerTextDraw(playerid, 320.000, 364.000, "Info");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][14], 0.300, 1.500);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][14], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][14], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][14], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][14], 1);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][14], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][14], 3);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][14], 1);

    healthy_bars_ptd[playerid][15] = CreatePlayerTextDraw(playerid, 320.000, 315.000, "SALIR");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][15], 0.300, 1.500);
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][15], 15.000, 65.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][15], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][15], -1);
    PlayerTextDrawUseBox(playerid, healthy_bars_ptd[playerid][15], 1);
    PlayerTextDrawBoxColor(playerid, healthy_bars_ptd[playerid][15], -1976835329);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][15], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][15], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][15], 1258324735);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][15], 1);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][15], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][15], 1);

    return Y_HOOKS_CONTINUE_RETURN_1;
}

