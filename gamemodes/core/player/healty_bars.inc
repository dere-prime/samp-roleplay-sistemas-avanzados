/*

    Healthy Bars
        Prime Roleplay

*/

#include <YSI_Coding\y_hooks>

#define HEALTHY_BARS_TD 8

static PlayerText: healthy_bars_ptd[MAX_PLAYERS][HEALTHY_BARS_TD] = {PlayerText:INVALID_TEXT_DRAW, ...}; //healthy bars editor
static bool: player_editing_healthy_bar[MAX_PLAYERS];

hook OnPlayerConnect(playerid) {
    healthy_bars_ptd[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 235.000, "Posicion");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][0], 0.300, 1.500);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][0], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][0], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][0], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][0], 3);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][0], 1);

    healthy_bars_ptd[playerid][1] = CreatePlayerTextDraw(playerid, 320.000, 245.000, "_");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][1], 0.000, 10.000);
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][1], 0.000, 130.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][1], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][1], -1);
    PlayerTextDrawUseBox(playerid, healthy_bars_ptd[playerid][1], 1);
    PlayerTextDrawBoxColor(playerid, healthy_bars_ptd[playerid][1], 150);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][1], 1);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][1], 1);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][1], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][1], 1);

    healthy_bars_ptd[playerid][2] = CreatePlayerTextDraw(playerid, 260.000, 250.000, "LD_BEAT:left");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][2], 25.000, 30.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][2], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][2], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][2], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][2], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][2], 1);

    healthy_bars_ptd[playerid][3] = CreatePlayerTextDraw(playerid, 291.000, 250.000, "LD_BEAT:right");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][3], 25.000, 30.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][3], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][3], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][3], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][3], 1);

    healthy_bars_ptd[playerid][4] = CreatePlayerTextDraw(playerid, 260.000, 290.000, "LD_BEAT:up");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][4], 25.000, 30.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][4], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][4], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][4], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][4], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][4], 1);

    healthy_bars_ptd[playerid][5] = CreatePlayerTextDraw(playerid, 290.000, 290.000, "LD_BEAT:down");
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][5], 25.000, 30.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][5], 1);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][5], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][5], 255);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][5], 4);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][5], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][5], 1);

    healthy_bars_ptd[playerid][6] = CreatePlayerTextDraw(playerid, 350.000, 300.000, "Terminar");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][6], 0.400, 1.799);
    PlayerTextDrawTextSize(playerid, healthy_bars_ptd[playerid][6], 15.000, 75.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][6], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][6], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][6], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][6], 1);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][6], 1);
    PlayerTextDrawSetSelectable(playerid, healthy_bars_ptd[playerid][6], 1);

    healthy_bars_ptd[playerid][7] = CreatePlayerTextDraw(playerid, 350.000, 275.000, "Y: 0.000, X: 0.000");
    PlayerTextDrawLetterSize(playerid, healthy_bars_ptd[playerid][7], 0.158, 1.000);
    PlayerTextDrawAlignment(playerid, healthy_bars_ptd[playerid][7], 2);
    PlayerTextDrawColor(playerid, healthy_bars_ptd[playerid][7], -1);
    PlayerTextDrawSetShadow(playerid, healthy_bars_ptd[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, healthy_bars_ptd[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, healthy_bars_ptd[playerid][7], 150);
    PlayerTextDrawFont(playerid, healthy_bars_ptd[playerid][7], 1);
    PlayerTextDrawSetProportional(playerid, healthy_bars_ptd[playerid][7], 1);

    return Y_HOOKS_CONTINUE_RETURN_1;
}
