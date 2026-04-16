/*

    Interaction Menu
        Impl

*/

#include <YSI_Coding\y_hooks>

#define TIME_BETWEEN_GIVE_CASH	30 // segundos

/* HEADER */
new g_interaction_menu_options[][] = {
    "GPS",
    "Telefono",
    "Guia telefonica",
    "Consumir medicamento",
    "Consumir marihuana",
    "Consumir crack",
    "Dar dinero",
    "Vender objeto",
    "Tirar objeto",
    "Dar objeto",
    "Estadisticas",
    "Experiencia",
    "Inventario",
    "Configuracion"
};

/* IMPL */
DialogCreate:InteractionMenu(playerid) {
    new dialog[80 * sizeof(g_interaction_menu_options) + 3], listitem;
    for (new i = 0; i != MAX_LISTITEMS; i++) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;

    for (new i = 0; i != sizeof(g_interaction_menu_options); i ++) {
        strcatf(dialog, "{"#SECONDARY_COLOR"}·{ffffff}%s\n", g_interaction_menu_options[i]);
        PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] ++;
        listitem ++;
    }
    Dialog_Open(playerid, Dialog:InteractionMenu, DIALOG_STYLE_TABLIST, "{"#PRIMARY_COLOR"}·{ffffff}Menu de interaccion", dialog, "Continuar", "Cerrar");
    return 1;
}
DialogResponse:InteractionMenu(playerid, response, listitem, inputtext[]) {
    if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK || PI[playerid][pi_STATE] == ROLEPLAY_STATE_JAIL || PI[playerid][pi_STATE] == ROLEPLAY_STATE_ARRESTED) return 1;
    if (response) {
        switch (listitem) {
            case 0: PC_EmulateCommand(playerid, "/gps");
            case 1: PC_EmulateCommand(playerid, "/movil");
            case 2: PC_EmulateCommand(playerid, "/guia");
            case 3: PC_EmulateCommand(playerid, "/consumir medicamento");
            case 4: PC_EmulateCommand(playerid, "/consumir marihuana");
            case 5: PC_EmulateCommand(playerid, "/consumir crack");
            case 6: Dialog_Show(playerid, Dialog:PlayerGiveCashSelectPlayer);
            case 7: Dialog_Show(playerid, Dialog:PlayerSellInvDialog);
            case 8: Dialog_Show(playerid, Dialog:PlayerDropInvDialog);
            case 9: Dialog_Show(playerid, Dialog:PlayerGiveInvDialog);
            case 10: ShowPlayerStats(playerid, playerid);
            case 11: ShowPlayerSkills(playerid, playerid);
            case 12: ShowPlayerInventory(playerid);
            case 13: PC_EmulateCommand(playerid, "/panel");
        }
    }
    return 1;
}

/* PHONE_BOOK */
DialogCreate:PlayerPhoneBookFind(playerid) {
    Dialog_Open(playerid, Dialog:PlayerPhoneBookFind, DIALOG_STYLE_INPUT, "{"#PRIMARY_COLOR"}·{ffffff}Guia telefonica", "{"#SECONDARY_COLOR"}·{ffffff}Introduce la id o parte de el nombre para buscar el numero telefonico de la persona:", "Buscar", "Cancelar");
}
DialogResponse:PlayerPhoneBookFind(playerid, response, listitem, inputtext[]) {
    if (response) {
        PLAYER_TEMP[playerid][pt_PLAYER_PHONE_NUMBER_SELECTED] = 0;

        if (sscanf(inputtext, "u", inputtext[0])) return Dialog_Open(playerid, Dialog:PlayerPhoneBookResult, DIALOG_STYLE_INPUT, "{"#PRIMARY_COLOR"}·{ffffff}Guia telefonica", "{"#SECONDARY_COLOR"}·{ffffff}Parametros incorrectos, introduce el id o el nombre de la persona.", "Volver", "Cancelar");
        if (!IsPlayerConnected(inputtext[0])) return Dialog_Open(playerid, Dialog:PlayerPhoneBookResult, DIALOG_STYLE_INPUT, "{"#PRIMARY_COLOR"}·{ffffff}Guia telefonica", "{"#SECONDARY_COLOR"}·{ffffff}El jugador proporcionado se encuentra desconectado.", "Volver", "Cancelar");

        if (!GetValidPlayerPhoneNumber(inputtext[0])) return Dialog_Open(playerid, Dialog:PlayerPhoneBookResult, DIALOG_STYLE_INPUT, "{"#PRIMARY_COLOR"}·{ffffff}Guia telefonica", "{"#SECONDARY_COLOR"}·{ffffff}No pudimos encontrar el numero telefonico de la persona.", "Volver", "Cancelar");
        if (!PI[inputtext[0]][pi_PHONE_VISIBLE_NUMBER]) return Dialog_Open(playerid, Dialog:PlayerPhoneBookResult, DIALOG_STYLE_INPUT, "{"#PRIMARY_COLOR"}·{ffffff}Guia telefonica", "{"#SECONDARY_COLOR"}·{ffffff}Este jugador ha decidido no mostrar su número en la guía.", "Volver", "Cancelar");

        PLAYER_TEMP[playerid][pt_PLAYER_PHONE_NUMBER_SELECTED] = GetValidPlayerPhoneNumber(inputtext[0]);

        new dialog_result[128 + 48 + 3];
        format(dialog_result, sizeof(dialog_result), "{"#PRIMARY_COLOR"}[Guía] ·{FFFFFF}Persona encontrada | Nombre: {"#SECONDARY_COLOR"}%s {FFFFFF}| Teléfono: {"#SECONDARY_COLOR"}%d", PLAYER_TEMP[inputtext[0]][pt_NAME], GetValidPlayerPhoneNumber(inputtext[0]));
        Dialog_Open(playerid, Dialog:PlayerPhoneBookResult, DIALOG_STYLE_MSGBOX, "{"#PRIMARY_COLOR"}·{ffffff}Guia telefonica", dialog_result, "LLamar", "Cerrar");
    }
    return 1;
}
DialogResponse:PlayerPhoneBookResult(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (PLAYER_TEMP[playerid][pt_PLAYER_PHONE_NUMBER_SELECTED] <= 0) {
            Dialog_Show(playerid, Dialog:PlayerPhoneBookFind);
            return 1;
        }

        new selected_number = PLAYER_TEMP[playerid][pt_PLAYER_PHONE_NUMBER_SELECTED];
        StartPlayerCallUsingPhoneNumber(playerid, selected_number);
    }
    return 1;
}

/* SELL INVENTORY */
DialogCreate:PlayerSellInvDialog(playerid) {
    new dialog[80 * MAX_PLAYER_SLOTS + 256];
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) strcatf(dialog, "{cccccc}%d. Slot Vacio\n", (i + 1));
        else strcatf(dialog, "%s {"#SECONDARY_COLOR"}[%d]\n", g_inventory_data[ p_inventory[playerid][i][p_inventory_type] ][g_info_name], p_inventory[playerid][i][p_inventory_amount]);
    }
    Dialog_Open(playerid, Dialog:PlayerSellInvDialog, DIALOG_STYLE_TABLIST, "{"#PRIMARY_COLOR"}Vender Objeto{f0f0f0} » Seleccionar", dialog, "Seleccionar", "Cerrar");
    return 1;
}
DialogResponse:PlayerSellInvDialog(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (PI[playerid][pi_LEVEL] < 15) return ShowPlayerMessage(playerid, "Necesitas ser nivel 15 para poder hacer esto.");
        i_CurrentSlot[playerid] = listitem;
        if (IsPlayerWorking(playerid, WORK_POLICE)) {
            if (IsInventoryTypeWeapon(p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type])) {
                ShowPlayerMessage(playerid, "~y~Advertencia~n~~w~~h~No puedes hacer esto con tus armas mientras trabajas como policía.");
                return 1;
            }
        }

        Dialog_Show(playerid, Dialog:PlayerSellInvSlot);
    }
    return 1;
}

/* GIVE INVENTORY */
DialogCreate:PlayerGiveInvDialog(playerid) {
    new dialog[80 * MAX_PLAYER_SLOTS + 256];
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) strcatf(dialog, "{cccccc}%d. Slot Vacio\n", (i + 1));
        else strcatf(dialog, "%s {"#SECONDARY_COLOR"}[%d]\n", g_inventory_data[ p_inventory[playerid][i][p_inventory_type] ][g_info_name], p_inventory[playerid][i][p_inventory_amount]);
    }
    Dialog_Open(playerid, Dialog:PlayerGiveInvDialog, DIALOG_STYLE_TABLIST, "{"#PRIMARY_COLOR"}Dar Objeto{f0f0f0} » Seleccionar", dialog, "Seleccionar", "Cerrar");
    return 1;
}
DialogResponse:PlayerGiveInvDialog(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (PI[playerid][pi_LEVEL] < 15) return ShowPlayerMessage(playerid, "Necesitas ser nivel 15 para poder hacer esto.");
        i_CurrentSlot[playerid] = listitem;
        if (IsPlayerWorking(playerid, WORK_POLICE)) {
            if (IsInventoryTypeWeapon(p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type])) {
                ShowPlayerMessage(playerid, "~y~Advertencia~n~~w~~h~No puedes hacer esto con tus armas mientras trabajas como policía.");
                return 1;
            }
        }

        Dialog_Show(playerid, Dialog:PlayerGiveInvSlot);
    }
    return 1;
}

/* DROP INVENTORY */
DialogCreate:PlayerDropInvDialog(playerid) {
    new dialog[80 * MAX_PLAYER_SLOTS + 256];
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) strcatf(dialog, "{cccccc}%d. Slot Vacio\n", (i + 1));
        else strcatf(dialog, "%s {"#SECONDARY_COLOR"}[%d]\n", g_inventory_data[ p_inventory[playerid][i][p_inventory_type] ][g_info_name], p_inventory[playerid][i][p_inventory_amount]);
    }
    Dialog_Open(playerid, Dialog:PlayerDropInvDialog, DIALOG_STYLE_TABLIST, "{"#PRIMARY_COLOR"}Tirar Objeto{f0f0f0} » Seleccionar", dialog, "Seleccionar", "Cerrar");
    return 1;
}
DialogResponse:PlayerDropInvDialog(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (PI[playerid][pi_LEVEL] < 15) return ShowPlayerMessage(playerid, "Necesitas ser nivel 15 para poder hacer esto.");
        i_CurrentSlot[playerid] = listitem;
        if (IsPlayerWorking(playerid, WORK_POLICE)) {
            if (IsInventoryTypeWeapon(p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type])) {
                ShowPlayerMessage(playerid, "~y~Advertencia~n~~w~~h~No puedes hacer esto con tus armas mientras trabajas como policía.");
                return 1;
            }
        }

        Auto_SendPlayerAction(playerid, "tiro %s al suelo.", g_inventory_data[ p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] ][g_info_name]);
        ApplyAnimation(playerid, "CARRY", "putdwn05", 4.1, 0, 1, 1, 0, 0, 1);
        DropPlayerInventorySlot(playerid, i_CurrentSlot[playerid]);
    }
    return 1;
}

/* GIVE CASH */
DialogCreate:PlayerGiveCashSelectPlayer(playerid) {
    new dialog_body[2000] = "{f0f0f0}Introduce el {"#SECONDARY_COLOR"}ID de el jugador{f0f0f0} al que quieres darle dinero\n\n{cccccc}Jugadores Cercanos:\n", counter = 0;
    for(new i = 0; i != MAX_PLAYERS; i++) {
        if(i == playerid) continue;
        if(IsPlayerConnected(i)) {
            new Float:pos[3];
            GetPlayerPos(i, pos[0], pos[1], pos[2]);
            if(IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2]) && !PLAYER_TEMP[i][pt_PLAYER_SPECTATE]) {
                strcatf(dialog_body, "- {ffeb1e}[{ffc81e}ID: {f0f0f0}%d{ffeb1e}] {f0f0f0}%s\n", i, PLAYER_TEMP[i][pt_NAME]);
                counter ++;
                if(counter > 5) break;
            }
        }
    }
    if (!counter) strcat(dialog_body, "{666666}No hay jugadores cerca.");
    Dialog_Open(playerid, Dialog:PlayerGiveCashSelectPlayer, DIALOG_STYLE_INPUT, "{"#PRIMARY_COLOR"}Dar Dinero{f0f0f0} » Cantidad", dialog_body, "Continuar", "Cerrar");
    return 1;
}
DialogResponse:PlayerGiveCashSelectPlayer(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (PI[playerid][pi_LEVEL] < 15) return ShowPlayerMessage(playerid, "Necesitas ser nivel 15 para poder hacer esto.");
        if (gettime() < PLAYER_TEMP[playerid][pt_LAST_GIVE_MONEY_TIME] + TIME_BETWEEN_GIVE_CASH) {
            new time = TIME_BETWEEN_GIVE_CASH - (gettime() - PLAYER_TEMP[playerid][pt_LAST_GIVE_MONEY_TIME]);
            ShowPlayerMessage(playerid, "Debes de esperar %s minutos para volver a dar dinero.", TimeConvert(time));
            return 1;
        }

        new to_playerid;
        if (sscanf(inputtext, "u", to_playerid)) return ShowPlayerMessage(playerid, "Error en los parámetros.");
        if (to_playerid == playerid) return ShowPlayerMessage(playerid, "No puedes darte dinero a ti mismo.");
        if (!IsPlayerConnected(to_playerid)) return ShowPlayerMessage(playerid, "El jugador no está conectado.");

        new Float:pos[3]; GetPlayerPos(to_playerid, pos[0], pos[1], pos[2]);
        if (!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return ShowPlayerMessage(playerid, "Este jugador no está cerca tuya.");
        if (PLAYER_TEMP[to_playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return ShowPlayerMessage(playerid, "No puedes darle dinero a este jugador ahora.");
        
        PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] = to_playerid;
	    PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID] = PI[to_playerid][pi_ID];
        Dialog_Show(playerid, Dialog:PlayerGiveCashAmount);
    }
    return 1;
}

DialogCreate:PlayerGiveCashAmount(playerid) {
    Dialog_Open(playerid, Dialog:PlayerGiveCashAmount, DIALOG_STYLE_INPUT, "{"#PRIMARY_COLOR"}·{ffffff}Dar dinero", "{"#SECONDARY_COLOR"}·{ffffff}Introduce la cantidad de dinero que quieres darle al jugador:", "Dar", "Cancelar");
}
DialogResponse:PlayerGiveCashAmount(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (gettime() < PLAYER_TEMP[playerid][pt_LAST_GIVE_MONEY_TIME] + TIME_BETWEEN_GIVE_CASH) {
            new time = TIME_BETWEEN_GIVE_CASH - (gettime() - PLAYER_TEMP[playerid][pt_LAST_GIVE_MONEY_TIME]);
            ShowPlayerMessage(playerid, "Debes de esperar %s minutos para volver a dar dinero.", TimeConvert(time));
            return 1;
        }

        new amount;
        if (sscanf(inputtext, "d", amount)) return ShowPlayerMessage(playerid, "Error en los parámetros.");
        if (amount <= 0) return ShowPlayerMessage(playerid, "La cantidad debe ser mayor a 0.");
        if (amount > PI[playerid][pi_CASH]) return ShowPlayerMessage(playerid, "No tienes suficiente dinero para dar esa cantidad.");

        new Float:pos[3], to_playerid = PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID];
        if (!IsPlayerConnected(to_playerid)) return ShowPlayerMessage(playerid, "El jugador no está conectado.");
        if (PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID] != PI[to_playerid][pi_ID]) return ShowPlayerMessage(playerid, "El jugador no está conectado.");
        GetPlayerPos(to_playerid, pos[0], pos[1], pos[2]);
        if (!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return ShowPlayerMessage(playerid, "Este jugador ya no está cerca de ti.");
        if (PLAYER_TEMP[to_playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return ShowPlayerMessage(playerid, "No puedes darle dinero a este jugador ahora.");

        if (GivePlayerCash(playerid, -amount, true, true) && GivePlayerCash(to_playerid, amount, true, false)) {
            ShowPlayerMessage(playerid, "Has dado %s~g~$~w~~h~ a %s.", number_format_thousand(amount), PLAYER_TEMP[to_playerid][pt_NAME]);
            ShowPlayerMessage(to_playerid, "Has recibido %s~g~$~w~~h~ de %s.", number_format_thousand(amount), PLAYER_TEMP[playerid][pt_NAME]);
            Auto_SendPlayerAction(playerid, "le da dinero en efectivo a %s.", PLAYER_TEMP[to_playerid][pt_NAME]);

            PLAYER_TEMP[playerid][pt_LAST_GIVE_MONEY_TIME] = gettime();
        }
    }
    return 1;
}

/* HOOKS */
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (newkeys & KEY_YES) {
        if (!IsPlayerInDynamicArea(playerid, Farmer_Area) && PI[playerid][pi_STATE] != ROLEPLAY_STATE_CRACK && PI[playerid][pi_STATE] != ROLEPLAY_STATE_JAIL && PI[playerid][pi_STATE] != ROLEPLAY_STATE_ARRESTED) {
            if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) Dialog_Show(playerid, Dialog:InteractionMenu);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
