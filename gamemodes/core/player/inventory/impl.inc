/* 

    Inventory
        Impl

*/

#define IsInventoryTypeWeapon(%0) (%0>=INVENTORY_BRASSKNUCKLE&&%0<=INVENTORY_CAMERA)

#include <YSI_Coding\y_hooks>

stock ShowPlayerInventory(const playerid) {
    va_PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][19], "%d", floatround(PI[playerid][pi_HEALTH]));
    va_PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][21], "%d", floatround(PI[playerid][pi_ARMOUR]));

    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][15], PI[playerid][pi_SKIN]);
    
    if (i_CurrentSlot[playerid] == -1) {
        PlayerTextDrawColor(playerid, p_InventoryTd[playerid][23], 1768516095);
        PlayerTextDrawColor(playerid, p_InventoryTd[playerid][24], 1768516095);
        PlayerTextDrawColor(playerid, p_InventoryTd[playerid][25], 1768516095);
        PlayerTextDrawColor(playerid, p_InventoryTd[playerid][26], 1768516095);

        PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][23], "USAR");
        
        PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][16], "Inventario");
        PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][17], "Selecciona un objeto de tu inventario para ver su informacion.");
    } else {
        PlayerTextDrawColor(playerid, p_InventoryTd[playerid][23], -1);
        PlayerTextDrawColor(playerid, p_InventoryTd[playerid][24], -1);
        PlayerTextDrawColor(playerid, p_InventoryTd[playerid][25], -1);
        PlayerTextDrawColor(playerid, p_InventoryTd[playerid][26], -1);
    
        va_PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][16], "%s (%d)", g_inventory_data[ p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] ][g_info_name], p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_amount]);
        PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][17], g_inventory_data[ p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] ][g_info_description]);

        switch (p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type]) {
            case INVENTORY_PHONE, INVENTORY_PHONE_RESOLVER: {
                if (inventory_slot_info[ i_CurrentSlot[playerid] ][i_specific_slot_type] != p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type]) {
                    PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][23], "EQUIPAR");
                } else {
                    PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][23], "USAR");
                }
            }
            case INVENTORY_BRASSKNUCKLE .. INVENTORY_CAMERA: {
                if (p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_data] == 0) PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][23], "EQUIPAR");
                else PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][23], "GUARDAR");
            }
            case INVENTORY_MECHANIC_PIECES .. INVENTORY_CRACK: PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][23], "JUNTAR");
            default: PlayerTextDrawSetString(playerid, p_InventoryTd[playerid][23], "USAR");
        }
    }

    if (p_inventory[playerid][0][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][2], g_inventory_data[ p_inventory[playerid][0][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][2], g_inventory_data[ p_inventory[playerid][0][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][0][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][0][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][0][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][2], 19300);
    if (p_inventory[playerid][1][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][3], g_inventory_data[ p_inventory[playerid][1][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][3], g_inventory_data[ p_inventory[playerid][1][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][1][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][1][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][1][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][3], 19300);
    if (p_inventory[playerid][2][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][4], g_inventory_data[ p_inventory[playerid][2][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][4], g_inventory_data[ p_inventory[playerid][2][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][2][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][2][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][2][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][4], 19300);
    if (p_inventory[playerid][3][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][5], g_inventory_data[ p_inventory[playerid][3][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][5], g_inventory_data[ p_inventory[playerid][3][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][3][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][3][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][3][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][5], 19300);
    if (p_inventory[playerid][4][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][6], g_inventory_data[ p_inventory[playerid][4][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][6], g_inventory_data[ p_inventory[playerid][4][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][4][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][4][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][4][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][6], 19300);
    if (p_inventory[playerid][5][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][7], g_inventory_data[ p_inventory[playerid][5][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][7], g_inventory_data[ p_inventory[playerid][5][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][5][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][5][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][5][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][7], 19300);
    if (p_inventory[playerid][6][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][8], g_inventory_data[ p_inventory[playerid][6][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][8], g_inventory_data[ p_inventory[playerid][6][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][6][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][6][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][6][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][8], 19300);
    if (p_inventory[playerid][7][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][9], g_inventory_data[ p_inventory[playerid][7][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][9], g_inventory_data[ p_inventory[playerid][7][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][7][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][7][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][7][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][9], 19300);
    if (p_inventory[playerid][8][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][10], g_inventory_data[ p_inventory[playerid][8][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][10], g_inventory_data[ p_inventory[playerid][8][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][8][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][8][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][8][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][10], 19300);
    if (p_inventory[playerid][9][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][11], g_inventory_data[ p_inventory[playerid][9][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][11], g_inventory_data[ p_inventory[playerid][9][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][9][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][9][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][9][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][11], 19300);
    if (p_inventory[playerid][10][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][12], g_inventory_data[ p_inventory[playerid][10][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][12], g_inventory_data[ p_inventory[playerid][10][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][10][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][10][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][10][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][12], 19300);
    if (p_inventory[playerid][11][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][13], g_inventory_data[ p_inventory[playerid][11][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][13], g_inventory_data[ p_inventory[playerid][11][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][11][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][11][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][11][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][13], 19300);
    if (p_inventory[playerid][12][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][27], g_inventory_data[ p_inventory[playerid][12][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][27], g_inventory_data[ p_inventory[playerid][12][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][12][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][12][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][12][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][27], 19300);
    if (p_inventory[playerid][13][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][28], g_inventory_data[ p_inventory[playerid][13][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][28], g_inventory_data[ p_inventory[playerid][13][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][13][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][13][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][13][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][28], 19300);
    if (p_inventory[playerid][14][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][29], g_inventory_data[ p_inventory[playerid][14][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][29], g_inventory_data[ p_inventory[playerid][14][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][14][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][14][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][14][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][29], 19300);
    if (p_inventory[playerid][15][p_inventory_valid]) {
        PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][30], g_inventory_data[ p_inventory[playerid][15][p_inventory_type] ][g_info_model]);
        PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][30], g_inventory_data[ p_inventory[playerid][15][p_inventory_type] ][g_info_rot_x], g_inventory_data[ p_inventory[playerid][15][p_inventory_type] ][g_info_rot_y], g_inventory_data[ p_inventory[playerid][15][p_inventory_type] ][g_info_rot_z], g_inventory_data[ p_inventory[playerid][15][p_inventory_type] ][g_info_zoom]);
    }
    else PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][30], 19300);

    for (new i = 0; i != MAX_PLAYER_INVENTORY_TD; i ++) {
        PlayerTextDrawShow(playerid, p_InventoryTd[playerid][i]);
    }

    i_PlayerInventoryShow[playerid] = true;
    SelectTextDrawEx(playerid, 0x8340FFFF);
    return 1;
}

stock HidePlayerInventory(const playerid) {
    for (new i = 0; i != MAX_PLAYER_INVENTORY_TD; i ++) {
        PlayerTextDrawHide(playerid, p_InventoryTd[playerid][i]);
    }
    i_PlayerInventoryShow[playerid] = false;
    //i_CurrentSlot[playerid] = -1;
    CancelSelectTextDrawEx(playerid);
    return 1;
}

stock ShowPlayerDialogInventory(const playerid, const pid) {
    new dialog[80 * MAX_PLAYER_SLOTS + 256];
    
    strcatf(dialog, "{f0f0f0}Dinero {d1d1d1}(%s{27F587}${d1d1d1})\n", number_format_thousand(PI[pid][pi_CASH]));
    new toys = CountPlayerToys(pid);
	if (toys > 0) strcatf(dialog, "{f0f0f0}Accesorios {d1d1d1}(%d)\n", toys);

    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[pid][i][p_inventory_valid]) strcatf(dialog, "{cccccc}%d. Slot Vacio\n", (i + 1));
        else strcatf(dialog, "%s {"#SECONDARY_COLOR"}[%d]\n", g_inventory_data[ p_inventory[pid][i][p_inventory_type] ][g_info_name], p_inventory[pid][i][p_inventory_amount]);
    }
    Dialog_Open(playerid, Dialog:Message, DIALOG_STYLE_TABLIST, "Inventario", dialog, "Cerrar", "");
    return 1;
}

stock ShowPlayerDialogInventoryEx(const playerid, const pid) {
    new dialog[200 * MAX_PLAYER_SLOTS + 256];
    
    strcatf(dialog, "{f0f0f0}Dinero {d1d1d1}(%s{27F587}${d1d1d1})\n", number_format_thousand(PI[pid][pi_CASH]));
    new toys = CountPlayerToys(pid);
	if (toys > 0) strcatf(dialog, "{f0f0f0}Accesorios {d1d1d1}(%d)\n", toys);

    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[pid][i][p_inventory_valid]) strcatf(dialog, "{cccccc}%d. Slot Vacio\n", (i + 1));
        else strcatf(dialog, "%s {"#SECONDARY_COLOR"}[amount: %d, data: %d, extra: %d]\n", g_inventory_data[ p_inventory[pid][i][p_inventory_type] ][g_info_name], p_inventory[pid][i][p_inventory_amount], p_inventory[pid][i][p_inventory_data], p_inventory[pid][i][p_inventory_extra]);
    }
    Dialog_Open(playerid, Dialog:Message, DIALOG_STYLE_TABLIST, "Inventario", dialog, "Cerrar", "");
    return 1;
}

stock GetPlayerInventorySlot(const playerid) {
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if(!p_inventory[playerid][i][p_inventory_valid]) return i;
    }
    return -1;
}

GetSpecificSlotIndex(const type) {
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (inventory_slot_info[i][i_specific_slot_type] == type) return i;
    }
    return -1;
}

stock GetValidPlayerPhoneNumber(const playerid) {
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if(p_inventory[playerid][i][p_inventory_valid]) {
            if (p_inventory[playerid][i][p_inventory_type] == INVENTORY_PHONE) {
                return p_inventory[playerid][i][p_inventory_data];
            }
        }
    }
    return 0;
}

stock GetValidPlayerInventoryType(const playerid, const type) {
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if(p_inventory[playerid][i][p_inventory_valid]) {
            if (p_inventory[playerid][i][p_inventory_type] == type) {
                return i;
            }
        }
    }
    return -1;
}

stock RemovePlayerInventorySlot(const playerid, const slot) {
    p_inventory[playerid][slot][p_inventory_valid] = false;
    p_inventory[playerid][slot][p_inventory_type] = 0;
    p_inventory[playerid][slot][p_inventory_amount] = 0;
    p_inventory[playerid][slot][p_inventory_data] = 0;
    p_inventory[playerid][slot][p_inventory_extra] = 0;
   
    mysql_format(handle_db, QUERY_BUFFER, sizeof(QUERY_BUFFER), "DELETE FROM player_inventory WHERE id = %d;", p_inventory[playerid][slot][p_inventory_id]);
    mysql_tquery(handle_db, QUERY_BUFFER);
    return 1;
}

DropPlayerInventorySlot(const playerid, const slot) {
    if (slot == -1) return 0;
    if (!p_inventory[playerid][slot][p_inventory_valid]) return 0;

    new Dropped_inventory_data[E_DroppedInventoryObjects];

    p_inventory[playerid][slot][p_inventory_valid] = false;

    Dropped_inventory_data[drp_Type] = p_inventory[playerid][slot][p_inventory_type];
    p_inventory[playerid][slot][p_inventory_type] = 0;

    Dropped_inventory_data[drp_Amount] = p_inventory[playerid][slot][p_inventory_amount];
    p_inventory[playerid][slot][p_inventory_amount] = 0;

    Dropped_inventory_data[drp_Data] = p_inventory[playerid][slot][p_inventory_data];
    p_inventory[playerid][slot][p_inventory_data] = 0;

    Dropped_inventory_data[drp_Extra] = p_inventory[playerid][slot][p_inventory_extra];
    p_inventory[playerid][slot][p_inventory_extra] = 0;

    RemovePlayerInventorySlot(playerid, slot);
    UpdateSetPlayerEquippedWeapons(playerid);

    Dropped_inventory_data[drp_World] = GetPlayerVirtualWorld(playerid);
    Dropped_inventory_data[drp_Interior] = GetPlayerInterior(playerid);
    GetPlayerPos(playerid, Dropped_inventory_data[drp_X], Dropped_inventory_data[drp_Y], Dropped_inventory_data[drp_Z]);

    Dropped_inventory_data[drp_ObjectId] = CreateDynamicObject(g_inventory_data[ Dropped_inventory_data[drp_Type] ][g_info_model], Dropped_inventory_data[drp_X], Dropped_inventory_data[drp_Y], Dropped_inventory_data[drp_Z] - 1.0, 90.0, 0.0, float(random(360)), Dropped_inventory_data[drp_World], Dropped_inventory_data[drp_Interior]);

    new label_str[256];
    format(label_str, sizeof label_str, "%s {"#SECONDARY_COLOR"}[%d]", g_inventory_data[  Dropped_inventory_data[drp_Type] ][g_info_name], Dropped_inventory_data[drp_Amount]);
    Dropped_inventory_data[drp_LabelId] = CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, Dropped_inventory_data[drp_X], Dropped_inventory_data[drp_Y], Dropped_inventory_data[drp_Z] - 0.7, 5.0, .testlos = true, .interiorid = Dropped_inventory_data[drp_Interior], .worldid = Dropped_inventory_data[drp_World]);

    list_add_arr(DroppedInventoryObjects, Dropped_inventory_data);
    return 1;
}

stock UsePlayerInventorySlot(const playerid, const slot) {
    if (!p_inventory[playerid][slot][p_inventory_valid]) return 0;

    switch (p_inventory[playerid][slot][p_inventory_type]) {
        case INVENTORY_BURGER .. INVENTORY_BEER: { // Comida
            p_inventory[playerid][slot][p_inventory_amount] --;
            
			Add_Hungry_Thirst(playerid, g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_hungry], g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_thirst]);
			GivePlayerDrunkLevel(playerid, g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_drunk]);
			
			new action[64];
			format(action, sizeof action, "consume %s.", g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_name]);
			Auto_SendPlayerAction(playerid, action);

            if (p_inventory[playerid][slot][p_inventory_amount] <= 0) {
                RemovePlayerInventorySlot(playerid, slot);
            } else {
                SavePlayerInventorySlot(playerid, slot);
            }

			ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 0, 0, 0, 0, 0, 0);
			ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.1, false, true, true, false, 1000);

            HidePlayerInventory(playerid);
        }
        case INVENTORY_PHONE: {
            if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK || PI[playerid][pi_STATE] == ROLEPLAY_STATE_JAIL || PI[playerid][pi_STATE] == ROLEPLAY_STATE_ARRESTED) return Y_HOOKS_BREAK_RETURN_0;
            HidePlayerInventory(playerid);
            if (PLAYER_TEMP[playerid][pt_PLAYER_IN_CALL]) SendPlayerClientMessage(playerid, "Estás en una llamada.");
            Auto_SendPlayerAction(playerid, "mira su teléfono.");
            ShowDialog(playerid, DIALOG_PHONE);
        }
        case INVENTORY_PHONE_RESOLVER: {
            if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK || PI[playerid][pi_STATE] == ROLEPLAY_STATE_JAIL || PI[playerid][pi_STATE] == ROLEPLAY_STATE_ARRESTED) return Y_HOOKS_BREAK_RETURN_0;
            HidePlayerInventory(playerid);
            Dialog_Show(playerid, Dialog:PlayerPhoneBookFind);
        }
        case INVENTORY_GAS_CANISTER: {
            if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return ShowPlayerMessage(playerid, "Debes estar de pie para poder repostar combustible desde el bidon.");

            HidePlayerInventory(playerid);
            Dialog_Show(playerid, Dialog:GasCanisterSelectVehicle);
        }
        case INVENTORY_BRASSKNUCKLE .. INVENTORY_CAMERA: {
            if (p_inventory[playerid][slot][p_inventory_data] == 0) {
                new 
                    weapon_slot = WEAPON_INFO[ g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_extra] ][weapon_info_SLOT], 
                    any_weapon_in_same_slot = IfInventoryDataTrueWhereType(playerid, p_inventory[playerid][slot][p_inventory_type]);
                
                if (any_weapon_in_same_slot != -1) {
                    if (WEAPON_INFO[ g_inventory_data[ p_inventory[playerid][ any_weapon_in_same_slot ][p_inventory_type] ][g_info_extra] ][weapon_info_SLOT] == weapon_slot) {
                        ShowPlayerMessage(playerid, "Para equipar esta arma debes desequipar %s.", g_inventory_data[ p_inventory[playerid][ any_weapon_in_same_slot ][p_inventory_type] ][g_info_name]);
                        HidePlayerInventory(playerid);
                        return 1;
                    }
                }

                p_inventory[playerid][slot][p_inventory_data] = 1;
                Auto_SendPlayerAction(playerid, "equipa su %s.", g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_name]);
                UpdateSetPlayerEquippedWeapons(playerid);
                SavePlayerInventorySlot(playerid, slot);
            } else {
                p_inventory[playerid][slot][p_inventory_data] = 0;
                Auto_SendPlayerAction(playerid, "guarda su %s.", g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_name]);
                UpdateSetPlayerEquippedWeapons(playerid);
                SavePlayerInventorySlot(playerid, slot);
            }
            HidePlayerInventory(playerid);
        }
        case INVENTORY_MECHANIC_PIECES .. INVENTORY_CRACK: { //Sistema juntar global
            if (p_inventory[playerid][slot][p_inventory_amount] <= 0) { ShowPlayerMessage(playerid, "No tienes nada para juntar."); HidePlayerInventory(playerid); return 1; }

            new amount_to_add = g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_extra] - p_inventory[playerid][slot][p_inventory_amount];
            if (amount_to_add <= 0) { ShowPlayerMessage(playerid, "Ya tienes el máximo de %s en este espacio.", g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_name]); HidePlayerInventory(playerid); return 1; }
            
            for (new i = 0; i != MAX_PLAYER_SLOTS; i++) {
                if (i == slot || !p_inventory[playerid][i][p_inventory_valid]) continue;
                if (p_inventory[playerid][i][p_inventory_type] == p_inventory[playerid][slot][p_inventory_type]) {
                    new transfer = amount_to_add;
                    if (p_inventory[playerid][i][p_inventory_amount] < amount_to_add) {
                        transfer = p_inventory[playerid][i][p_inventory_amount];
                    }
                    
                    p_inventory[playerid][slot][p_inventory_amount] += transfer;
                    p_inventory[playerid][i][p_inventory_amount] -= transfer;
                    amount_to_add -= transfer;
                    
                    if (p_inventory[playerid][i][p_inventory_amount] <= 0) {
                        RemovePlayerInventorySlot(playerid, i);
                    } else {
                        SavePlayerInventorySlot(playerid, i);
                    }
                    
                    if (amount_to_add <= g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_extra] || p_inventory[playerid][slot][p_inventory_amount] >= g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_extra]) break;
                }
            }
            SavePlayerInventorySlot(playerid, slot);
            
            Auto_SendPlayerAction(playerid, "junta %s.", g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_name]);
            ShowPlayerMessage(playerid, "Has juntado %s en un solo espacio.", g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_name]);
            HidePlayerInventory(playerid);
        }
    }

    i_CurrentSlot[playerid] = -1;
    return 1;
}

TransferPlayerInventorySlot(const playerid, const to_playerid, slot, to_slot) {
    new const ToPlayerDBID = PI[to_playerid][pi_ID]; //Anti Dupe
    if (!ToPlayerDBID) return 0;

    mysql_format(
        handle_db, QUERY_BUFFER, sizeof(QUERY_BUFFER), "UPDATE player_inventory SET slot = %d, id_player = %d WHERE id = %d;", 
        to_slot, ToPlayerDBID, p_inventory[playerid][slot][p_inventory_id]
    );
    mysql_tquery(handle_db, QUERY_BUFFER);

    p_inventory[to_playerid][to_slot][p_inventory_valid] = true;
    p_inventory[playerid][slot][p_inventory_valid] = false;
    p_inventory[to_playerid][to_slot][p_inventory_id] = p_inventory[playerid][slot][p_inventory_id];
    p_inventory[playerid][slot][p_inventory_id] = 0;
    p_inventory[to_playerid][to_slot][p_inventory_amount] = p_inventory[playerid][slot][p_inventory_amount];
    p_inventory[playerid][slot][p_inventory_amount] = 0;
    p_inventory[to_playerid][to_slot][p_inventory_type] = p_inventory[playerid][slot][p_inventory_type];
    p_inventory[playerid][slot][p_inventory_type] = 0;
    p_inventory[to_playerid][to_slot][p_inventory_data] = p_inventory[playerid][slot][p_inventory_data];
    p_inventory[playerid][slot][p_inventory_data] = 0;
    p_inventory[to_playerid][to_slot][p_inventory_extra] = p_inventory[playerid][slot][p_inventory_extra];
    p_inventory[playerid][slot][p_inventory_extra] = 0;
    return 1;
}

ChangePlayerInventorySlotToSlot(const playerid, const from_slot, const to_slot) {
    mysql_format(
        handle_db, QUERY_BUFFER, sizeof(QUERY_BUFFER), "UPDATE player_inventory SET slot = %d WHERE id = %d;", 
        to_slot, p_inventory[playerid][from_slot][p_inventory_id]
    );
    mysql_tquery(handle_db, QUERY_BUFFER);

    p_inventory[playerid][to_slot][p_inventory_valid] = true;
    p_inventory[playerid][from_slot][p_inventory_valid] = false;
    p_inventory[playerid][to_slot][p_inventory_id] = p_inventory[playerid][from_slot][p_inventory_id];
    p_inventory[playerid][from_slot][p_inventory_id] = 0;
    p_inventory[playerid][to_slot][p_inventory_amount] = p_inventory[playerid][from_slot][p_inventory_amount];
    p_inventory[playerid][from_slot][p_inventory_amount] = 0;
    p_inventory[playerid][to_slot][p_inventory_type] = p_inventory[playerid][from_slot][p_inventory_type];
    p_inventory[playerid][from_slot][p_inventory_type] = 0;
    p_inventory[playerid][to_slot][p_inventory_data] = p_inventory[playerid][from_slot][p_inventory_data];
    p_inventory[playerid][from_slot][p_inventory_data] = 0;
    p_inventory[playerid][to_slot][p_inventory_extra] = p_inventory[playerid][from_slot][p_inventory_extra];
    p_inventory[playerid][from_slot][p_inventory_extra] = 0;
    return 1;
}

SetPlayerInventorySlotData(const playerid, const slot, const type, const amount, const data, extra = 0) {
    p_inventory[playerid][slot][p_inventory_type] = type;
    p_inventory[playerid][slot][p_inventory_amount] = amount;
    p_inventory[playerid][slot][p_inventory_data] = data;
    p_inventory[playerid][slot][p_inventory_extra] = extra;
    return 1;
}

InsertPlayerInventorySlot(const playerid, const slot) {
    inline OnInventoryQueryInsert() {
        p_inventory[playerid][slot][p_inventory_valid] = true;
        p_inventory[playerid][slot][p_inventory_id] = cache_insert_id();

        ShowPlayerNotificationModel(
            playerid, g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_model], 
            g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_rot_x], 
            g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_rot_y], 
            g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_rot_z], 
            "~n~~g~+%d~w~~h~ %s~n~", 
                                    p_inventory[playerid][slot][p_inventory_amount], 
                                    g_inventory_data[ p_inventory[playerid][slot][p_inventory_type] ][g_info_name]
        );
    }
    MySQL_TQueryInline(
        handle_db, using inline OnInventoryQueryInsert, "INSERT INTO player_inventory (id_player, slot, type, amount, data, extra) VALUES (%d, %d, %d, %d, %d, %d);", 
        PI[playerid][pi_ID], slot, p_inventory[playerid][slot][p_inventory_type], p_inventory[playerid][slot][p_inventory_amount], p_inventory[playerid][slot][p_inventory_data], p_inventory[playerid][slot][p_inventory_extra]
    );
    return 1;
}

SavePlayerInventorySlot(const playerid, const slot) {
    if (!p_inventory[playerid][slot][p_inventory_valid]) return 0;
    mysql_format(
        handle_db, QUERY_BUFFER, sizeof(QUERY_BUFFER), "UPDATE player_inventory SET type = %d, amount = %d, data = %d, extra = %d WHERE id = %d;", 
        p_inventory[playerid][slot][p_inventory_type], p_inventory[playerid][slot][p_inventory_amount], p_inventory[playerid][slot][p_inventory_data], 
        p_inventory[playerid][slot][p_inventory_extra], p_inventory[playerid][slot][p_inventory_id]
    );
    mysql_tquery(handle_db, QUERY_BUFFER);
    return 1;
}

LoadPlayerInventory(const playerid) {
    inline OnInventoryQueryLoad() {
        new rows;
        if (cache_get_row_count(rows)) {
            if (rows) {
                for (new i = 0; i != rows; i ++) {
                    new slot;
                    cache_get_value_name_int(i, "slot", slot);
                    p_inventory[playerid][slot][p_inventory_valid] = true;
                    cache_get_value_name_int(i, "id", p_inventory[playerid][slot][p_inventory_id]);
                    cache_get_value_name_int(i, "type", p_inventory[playerid][slot][p_inventory_type]);
                    cache_get_value_name_int(i, "amount", p_inventory[playerid][slot][p_inventory_amount]);
                    cache_get_value_name_int(i, "data", p_inventory[playerid][slot][p_inventory_data]);
                    cache_get_value_name_int(i, "extra", p_inventory[playerid][slot][p_inventory_extra]);
                }
            }
        }
    }
    MySQL_TQueryInline(handle_db, using inline OnInventoryQueryLoad, "SELECT * FROM player_inventory WHERE id_player = %d;", PI[playerid][pi_ID]);
    return 1;
}

UpdateSetPlayerEquippedWeapons(const playerid) { //Decidir cuando el arma podra ser usable
    ResetPlayerWeapons(playerid);

    if (PLAYER_TEMP[playerid][pt_CUFFED]) return 0;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_ARRESTED) return 0;
    if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_JAIL) return 0;
    if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_CRACK) return 0;
    if (IsPlayerActiveStaticPos(playerid)) return 0;

    PLAYER_AC_INFO[playerid][CHEAT_WEAPON][p_ac_info_IMMUNITY] = gettime() + 3;
    PLAYER_AC_INFO[playerid][CHEAT_AMMO][p_ac_info_IMMUNITY] = gettime() + 3;

    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) continue;

        if (IsInventoryTypeWeapon(p_inventory[playerid][i][p_inventory_type]) && p_inventory[playerid][i][p_inventory_amount] > 0 && p_inventory[playerid][i][p_inventory_data] == 1) {
            GivePlayerWeapon(playerid, g_inventory_data[ p_inventory[playerid][i][p_inventory_type] ][g_info_extra], p_inventory[playerid][i][p_inventory_amount]);
        }
    }
    return 1;
}

stock IfInventoryDataTrueWhereType(const playerid, const type) {
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) continue;
        if (
            p_inventory[playerid][i][p_inventory_type] == type &&
            p_inventory[playerid][i][p_inventory_data] == 1
        ) {
            return i;
        }
    }
    return -1;
}

stock CheckIfDataTrueFromInfoExtra(const playerid, const extra) {
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) continue;
        if (
            g_inventory_data[ p_inventory[playerid][i][p_inventory_type] ][g_info_extra] == extra &&
            p_inventory[playerid][i][p_inventory_data] == 1
        ) {
            return i;
        }
    }
    return -1;
}

stock GetPlayerMechanicPieces(const playerid) {
    new pieces = 0;
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) continue;
        if (p_inventory[playerid][i][p_inventory_type] == INVENTORY_MECHANIC_PIECES) {
            pieces += p_inventory[playerid][i][p_inventory_amount];
        }
    }
    return pieces;
}

stock AddPlayerMechanicPieces(const playerid, amount) {
    if (amount > 100) { //Que no pase de 100 piezas
        new pieces = GetPlayerMechanicPieces(playerid);
        new total = pieces + amount;
        if (total > 100) {
            amount = 100 - pieces;
        }
    }

    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) continue;
        if (p_inventory[playerid][i][p_inventory_type] == INVENTORY_MECHANIC_PIECES && p_inventory[playerid][i][p_inventory_amount] <= 100) {
            p_inventory[playerid][i][p_inventory_amount] += amount;
            SavePlayerInventorySlot(playerid, i);
            return 1;
        }
    }
    new slot = GetPlayerInventorySlot(playerid);
    if (slot == -1) return 0;

    SetPlayerInventorySlotData(playerid, slot, INVENTORY_MECHANIC_PIECES, amount, 0, 0);
    InsertPlayerInventorySlot(playerid, slot);
    return 1;
}

stock RemovePlayerMechanicPieces(const playerid, amount) {
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) continue;
        if (p_inventory[playerid][i][p_inventory_type] == INVENTORY_MECHANIC_PIECES) {
            if (p_inventory[playerid][i][p_inventory_amount] >= amount) {
                p_inventory[playerid][i][p_inventory_amount] -= amount;
                SavePlayerInventorySlot(playerid, i);
            } else {
                amount -= p_inventory[playerid][i][p_inventory_amount];
                RemovePlayerInventorySlot(playerid, i);
            }
        }
    }
    return 1;
}

stock DropEquippedCurrentWeapon(const playerid) {
    for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
        if (!p_inventory[playerid][i][p_inventory_valid]) continue;

        if (IsInventoryTypeWeapon(p_inventory[playerid][i][p_inventory_type]) && p_inventory[playerid][i][p_inventory_amount] > 0 && p_inventory[playerid][i][p_inventory_data] == 1) {
            if (g_inventory_data[ p_inventory[playerid][i][p_inventory_type] ][g_info_extra] == GetPlayerWeapon(playerid)) {
                DropPlayerInventorySlot(playerid, i);
                break;
            }
        }
    }
    return 0;
}

DialogCreate:PlayerGiveInvSlot(playerid) {
    new caption[124], dialog_body[2000] = "{f0f0f0}Introduce el {"#SECONDARY_COLOR"}ID de el jugador{f0f0f0} al que quieres darle el objeto\n\n{cccccc}Jugadores Cercanos:\n", counter = 0;
    format(caption, sizeof caption, "{"#PRIMARY_COLOR"}%s{f0f0f0} » Dar Objeto", g_inventory_data[ p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] ][g_info_name]);
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
    Dialog_Open(playerid, Dialog:PlayerGiveInvSlot, DIALOG_STYLE_INPUT, caption, dialog_body, "Dar", "Cancelar");
    return 1;
}

DialogCreate:PlayerSellInvSlot(playerid) {
    new caption[124], dialog_body[2000] = "{f0f0f0}Introduce el {"#SECONDARY_COLOR"}ID de el jugador{f0f0f0} al que quieres venderle el objeto\n\n{cccccc}Jugadores Cercanos:\n", counter = 0;
    format(caption, sizeof caption, "{"#PRIMARY_COLOR"}%s{f0f0f0} » Vender Objeto", g_inventory_data[ p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] ][g_info_name]);
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
    Dialog_Open(playerid, Dialog:PlayerSellInvSlot, DIALOG_STYLE_INPUT, caption, dialog_body, "Continuar", "Cancelar");
    return 1;
}

DialogCreate:PlayerSellPriceInvSlot(playerid) {
    new caption[124];
    format(caption, sizeof caption, "{"#PRIMARY_COLOR"}%s{f0f0f0} » Precio", g_inventory_data[ p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] ][g_info_name]);
    Dialog_Open(playerid, Dialog:PlayerSellPriceInvSlot, DIALOG_STYLE_INPUT, caption, "{f0f0f0}Introduce el precio de el objeto que quieres vender:", "Continuar", "Cancelar");
    return 1;
}

hook OnPlayerClickPlayerTD(playerid, PlayerText:playertextid) {
    if (i_PlayerInventoryShow[playerid]) {
        if (gettime() < p_inventory_click_timer[playerid]) {
            return Y_HOOKS_BREAK_RETURN_1;
        }
        /*if (playertextid == p_InventoryTd[playerid][15]) { //SKIN
            return Y_HOOKS_BREAK_RETURN_1;
        }*/

        if (playertextid == p_InventoryTd[playerid][23]) { //USAR, ETC
            if (i_CurrentSlot[playerid] != -1) {
                if (p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] == INVENTORY_PHONE) {
                    p_inventory_click_timer[playerid] = gettime() + 1;
                    
                    if (inventory_slot_info[ i_CurrentSlot[playerid] ][i_specific_slot_type] == INVENTORY_PHONE) {
                        UsePlayerInventorySlot(playerid, i_CurrentSlot[playerid]);
                    } else {
                        new phone_slot = GetSpecificSlotIndex(INVENTORY_PHONE);
                        if(p_inventory[playerid][phone_slot][p_inventory_valid]) return ShowPlayerMessage(playerid, "Ya tienes un teléfono equipado.");

                        ChangePlayerInventorySlotToSlot(playerid, i_CurrentSlot[playerid], phone_slot);
                        Auto_SendPlayerAction(playerid, "coloca su telefono en el bolsillo.");
                        i_CurrentSlot[playerid] = phone_slot;
                        ShowPlayerInventory(playerid);
                    }
                    return Y_HOOKS_BREAK_RETURN_1;
                }

                if (p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] == INVENTORY_PHONE_RESOLVER) {
                    p_inventory_click_timer[playerid] = gettime() + 1;

                    if (inventory_slot_info[ i_CurrentSlot[playerid] ][i_specific_slot_type] == INVENTORY_PHONE_RESOLVER) {
                        UsePlayerInventorySlot(playerid, i_CurrentSlot[playerid]);
                    } else {
                        new guide_slot = GetSpecificSlotIndex(INVENTORY_PHONE_RESOLVER);
                        if(p_inventory[playerid][guide_slot][p_inventory_valid]) return ShowPlayerMessage(playerid, "Ya tienes una guia telefonica equipada.");

                        ChangePlayerInventorySlotToSlot(playerid, i_CurrentSlot[playerid], guide_slot);
                        Auto_SendPlayerAction(playerid, "coloca su guia telefonica en el bolsillo.");
                        i_CurrentSlot[playerid] = guide_slot;
                        ShowPlayerInventory(playerid);
                    }
                    return Y_HOOKS_BREAK_RETURN_1;
                }

                UsePlayerInventorySlot(playerid, i_CurrentSlot[playerid]);
                return Y_HOOKS_BREAK_RETURN_1;
            }
        }

        if (playertextid == p_InventoryTd[playerid][24]) { //TIRAR o GUARDAR
            if (i_CurrentSlot[playerid] != -1) {
                if (IsPlayerWorking(playerid, WORK_POLICE)) {
                    if (IsInventoryTypeWeapon(p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type])) {
                        ShowPlayerMessage(playerid, "~y~Advertencia~n~~w~~h~No puedes hacer esto con tus armas mientras trabajas como policía.");
                        HidePlayerInventory(playerid);
                        return Y_HOOKS_BREAK_RETURN_1;
                    }
                }

                Auto_SendPlayerAction(playerid, "tiro %c%s al suelo.", tolower(g_inventory_data[ p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] ][g_info_name][0]), g_inventory_data[ p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type] ][g_info_name][1]);
                ApplyAnimation(playerid, "CARRY", "putdwn05", 4.1, 0, 1, 1, 0, 0, 1);
                DropPlayerInventorySlot(playerid, i_CurrentSlot[playerid]);
                HidePlayerInventory(playerid);
                i_CurrentSlot[playerid] = -1;
                return Y_HOOKS_BREAK_RETURN_1;
            }
        }

        if (playertextid == p_InventoryTd[playerid][25]) { //DAR
            if (i_CurrentSlot[playerid] != -1) {
                if (IsPlayerWorking(playerid, WORK_POLICE)) {
                    if (IsInventoryTypeWeapon(p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type])) {
                        ShowPlayerMessage(playerid, "~y~Advertencia~n~~w~~h~No puedes hacer esto con tus armas mientras trabajas como policía.");
                        HidePlayerInventory(playerid);
                        return Y_HOOKS_BREAK_RETURN_1;
                    }
                }

                Dialog_Show(playerid, Dialog:PlayerGiveInvSlot);
                HidePlayerInventory(playerid);
                return Y_HOOKS_BREAK_RETURN_1;
            }
        }

        if (playertextid == p_InventoryTd[playerid][26]) { //VENDER
            if (i_CurrentSlot[playerid] != -1) {
                if (IsPlayerWorking(playerid, WORK_POLICE)) {
                    if (IsInventoryTypeWeapon(p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_type])) {
                        ShowPlayerMessage(playerid, "~y~Advertencia~n~~w~~h~No puedes hacer esto con tus armas mientras trabajas como policía.");
                        HidePlayerInventory(playerid);
                        return Y_HOOKS_BREAK_RETURN_1;
                    }
                }

                Dialog_Show(playerid, Dialog:PlayerSellInvSlot);
                HidePlayerInventory(playerid);
                return Y_HOOKS_BREAK_RETURN_1;
            }
        }

        new slot = -1;
        if (playertextid == p_InventoryTd[playerid][2])  slot = 0;
        if (playertextid == p_InventoryTd[playerid][3])  slot = 1;
        if (playertextid == p_InventoryTd[playerid][4])  slot = 2;
        if (playertextid == p_InventoryTd[playerid][5])  slot = 3;
        if (playertextid == p_InventoryTd[playerid][6])  slot = 4;
        if (playertextid == p_InventoryTd[playerid][7])  slot = 5;
        if (playertextid == p_InventoryTd[playerid][8])  slot = 6;
        if (playertextid == p_InventoryTd[playerid][9])  slot = 7;
        if (playertextid == p_InventoryTd[playerid][10]) slot = 8;
        if (playertextid == p_InventoryTd[playerid][11]) slot = 9;
        if (playertextid == p_InventoryTd[playerid][12]) slot = 10;
        if (playertextid == p_InventoryTd[playerid][13]) slot = 11;
        if (playertextid == p_InventoryTd[playerid][27]) slot = 12;
        if (playertextid == p_InventoryTd[playerid][28]) slot = 13;
        if (playertextid == p_InventoryTd[playerid][29]) slot = 14;
        if (playertextid == p_InventoryTd[playerid][30]) slot = 15;
        i_CurrentSlot[playerid] = slot;
        if (i_CurrentSlot[playerid] != -1) {
            if (p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_valid] != true) {
                i_CurrentSlot[playerid] = -1;
            }
        }

        ShowPlayerInventory(playerid);
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

DialogResponse:PlayerGiveInvSlot(playerid, response, listitem, inputtext[]) {
    if (!p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_valid]) return 0;

    if (response) {
        if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_JAIL || PI[playerid][pi_STATE] == ROLEPLAY_STATE_ARRESTED) return 0;
        if (PI[playerid][pi_LEVEL] < 15) return ShowPlayerMessage(playerid, "Necesitas ser nivel 15 para poder hacer esto.");
        
        new to_playerid;
        if (sscanf(inputtext, "u", to_playerid)) return ShowPlayerMessage(playerid, "Error en los parámetros.");
        if (to_playerid == playerid) return ShowPlayerMessage(playerid, "No puedes darte objetos a ti mismo.");
        if (!IsPlayerConnected(to_playerid)) return ShowPlayerMessage(playerid, "El jugador no está conectado.");

        new Float:pos[3]; GetPlayerPos(to_playerid, pos[0], pos[1], pos[2]);
        if (!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return ShowPlayerMessage(playerid, "Este jugador no está cerca tuya.");
        if (PLAYER_TEMP[to_playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return ShowPlayerMessage(playerid, "No puedes darle nada a este jugador ahora.");

        new to_slot = GetPlayerInventorySlot(to_playerid);
        if (to_slot == -1) {
            PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
            ShowPlayerMessage(playerid, "El jugador no tiene espacio en el inventario.");
        } else {
            TransferPlayerInventorySlot(playerid, to_playerid, i_CurrentSlot[playerid], to_slot);
            Auto_SendPlayerAction(playerid, "le da algo a %s.", PLAYER_TEMP[to_playerid][pt_NAME]);
        }
    }
    return 1;
}

DialogResponse:PlayerSellInvSlot(playerid, response, listitem, inputtext[]) {
    if (!p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_valid]) return 0;

    if (response) {
        if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_JAIL || PI[playerid][pi_STATE] == ROLEPLAY_STATE_ARRESTED) return 0;
        if (PI[playerid][pi_LEVEL] < 15) return ShowPlayerMessage(playerid, "Necesitas ser nivel 15 para poder hacer esto.");
        
        new to_playerid;
        if (sscanf(inputtext, "u", to_playerid)) return ShowPlayerMessage(playerid, "Error en los parámetros.");
        if (to_playerid == playerid) return ShowPlayerMessage(playerid, "No puedes venderte objetos a ti mismo.");
        if (!IsPlayerConnected(to_playerid)) return ShowPlayerMessage(playerid, "El jugador no está conectado.");

        new Float:pos[3]; GetPlayerPos(to_playerid, pos[0], pos[1], pos[2]);
        if (!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return ShowPlayerMessage(playerid, "Este jugador no está cerca tuya.");
        if (PLAYER_TEMP[to_playerid][pt_GAME_STATE] != GAME_STATE_NORMAL) return ShowPlayerMessage(playerid, "No puedes darle nada a este jugador ahora.");
        
        PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] = to_playerid;
        PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID] = PI[to_playerid][pi_ID];

        Dialog_Show(playerid, Dialog:PlayerSellPriceInvSlot);
    }
    return 1;
}

DialogResponse:PlayerSellPriceInvSlot(playerid, response, listitem, inputtext[]) {
    if (!p_inventory[playerid][ i_CurrentSlot[playerid] ][p_inventory_valid]) return 0;

    if (response) {
        if (PI[playerid][pi_STATE] == ROLEPLAY_STATE_JAIL || PI[playerid][pi_STATE] == ROLEPLAY_STATE_ARRESTED) return 0;
        if (PI[playerid][pi_LEVEL] < 15) return ShowPlayerMessage(playerid, "Necesitas ser nivel 15 para poder hacer esto.");
        
        new price, to_playerid = PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID];
        if (sscanf(inputtext, "d", price)) return ShowPlayerMessage(playerid, "Error en los parámetros.");
        if (price < 0 || price > 10000000) return ShowPlayerMessage(playerid, "Error en los parámetros.");

        if (!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return ShowPlayerMessage(playerid, "El jugador no está conectado.");
        if (PI[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pi_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return ShowPlayerMessage(playerid, "El jugador no está conectado.");

        new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], pos[0], pos[1], pos[2]);
        if (!IsPlayerInRangeOfPoint(playerid, NEARS_PLAYERS_DISTANCE, pos[0], pos[1], pos[2])) return ShowPlayerMessage(playerid, "Este jugador no está cerca tuya.");
        if (PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return ShowPlayerMessage(playerid, "No puedes darle nada a este jugador ahora.");
        
        PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_PID] = playerid;
        PLAYER_TEMP[to_playerid][pt_TRICK_SELLER_AID] = PI[playerid][pi_ID];
        PLAYER_TEMP[to_playerid][pt_TRICK_PRICE] = price;
        PLAYER_TEMP[to_playerid][pt_TRICK_TIME] = gettime();
        PLAYER_TEMP[to_playerid][pt_TRICK_SLOT] = i_CurrentSlot[playerid];

        new dialog[200], caption[100];
        format(dialog, sizeof dialog, "{f0f0f0}%s te quiere vender algo.\n\nObjeto: %s\nExtra: %d\nPrecio: %s{"#GREEN_COLOR"}${f0f0f0}\n\n¿Quieres comprárselo?",
            PLAYER_TEMP[playerid][pt_NAME],
            g_inventory_data[ p_inventory[playerid][ PLAYER_TEMP[to_playerid][pt_TRICK_SLOT] ][p_inventory_type] ][g_info_name],
            p_inventory[playerid][ PLAYER_TEMP[to_playerid][pt_TRICK_SLOT] ][p_inventory_amount],
            number_format_thousand(PLAYER_TEMP[to_playerid][pt_TRICK_PRICE])
        );

        format(caption, sizeof caption, "{"#PRIMARY_COLOR"}%s{f0f0f0} » Comprar Objeto", g_inventory_data[ p_inventory[playerid][ PLAYER_TEMP[to_playerid][pt_TRICK_SLOT] ][p_inventory_type] ][g_info_name]);        
        Dialog_Open(to_playerid, Dialog:PlayerTrickObject, DIALOG_STYLE_MSGBOX, caption, dialog, "Aceptar", "Cancelar");

        ShowPlayerMessage(playerid, "Le has ofrecido una venta a %s, espera a ver si la acepta.", PLAYER_TEMP[to_playerid][pt_NAME]);	
    }
    return 1;
}

DialogResponse:PlayerTrickObject(playerid, response, listitem, inputtext[]) {
    if (response) {
        if (gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return ShowPlayerMessage(playerid, "Has tardardo mucho en aceptarlo.");
        if (!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return ShowPlayerMessage(playerid, "El vendedor ya no está en el servidor.");
        if (PI[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pi_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return ShowPlayerMessage(playerid, "El vendedor ya no está en el servidor.");

        new Float:pos[3]; GetPlayerPos(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], pos[0], pos[1], pos[2]);
        if (!IsPlayerInRangeOfPoint(playerid, 2.0, pos[0], pos[1], pos[2])) return ShowPlayerMessage(playerid, "El vendedor está demasiado lejos.");
        if (PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pt_GAME_STATE] != GAME_STATE_NORMAL) return ShowPlayerMessage(playerid, "El vendedor no está disponible.");

        new to_slot = GetPlayerInventorySlot(playerid);
        if (to_slot == -1) {
            PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
            ShowPlayerMessage(playerid, "No tienes espacio para aceptar esto.");
            return 1;
        }

        if (GivePlayerCash(playerid, -PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, true) && GivePlayerCash(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], PLAYER_TEMP[playerid][pt_TRICK_PRICE], true, false)) {
            TransferPlayerInventorySlot(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], playerid, i_CurrentSlot[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ], to_slot);

            ShowPlayerMessage(playerid, "Has gastado %s~g~$~w~~h~ con esta compra.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));
            ShowPlayerMessage(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "Has ganado %s~g~$~w~~h~ con esta venta.", number_format_thousand(PLAYER_TEMP[playerid][pt_TRICK_PRICE]));

            Auto_SendPlayerAction(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "y %s llegan a un acuerdo.", PLAYER_TEMP[playerid][pt_NAME]);

            HidePlayerInventory(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID]);
            HidePlayerInventory(playerid);

            UpdateSetPlayerEquippedWeapons(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID]);
            UpdateSetPlayerEquippedWeapons(playerid);
        }
    } else {
        if (gettime() > PLAYER_TEMP[playerid][pt_TRICK_TIME] + 20) return 1;
        if (!IsPlayerConnected(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID])) return 1;
        if (PI[ PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID] ][pi_ID] != PLAYER_TEMP[playerid][pt_TRICK_SELLER_AID]) return 1;

        ShowPlayerMessage(PLAYER_TEMP[playerid][pt_TRICK_SELLER_PID], "El comprador no ha aceptado tu trato.");
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (PLAYER_TEMP[playerid][pt_GAME_STATE] == GAME_STATE_NORMAL && PlayerMissionStep(playerid) != MISSION_STEP_CINEMATIC && !i_PlayerInventoryShow[playerid]) {
        if (newkeys & KEY_NO) {
            ShowPlayerInventory(playerid);
        }
        if(newkeys & KEY_CTRL_BACK) {
            if(PI[playerid][pi_STATE] != ROLEPLAY_STATE_CRACK && PI[playerid][pi_STATE] != ROLEPLAY_STATE_JAIL) {
                for(new i = 0; i < list_size(DroppedInventoryObjects); i ++) {
                    new Dropped_inventory_data[E_DroppedInventoryObjects];
                    list_get_arr(DroppedInventoryObjects, i, Dropped_inventory_data);

                    if(GetPlayerInterior(playerid) == Dropped_inventory_data[drp_Interior] && GetPlayerVirtualWorld(playerid) == Dropped_inventory_data[drp_World] && IsPlayerInRangeOfPoint(playerid, 1.0, Dropped_inventory_data[drp_X], Dropped_inventory_data[drp_Y], Dropped_inventory_data[drp_Z])) {
                        if(PI[playerid][pi_LEVEL] < 10) SendPlayerClientMessage(playerid, "{cccccc}Deber ser nivel 10 o superior para poder recoger objetos de el suelo.");
                        else {
                            new slot = GetPlayerInventorySlot(playerid);
                            if (slot == -1) {
                                PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                                ShowPlayerMessage(playerid, "No tienes más espacio en tu inventario.");
                            } else {
                                Auto_SendPlayerAction(playerid, "recoge %s del suelo.", g_inventory_data[  Dropped_inventory_data[drp_Type] ][g_info_name]);
                                ApplyAnimation(playerid, "CARRY", "putdwn05", 4.1, 0, 1, 1, 0, 0, 1);

                                if (IsInventoryTypeWeapon(Dropped_inventory_data[drp_Type])) {
                                    Dropped_inventory_data[drp_Data] = 0;
                                }

                                SetPlayerInventorySlotData(playerid, slot, Dropped_inventory_data[drp_Type], Dropped_inventory_data[drp_Amount], Dropped_inventory_data[drp_Data], Dropped_inventory_data[drp_Extra]);
                                InsertPlayerInventorySlot(playerid, slot);
                                UpdateSetPlayerEquippedWeapons(playerid);

                                if (IsValidDynamicObject(Dropped_inventory_data[drp_ObjectId])) DestroyDynamicObject(Dropped_inventory_data[drp_ObjectId]);
                                DestroyDynamic3DTextLabel(Dropped_inventory_data[drp_LabelId]);

                                list_remove(DroppedInventoryObjects, i);
                                Streamer_Update(playerid);
                            }
                        }
                        return Y_HOOKS_BREAK_RETURN_1;
                    }
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnScriptInit() {
    DroppedInventoryObjects = list_new();
}

hook OnPlayerConnect(playerid) {
    i_CurrentSlot[playerid] = -1;
    i_PlayerInventoryShow[playerid] = false;

    p_InventoryTd[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 110.000, "bg");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][0], 0.000, 27.000);
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][0], 0.000, 380.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][0], 2);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][0], 255);
    PlayerTextDrawUseBox(playerid, p_InventoryTd[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, p_InventoryTd[playerid][0], 1784335871);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][0], 255);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][0], 1);

    p_InventoryTd[playerid][1] = CreatePlayerTextDraw(playerid, 320.000, 112.000, "bg");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][1], 0.000, 26.599);
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][1], 0.000, 380.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][1], 2);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][1], 255);
    PlayerTextDrawUseBox(playerid, p_InventoryTd[playerid][1], 1);
    PlayerTextDrawBoxColor(playerid, p_InventoryTd[playerid][1], 84215290);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][1], 255);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][1], 1);

    p_InventoryTd[playerid][2] = CreatePlayerTextDraw(playerid, 452.000, 120.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][2], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][2], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][2], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][2], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][2], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][2], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][2], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][2], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][2], 1);

    p_InventoryTd[playerid][3] = CreatePlayerTextDraw(playerid, 452.000, 185.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][3], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][3], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][3], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][3], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][3], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][3], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][3], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][3], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][3], 1);

    p_InventoryTd[playerid][4] = CreatePlayerTextDraw(playerid, 452.000, 250.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][4], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][4], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][4], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][4], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][4], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][4], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][4], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][4], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][4], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][4], 1);

    p_InventoryTd[playerid][5] = CreatePlayerTextDraw(playerid, 393.000, 120.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][5], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][5], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][5], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][5], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][5], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][5], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][5], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][5], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][5], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][5], 1);

    p_InventoryTd[playerid][6] = CreatePlayerTextDraw(playerid, 393.000, 185.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][6], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][6], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][6], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][6], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][6], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][6], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][6], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][6], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][6], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][6], 1);

    p_InventoryTd[playerid][7] = CreatePlayerTextDraw(playerid, 393.000, 250.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][7], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][7], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][7], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][7], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][7], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][7], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][7], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][7], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][7], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][7], 1);

    p_InventoryTd[playerid][8] = CreatePlayerTextDraw(playerid, 334.000, 120.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][8], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][8], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][8], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][8], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][8], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][8], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][8], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][8], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][8], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][8], 1);

    p_InventoryTd[playerid][9] = CreatePlayerTextDraw(playerid, 334.000, 185.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][9], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][9], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][9], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][9], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][9], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][9], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][9], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][9], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][9], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][9], 1);

    p_InventoryTd[playerid][10] = CreatePlayerTextDraw(playerid, 334.000, 250.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][10], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][10], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][10], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][10], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][10], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][10], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][10], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][10], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][10], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][10], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][10], 1);

    p_InventoryTd[playerid][11] = CreatePlayerTextDraw(playerid, 275.000, 120.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][11], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][11], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][11], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][11], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][11], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][11], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][11], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][11], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][11], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][11], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][11], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][11], 1);

    p_InventoryTd[playerid][12] = CreatePlayerTextDraw(playerid, 275.000, 185.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][12], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][12], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][12], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][12], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][12], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][12], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][12], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][12], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][12], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][12], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][12], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][12], 1);

    p_InventoryTd[playerid][13] = CreatePlayerTextDraw(playerid, 275.000, 250.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][13], 55.000, 60.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][13], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][13], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][13], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][13], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][13], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][13], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][13], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][13], -1);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][13], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][13], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][13], 1);

    p_InventoryTd[playerid][14] = CreatePlayerTextDraw(playerid, 392.000, 312.000, "I");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][14], 35.000, 0.300);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][14], 2);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][14], 1784335871);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][14], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][14], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][14], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][14], 2);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][14], 1);

    p_InventoryTd[playerid][15] = CreatePlayerTextDraw(playerid, 181.000, 120.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][15], 90.000, 100.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][15], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][15], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][15], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][15], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][15], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][15], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][15], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][15], 3);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][15], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][15], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][15], 1);

    p_InventoryTd[playerid][16] = CreatePlayerTextDraw(playerid, 503.000, 317.000, "Nombre");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][16], 0.200, 1.279);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][16], 3);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][16], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][16], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][16], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][16], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][16], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][16], 1);

    p_InventoryTd[playerid][17] = CreatePlayerTextDraw(playerid, 502.000, 330.000, "Text");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][17], 0.188, 1.179);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][17], 3);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][17], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][17], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][17], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][17], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][17], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][17], 1);

    p_InventoryTd[playerid][18] = CreatePlayerTextDraw(playerid, 182.000, 222.000, "HUD:radar_girlfriend");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][18], 10.000, 10.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][18], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][18], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][18], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][18], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][18], 255);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][18], 4);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][18], 1);

    p_InventoryTd[playerid][19] = CreatePlayerTextDraw(playerid, 193.000, 223.000, "100");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][19], 0.200, 0.898);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][19], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][19], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][19], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][19], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][19], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][19], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][19], 1);

    p_InventoryTd[playerid][20] = CreatePlayerTextDraw(playerid, 260.000, 222.000, "HUD:radar_tshirt");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][20], 10.000, 11.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][20], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][20], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][20], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][20], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][20], 255);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][20], 4);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][20], 1);

    p_InventoryTd[playerid][21] = CreatePlayerTextDraw(playerid, 258.000, 223.000, "100");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][21], 0.200, 0.898);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][21], 3);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][21], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][21], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][21], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][21], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][21], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][21], 1);

    p_InventoryTd[playerid][22] = CreatePlayerTextDraw(playerid, 225.000, 235.000, "I");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][22], 13.708, 0.200);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][22], 2);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][22], 1784335871);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][22], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][22], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][22], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][22], 2);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][22], 1);

    p_InventoryTd[playerid][23] = CreatePlayerTextDraw(playerid, 226.000, 243.000, "USAR");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][23], 0.459, 2.199);
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][23], 10.000, 88.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][23], 2);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][23], 1768516095);
    PlayerTextDrawUseBox(playerid, p_InventoryTd[playerid][23], 1);
    PlayerTextDrawBoxColor(playerid, p_InventoryTd[playerid][23], 252645375);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][23], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][23], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][23], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][23], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][23], 1);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][23], 1);

    p_InventoryTd[playerid][24] = CreatePlayerTextDraw(playerid, 226.000, 270.000, "TIRAR");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][24], 0.459, 2.199);
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][24], 10.000, 88.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][24], 2);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][24], -1);
    PlayerTextDrawUseBox(playerid, p_InventoryTd[playerid][24], 1);
    PlayerTextDrawBoxColor(playerid, p_InventoryTd[playerid][24], 252645375);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][24], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][24], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][24], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][24], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][24], 1);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][24], 1);

    p_InventoryTd[playerid][25] = CreatePlayerTextDraw(playerid, 226.000, 296.000, "DAR");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][25], 0.459, 2.199);
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][25], 10.000, 88.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][25], 2);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][25], -1);
    PlayerTextDrawUseBox(playerid, p_InventoryTd[playerid][25], 1);
    PlayerTextDrawBoxColor(playerid, p_InventoryTd[playerid][25], 252645375);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][25], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][25], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][25], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][25], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][25], 1);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][25], 1);

    p_InventoryTd[playerid][26] = CreatePlayerTextDraw(playerid, 226.000, 322.000, "VENDER");
    PlayerTextDrawLetterSize(playerid, p_InventoryTd[playerid][26], 0.459, 2.199);
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][26], 10.000, 88.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][26], 2);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][26], -1);
    PlayerTextDrawUseBox(playerid, p_InventoryTd[playerid][26], 1);
    PlayerTextDrawBoxColor(playerid, p_InventoryTd[playerid][26], 252645375);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][26], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][26], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][26], 150);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][26], 1);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][26], 1);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][26], 1);

    p_InventoryTd[playerid][27] = CreatePlayerTextDraw(playerid, 132.000, 120.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][27], 46.000, 46.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][27], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][27], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][27], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][27], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][27], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][27], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][27], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][27], 19300);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][27], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][27], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][27], 1);

    p_InventoryTd[playerid][28] = CreatePlayerTextDraw(playerid, 132.000, 168.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][28], 46.000, 46.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][28], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][28], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][28], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][28], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][28], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][28], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][28], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][28], 19300);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][28], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][28], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][28], 1);

    p_InventoryTd[playerid][29] = CreatePlayerTextDraw(playerid, 132.000, 216.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][29], 46.000, 46.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][29], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][29], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][29], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][29], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][29], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][29], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][29], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][29], 19300);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][29], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][29], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][29], 1);

    p_InventoryTd[playerid][30] = CreatePlayerTextDraw(playerid, 132.000, 264.000, "_");
    PlayerTextDrawTextSize(playerid, p_InventoryTd[playerid][30], 46.000, 46.000);
    PlayerTextDrawAlignment(playerid, p_InventoryTd[playerid][30], 1);
    PlayerTextDrawColor(playerid, p_InventoryTd[playerid][30], -1);
    PlayerTextDrawSetShadow(playerid, p_InventoryTd[playerid][30], 0);
    PlayerTextDrawSetOutline(playerid, p_InventoryTd[playerid][30], 0);
    PlayerTextDrawBackgroundColor(playerid, p_InventoryTd[playerid][30], 252645375);
    PlayerTextDrawFont(playerid, p_InventoryTd[playerid][30], 5);
    PlayerTextDrawSetProportional(playerid, p_InventoryTd[playerid][30], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_InventoryTd[playerid][30], 19300);
    PlayerTextDrawSetPreviewRot(playerid, p_InventoryTd[playerid][30], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_InventoryTd[playerid][30], 0, 0);
    PlayerTextDrawSetSelectable(playerid, p_InventoryTd[playerid][30], 1);
    return Y_HOOKS_CONTINUE_RETURN_1;
}
