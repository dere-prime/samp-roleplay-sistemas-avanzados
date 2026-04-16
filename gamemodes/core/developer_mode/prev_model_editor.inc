/*

    Preview Model Editor

*/

#include <YSI_Coding\y_hooks>

#define MAX_IPME_TEXTDRAWS 8
new 
    PlayerText:p_InterfacePreviewModelEditor[MAX_PLAYERS][MAX_IPME_TEXTDRAWS],
    bool:p_IPMEditor[MAX_PLAYERS],
    p_IPME_ModelID[MAX_PLAYERS],
    Float:p_IPME_RotX[MAX_PLAYERS],
    Float:p_IPME_RotY[MAX_PLAYERS],
    Float:p_IPME_RotZ[MAX_PLAYERS],
    Float:p_IPME_Scale[MAX_PLAYERS],
    bool:p_IPME_ArrowMode[MAX_PLAYERS],
    Float:p_IPME_Multiplier[MAX_PLAYERS];

CMD:ipme(playerid) { ShowPlayerIPME(playerid); return 1; }

ShowPlayerIPME(const playerid) {
    p_IPMEditor[playerid] = true;
    va_PlayerTextDrawSetString(playerid, p_InterfacePreviewModelEditor[playerid][6], "%.4f,%.4f,%.4f,%f", p_IPME_RotX[playerid], p_IPME_RotY[playerid], p_IPME_RotZ[playerid], p_IPME_Scale[playerid]);

    PlayerTextDrawSetPreviewModel(playerid, p_InterfacePreviewModelEditor[playerid][1], p_IPME_ModelID[playerid]);
    PlayerTextDrawSetPreviewRot(playerid, p_InterfacePreviewModelEditor[playerid][1], p_IPME_RotX[playerid], p_IPME_RotY[playerid], p_IPME_RotZ[playerid], p_IPME_Scale[playerid]);

    for (new i = 0; i < MAX_IPME_TEXTDRAWS; i++) {
        PlayerTextDrawShow(playerid, p_InterfacePreviewModelEditor[playerid][i]);
    }
    SelectTextDrawEx(playerid, -1);
    return 1;
}

HidePlayerIPME(const playerid, bool:clear = true) {
    p_IPMEditor[playerid] = false;
    for (new i = 0; i < MAX_IPME_TEXTDRAWS; i++) {
        PlayerTextDrawHide(playerid, p_InterfacePreviewModelEditor[playerid][i]);
    }
    CancelSelectTextDrawEx(playerid);
    if (clear) ClearPlayerIPME(playerid);
    return 1;
}

ClearPlayerIPME(const playerid) {
    p_IPME_ModelID[playerid] = 19300; // Default model
    p_IPME_RotX[playerid] = 0.0;
    p_IPME_RotY[playerid] = 0.0;
    p_IPME_RotZ[playerid] = 0.0;
    p_IPME_Scale[playerid] = 1.0;
    p_IPME_ArrowMode[playerid] = false;
    p_IPME_Multiplier[playerid] = 1.0;
    return 1;
}

/* Principal Menu Editor */
DialogCreate:PlayerIPMEEditor(playerid) {
    Dialog_Open(playerid, Dialog:PlayerIPMEEditor, DIALOG_STYLE_TABLIST, "Preview Model Editor", "{d1d1d1}Exportar\nModelo\nZoom\nMultiplicador\nFlechas\nManual\nLimpiar", "Seleccionar", "Volver");
    return 1;
}
DialogResponse:PlayerIPMEEditor(playerid, response, listitem, inputtext[]) {
    if (response) {
        switch(listitem) {
            case 0: {
                ShowPlayerIPME(playerid);
                SendPlayerClientMessage(playerid, "{"#SECONDARY_COLOR"}El modelo ha sido exportado con exito. Revisa la consola del servidor para más detalles.");
                printf("Player %d exported model: {%d, %f, %f, %f, %f}", playerid, p_IPME_ModelID[playerid], p_IPME_RotX[playerid], p_IPME_RotY[playerid], p_IPME_RotZ[playerid], p_IPME_Scale[playerid]);
            }
            case 1: {
                Dialog_Show(playerid, Dialog:PlayerIPMEModelInput);
            }
            case 2: {
                Dialog_Show(playerid, Dialog:PlayerIPMEZoomInput);
            }
            case 3: {
                Dialog_Show(playerid, Dialog:PlayerIPMEMultiplierInput);
            }
            case 4: {
                p_IPME_ArrowMode[playerid] ^= true;
                ShowPlayerIPME(playerid);
			    SendPlayerClientMessage(playerid, "{"#GREEN_COLOR"}Flechas laterales establecidas en el Modo %s", p_IPME_ArrowMode[playerid] ? ("Y") : ("Z"));
            }
            case 5: {
                Dialog_Show(playerid, Dialog:PlayerIPMEManualRotInput);
            }
            case 6: {
                ClearPlayerIPME(playerid);
                ShowPlayerIPME(playerid);
                SendPlayerClientMessage(playerid, "{"#GREEN_COLOR"}El editor ha sido limpiado a sus valores por defecto.");
            }
        }
    }
    else {
        ShowPlayerIPME(playerid);
    }
    return 1;
}

/* Model Input Selector */
DialogCreate:PlayerIPMEModelInput(playerid) {
    Dialog_Open(playerid, Dialog:PlayerIPMEModelInput, DIALOG_STYLE_INPUT, "Preview Model Editor", "{d1d1d1}Introduce el ID del modelo que deseas previsualizar:", "Aceptar", "Atras");
    return 1;
}
DialogResponse:PlayerIPMEModelInput(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (strval(inputtext) < 0) return SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}El ID de modelo introducido no es valido.");
        p_IPME_ModelID[playerid] = strval(inputtext);
        SendPlayerClientMessage(playerid, "{"#GREEN_COLOR"}Modelo establecido a ID %d.", p_IPME_ModelID[playerid]);
    }
    Dialog_Show(playerid, Dialog:PlayerIPMEEditor);
    return 1;
}

/* Zoom Input Selector */
DialogCreate:PlayerIPMEZoomInput(playerid) {
    Dialog_Open(playerid, Dialog:PlayerIPMEZoomInput, DIALOG_STYLE_INPUT, "Preview Model Editor", "{d1d1d1}Introduce el valor de zoom (escala) que deseas aplicar al modelo:", "Aceptar", "Atras");
    return 1;
}
DialogResponse:PlayerIPMEZoomInput(playerid, response, listitem, inputtext[]) {
    if (response) {
        new Float:zoom;
        if (sscanf(inputtext, "f", zoom)) return SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}El valor de zoom introducido no es valido.");
        p_IPME_Scale[playerid] = zoom;
        SendPlayerClientMessage(playerid, "{"#GREEN_COLOR"}Zoom establecido a %f.", zoom);
    }
    Dialog_Show(playerid, Dialog:PlayerIPMEEditor);
    return 1;
}

/* Multiplier Input Selector */
DialogCreate:PlayerIPMEMultiplierInput(playerid) {
    Dialog_Open(playerid, Dialog:PlayerIPMEMultiplierInput, DIALOG_STYLE_INPUT, "Preview Model Editor", "{d1d1d1}Introduce el valor del multiplicador para la rotacion (ejemplo: 1.0):", "Aceptar", "Atras");
    return 1;
}
DialogResponse:PlayerIPMEMultiplierInput(playerid, response, listitem, inputtext[]) {
    if (response) {
        new Float:multiplier;
        if (sscanf(inputtext, "f", multiplier)) return SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}El valor del multiplicador introducido no es valido.");
        p_IPME_Multiplier[playerid] = multiplier;
        SendPlayerClientMessage(playerid, "{"#GREEN_COLOR"}Multiplicador establecido a %.2f.", multiplier);
    }
    Dialog_Show(playerid, Dialog:PlayerIPMEEditor);
    return 1;
}

/* Multiplier input manual */
DialogCreate:PlayerIPMEManualRotInput(playerid) {
    Dialog_Open(playerid, Dialog:PlayerIPMEManualRotInput, DIALOG_STYLE_INPUT, "Preview Model Editor", "{d1d1d1}Introduce el valor de x,y,z,zoom que deseas aplicar al preview:", "Aceptar", "Atras");
    return 1;  
}
DialogResponse:PlayerIPMEManualRotInput(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (sscanf(inputtext, "p<,>ffff", p_IPME_RotX[playerid], p_IPME_RotY[playerid], p_IPME_RotZ[playerid], p_IPME_Scale[playerid])) return SendPlayerClientMessage(playerid, "{"#ERROR_COLOR"}El valor introducido no es valido.");
        SendPlayerClientMessage(playerid, "{"#GREEN_COLOR"}Valores establecidos a {%f, %f, %f, %f}", p_IPME_RotX[playerid], p_IPME_RotY[playerid], p_IPME_RotZ[playerid], p_IPME_Scale[playerid]);
    }
    Dialog_Show(playerid, Dialog:PlayerIPMEEditor);
    return 1;
}

/* Eth */
hook OnPlayerClickPlayerTD(playerid, PlayerText:playertextid) {
    if (p_IPMEditor[playerid]) {
        if (playertextid == p_InterfacePreviewModelEditor[playerid][2]) { // Up
            p_IPME_RotX[playerid] += p_IPME_Multiplier[playerid];
            ShowPlayerIPME(playerid);
            return Y_HOOKS_BREAK_RETURN_1;
        }
        else if (playertextid == p_InterfacePreviewModelEditor[playerid][3]) { // Down
            p_IPME_RotX[playerid] -= p_IPME_Multiplier[playerid];
            ShowPlayerIPME(playerid);
            return Y_HOOKS_BREAK_RETURN_1;
        }
        else if (playertextid == p_InterfacePreviewModelEditor[playerid][4]) { // Left
            if (p_IPME_ArrowMode[playerid]) p_IPME_RotY[playerid] -= p_IPME_Multiplier[playerid];
            else p_IPME_RotZ[playerid] -= p_IPME_Multiplier[playerid];
            ShowPlayerIPME(playerid);
            return Y_HOOKS_BREAK_RETURN_1;
        }
        else if (playertextid == p_InterfacePreviewModelEditor[playerid][5]) { // Right
            if (p_IPME_ArrowMode[playerid]) p_IPME_RotY[playerid] += p_IPME_Multiplier[playerid];
            else p_IPME_RotZ[playerid] += p_IPME_Multiplier[playerid];
            ShowPlayerIPME(playerid);
            return Y_HOOKS_BREAK_RETURN_1;
        }
        else if (playertextid == p_InterfacePreviewModelEditor[playerid][7]) { // Editor
            HidePlayerIPME(playerid, false);
            Dialog_Show(playerid, Dialog:PlayerIPMEEditor);
            return Y_HOOKS_BREAK_RETURN_1;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid) {
    if (p_IPMEditor[playerid]) {
		PLAYER_TEMP[playerid][pt_SELECT_TEXTDRAW] = false;
        if (clickedid == Text:INVALID_TEXT_DRAW) {
            HidePlayerIPME(playerid, false);
            return Y_HOOKS_BREAK_RETURN_1;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid) {
    p_IPMEditor[playerid] = false;
    ClearPlayerIPME(playerid);

    p_InterfacePreviewModelEditor[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 135.000, "bg");
    PlayerTextDrawLetterSize(playerid, p_InterfacePreviewModelEditor[playerid][0], 0.000, 25.000);
    PlayerTextDrawTextSize(playerid, p_InterfacePreviewModelEditor[playerid][0], 0.000, 160.000);
    PlayerTextDrawAlignment(playerid, p_InterfacePreviewModelEditor[playerid][0], 2);
    PlayerTextDrawColor(playerid, p_InterfacePreviewModelEditor[playerid][0], -1);
    PlayerTextDrawUseBox(playerid, p_InterfacePreviewModelEditor[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, p_InterfacePreviewModelEditor[playerid][0], 252645375);
    PlayerTextDrawSetShadow(playerid, p_InterfacePreviewModelEditor[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, p_InterfacePreviewModelEditor[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, p_InterfacePreviewModelEditor[playerid][0], 150);
    PlayerTextDrawFont(playerid, p_InterfacePreviewModelEditor[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, p_InterfacePreviewModelEditor[playerid][0], 1);

    p_InterfacePreviewModelEditor[playerid][1] = CreatePlayerTextDraw(playerid, 245.000, 140.000, "_");
    PlayerTextDrawTextSize(playerid, p_InterfacePreviewModelEditor[playerid][1], 150.000, 150.000);
    PlayerTextDrawAlignment(playerid, p_InterfacePreviewModelEditor[playerid][1], 1);
    PlayerTextDrawColor(playerid, p_InterfacePreviewModelEditor[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, p_InterfacePreviewModelEditor[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, p_InterfacePreviewModelEditor[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InterfacePreviewModelEditor[playerid][1], 255);
    PlayerTextDrawFont(playerid, p_InterfacePreviewModelEditor[playerid][1], 5);
    PlayerTextDrawSetProportional(playerid, p_InterfacePreviewModelEditor[playerid][1], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InterfacePreviewModelEditor[playerid][1], 19300);
    PlayerTextDrawSetPreviewRot(playerid, p_InterfacePreviewModelEditor[playerid][1], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InterfacePreviewModelEditor[playerid][1], 0, 0);

    p_InterfacePreviewModelEditor[playerid][2] = CreatePlayerTextDraw(playerid, 250.000, 305.000, "LD_BEAT:up");
    PlayerTextDrawTextSize(playerid, p_InterfacePreviewModelEditor[playerid][2], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, p_InterfacePreviewModelEditor[playerid][2], 1);
    PlayerTextDrawColor(playerid, p_InterfacePreviewModelEditor[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, p_InterfacePreviewModelEditor[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, p_InterfacePreviewModelEditor[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InterfacePreviewModelEditor[playerid][2], 255);
    PlayerTextDrawFont(playerid, p_InterfacePreviewModelEditor[playerid][2], 4);
    PlayerTextDrawSetProportional(playerid, p_InterfacePreviewModelEditor[playerid][2], 1);
    PlayerTextDrawSetSelectable(playerid, p_InterfacePreviewModelEditor[playerid][2], 1);

    p_InterfacePreviewModelEditor[playerid][3] = CreatePlayerTextDraw(playerid, 270.000, 305.000, "LD_BEAT:down");
    PlayerTextDrawTextSize(playerid, p_InterfacePreviewModelEditor[playerid][3], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, p_InterfacePreviewModelEditor[playerid][3], 1);
    PlayerTextDrawColor(playerid, p_InterfacePreviewModelEditor[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, p_InterfacePreviewModelEditor[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, p_InterfacePreviewModelEditor[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InterfacePreviewModelEditor[playerid][3], 255);
    PlayerTextDrawFont(playerid, p_InterfacePreviewModelEditor[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, p_InterfacePreviewModelEditor[playerid][3], 1);
    PlayerTextDrawSetSelectable(playerid, p_InterfacePreviewModelEditor[playerid][3], 1);

    p_InterfacePreviewModelEditor[playerid][4] = CreatePlayerTextDraw(playerid, 350.000, 305.000, "LD_BEAT:left");
    PlayerTextDrawTextSize(playerid, p_InterfacePreviewModelEditor[playerid][4], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, p_InterfacePreviewModelEditor[playerid][4], 1);
    PlayerTextDrawColor(playerid, p_InterfacePreviewModelEditor[playerid][4], -1);
    PlayerTextDrawSetShadow(playerid, p_InterfacePreviewModelEditor[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, p_InterfacePreviewModelEditor[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InterfacePreviewModelEditor[playerid][4], 255);
    PlayerTextDrawFont(playerid, p_InterfacePreviewModelEditor[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, p_InterfacePreviewModelEditor[playerid][4], 1);
    PlayerTextDrawSetSelectable(playerid, p_InterfacePreviewModelEditor[playerid][4], 1);

    p_InterfacePreviewModelEditor[playerid][5] = CreatePlayerTextDraw(playerid, 373.000, 305.000, "LD_BEAT:right");
    PlayerTextDrawTextSize(playerid, p_InterfacePreviewModelEditor[playerid][5], 20.000, 20.000);
    PlayerTextDrawAlignment(playerid, p_InterfacePreviewModelEditor[playerid][5], 1);
    PlayerTextDrawColor(playerid, p_InterfacePreviewModelEditor[playerid][5], -1);
    PlayerTextDrawSetShadow(playerid, p_InterfacePreviewModelEditor[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, p_InterfacePreviewModelEditor[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InterfacePreviewModelEditor[playerid][5], 255);
    PlayerTextDrawFont(playerid, p_InterfacePreviewModelEditor[playerid][5], 4);
    PlayerTextDrawSetProportional(playerid, p_InterfacePreviewModelEditor[playerid][5], 1);
    PlayerTextDrawSetSelectable(playerid, p_InterfacePreviewModelEditor[playerid][5], 1);

    p_InterfacePreviewModelEditor[playerid][6] = CreatePlayerTextDraw(playerid, 320.000, 293.000, "0,0,0,1.000");
    PlayerTextDrawLetterSize(playerid, p_InterfacePreviewModelEditor[playerid][6], 0.150, 0.800);
    PlayerTextDrawAlignment(playerid, p_InterfacePreviewModelEditor[playerid][6], 2);
    PlayerTextDrawColor(playerid, p_InterfacePreviewModelEditor[playerid][6], -1);
    PlayerTextDrawSetShadow(playerid, p_InterfacePreviewModelEditor[playerid][6], 1);
    PlayerTextDrawSetOutline(playerid, p_InterfacePreviewModelEditor[playerid][6], 1);
    PlayerTextDrawBackgroundColor(playerid, p_InterfacePreviewModelEditor[playerid][6], 150);
    PlayerTextDrawFont(playerid, p_InterfacePreviewModelEditor[playerid][6], 2);
    PlayerTextDrawSetProportional(playerid, p_InterfacePreviewModelEditor[playerid][6], 1);

    p_InterfacePreviewModelEditor[playerid][7] = CreatePlayerTextDraw(playerid, 320.000, 340.000, "Editor");
    PlayerTextDrawLetterSize(playerid, p_InterfacePreviewModelEditor[playerid][7], 0.250, 1.500);
    PlayerTextDrawTextSize(playerid, p_InterfacePreviewModelEditor[playerid][7], 15.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InterfacePreviewModelEditor[playerid][7], 2);
    PlayerTextDrawColor(playerid, p_InterfacePreviewModelEditor[playerid][7], -1);
    PlayerTextDrawUseBox(playerid, p_InterfacePreviewModelEditor[playerid][7], 1);
    PlayerTextDrawBoxColor(playerid, p_InterfacePreviewModelEditor[playerid][7], -1976835329);
    PlayerTextDrawSetShadow(playerid, p_InterfacePreviewModelEditor[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, p_InterfacePreviewModelEditor[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InterfacePreviewModelEditor[playerid][7], 150);
    PlayerTextDrawFont(playerid, p_InterfacePreviewModelEditor[playerid][7], 1);
    PlayerTextDrawSetProportional(playerid, p_InterfacePreviewModelEditor[playerid][7], 1);
    PlayerTextDrawSetSelectable(playerid, p_InterfacePreviewModelEditor[playerid][7], 1);
    return Y_HOOKS_CONTINUE_RETURN_1;
}