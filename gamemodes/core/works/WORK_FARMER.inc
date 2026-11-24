/* 

    Trabajo Agricultor - Prime Roleplay

*/
#define MAX_PLANTS 1000

#include <YSI_Coding\y_hooks>

// AGRICULTOR
enum seed_info_enum {
	seed_info_TYPE,
	seed_info_EXP,
	seed_info_INVENTORY_TYPE,
	seed_info_PRICE_PER_SEED //precio 1c/u
}
static const seed_info[][seed_info_enum] = {
	{TYPE_LEGAL, 0, INVENTORY_SEED_MEDICINE, 50},
	{TYPE_ILLEGAL, 100, INVENTORY_SEED_CANNABIS, 150},
	{TYPE_ILLEGAL, 200, INVENTORY_SEED_CRACK, 300}
};

enum plant_info_enum {
	plant_info_SEEDS,
	plant_info_NAME[32],
	plant_info_MODEL
}
static const plant_info[][plant_info_enum] = {
	{2, "{"#GREEN_COLOR"}Medicamento", 682},
	{4, "{"#ERROR_COLOR"}Marihuana", 678},
	{6, "{"#ERROR_COLOR"}Crack", 630}
};

enum plant_result_info_enum {
	result_AMOUNT,
	result_TYPE
}
static const plant_result_info[][plant_result_info_enum] = {
	{2, INVENTORY_MEDICINE},
	{2, INVENTORY_CANNABIS},
	{4, INVENTORY_CRACK}
};

static const Float: farmer_buy_seeds_pos[][] = {
    {1547.9410, 15.0675, 24.1387},
    {1542.8691, 14.2591, 24.1345},
	{1541.9225, 18.1561, 24.1406},
    {1548.4288, 19.4098, 24.1406}
};

static const PlayerText: p_FarmerProgressTd[MAX_PLAYERS][5];

enum farmer_plant_info {
	bool:plant_VALID,
	plant_PID,
	plant_PLAYERID,
	plant_TYPE,
	plant_STATE,
	plant_TIMER,
	plant_MODELID,
	Text3D:plant_TEXT,
	plant_UPDATE_TIMER,
	Float:plant_GROW_PROGRESS,
	plant_OWNER_NAME[24]
}

static const f_plant[MAX_PLANTS][farmer_plant_info];

forward UpdatePlayerPlanting(const playerid, const plant_info_index);
forward UpdatePlantInfo(index);

/* IMPL */
GetValidPlantIndex() {
	for (new i = 0; i != MAX_PLANTS; i ++) {
		if (!f_plant[i][plant_VALID]) return i;
	}
	return -1;
}

LoadFarmerInfo() {
	Farmer_Area = CreateDynamicCircle(1461.8894, -83.2621, 65.0, 0, 0);
	CreateDynamicMapIcon(1461.8894, -83.2621, 65.0, 53, -1, 0, 0);

	new info[3] = 0;
	for (new i = 0; i != sizeof(farmer_buy_seeds_pos); i ++) {
		CreateDynamic3DTextLabel("{"#GREEN_COLOR"}·Agricultor{f0f0f0}\nPuesto de venta de semillas", 0xFFFFFFFF, farmer_buy_seeds_pos[i][0], farmer_buy_seeds_pos[i][1], farmer_buy_seeds_pos[i][2], 5.0, .testlos = true, .worldid = 0, .interiorid = 0);
		new Farmer_Pickup = CreateDynamicPickup(0, 1, farmer_buy_seeds_pos[i][0], farmer_buy_seeds_pos[i][1], farmer_buy_seeds_pos[i][2]);
		info[0] = PICKUP_TYPE_FARMER_BUY_SEEDS;
		Streamer_SetArrayData(STREAMER_TYPE_PICKUP, Farmer_Pickup, E_STREAMER_EXTRA_ID, info);
	}
	return 1;
}

Farmer_StartWork(playerid) {
	ShowPlayerNotification(playerid, "Has entrado en zona de agricultura~n~Pulsa ~g~Y~w~~h~ para mostrar el menu de trabajo.");
	return 1;
}

Farmer_EndWork(playerid) {
	#pragma unused playerid
	return 1;
}

IsNearAnyFarmerPlant(const playerid) {
	for (new i = 0; i != MAX_PLANTS; i ++) {
		if (!f_plant[i][plant_VALID]) continue;
		if (!IsValidDynamicObject(f_plant[i][plant_MODELID])) continue;

		new Float:x, Float:y, Float:z;
		GetDynamicObjectPos(f_plant[i][plant_MODELID], x, y, z);

		if (IsPlayerInRangeOfPoint(playerid, 4.0, x, y, z)) {
			return i;
		}
	}
	return -1;
}

ShowPlayerPlantingTd(const playerid) {
	UpdatePlayerPlantingTdBar(playerid);
	for (new i = 0; i != sizeof(p_FarmerProgressTd[]); i ++) {
		PlayerTextDrawShow(playerid, p_FarmerProgressTd[playerid][i]);
	}
	return 1;
}

HidePlayerPlantingTd(const playerid) {
	UpdatePlayerPlantingTdBar(playerid);
	for (new i = 0; i != sizeof(p_FarmerProgressTd[]); i ++) {
		PlayerTextDrawHide(playerid, p_FarmerProgressTd[playerid][i]);
	}
	return 1;
}

UpdatePlayerPlantingTdBar(const playerid) {
    new Float:size = 296.0 + (PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] * 0.64);
    PlayerTextDrawTextSize(playerid, p_FarmerProgressTd[playerid][4], size, 0.000000);
	PlayerTextDrawShow(playerid, p_FarmerProgressTd[playerid][4]);
	return 1;
}

DestroyFarmerPlant(index) {
	if (IsValidDynamicObject(f_plant[index][plant_MODELID])) {
		DestroyDynamicObject(f_plant[index][plant_MODELID]);
	}
	if (IsValidDynamic3DTextLabel(f_plant[index][plant_TEXT])) {
		DestroyDynamic3DTextLabel(f_plant[index][plant_TEXT]);
	}
	f_plant[index][plant_VALID] = false;
	f_plant[index][plant_PID] = 0;
	f_plant[index][plant_PLAYERID] = 0;
	f_plant[index][plant_TYPE] = 0;
	f_plant[index][plant_STATE] = 0;
	f_plant[index][plant_TIMER] = 0;
	return 1;
}

public UpdatePlantInfo(index) {
	KillTimer(f_plant[index][plant_UPDATE_TIMER]);

	if (!f_plant[index][plant_VALID]) return 0;
	
	new label_str[256];
	if (f_plant[index][plant_STATE] == 1) {
		f_plant[index][plant_GROW_PROGRESS] += 3.2;
		if (f_plant[index][plant_GROW_PROGRESS] >= 100.0) f_plant[index][plant_GROW_PROGRESS] = 100.0;
		format(label_str, sizeof(label_str), "%s\n{f0f0f0}Creciendo... %.1f", plant_info[ f_plant[index][plant_TYPE] ][plant_info_NAME], f_plant[index][plant_GROW_PROGRESS]);
		UpdateDynamic3DTextLabelText(f_plant[index][plant_TEXT], -1, label_str);
	} else if (f_plant[index][plant_STATE] == 2) {
		new time = (900-(gettime()-f_plant[index][plant_TIMER]));
		format(label_str, sizeof(label_str), "%s\n{f0f0f0}Planta de {"#GREEN_COLOR"}%s\n{f0f0f0}Tiempo restante {"#PRIMARY_COLOR"}%s\n\n{f0f0f0}Pulsa {"#SECONDARY_COLOR"}Y{f0f0f0} para recoger", plant_info[ f_plant[index][plant_TYPE] ][plant_info_NAME], f_plant[index][plant_OWNER_NAME], TimeConvertAsText(time));
		UpdateDynamic3DTextLabelText(f_plant[index][plant_TEXT], -1, label_str);
	}

	if (f_plant[index][plant_GROW_PROGRESS] >= 100.0 && f_plant[index][plant_STATE] == 1) {
		f_plant[index][plant_TIMER] = gettime();
		f_plant[index][plant_STATE] = 2;
	}
	if (f_plant[index][plant_STATE] == 2 && gettime() > f_plant[index][plant_TIMER] + 900) {
		DestroyFarmerPlant(index);
		return 1;
	}

	SetTimerEx("UpdatePlantInfo", 1000, false, "i", index);
	return 1;
}

public UpdatePlayerPlanting(const playerid, const plant_info_index) {
	if (PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMER] != -1) { KillTimer(PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMER]); PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMER] = -1; }
	if (!IsPlayerConnected(playerid) || !IsPlayerInDynamicArea(playerid, Farmer_Area) || !PLAYER_WORKS[playerid][WORK_FARMER][pwork_SET]) {
		PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] = 0.0;
		HidePlayerPlantingTd(playerid);
		return 1;
	}

	if (PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] >= 100.0) {		
		PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] = 0.0;
		HidePlayerPlantingTd(playerid);
		ClearAnimations(playerid);

		new plant_id = GetValidPlantIndex();
		if (plant_id == -1) return ShowPlayerMessage(playerid, "No se puede plantar mas...");

		f_plant[plant_id][plant_VALID] = true;
		f_plant[plant_id][plant_PID] = PI[playerid][pi_ID];
		f_plant[plant_id][plant_PLAYERID] = playerid;
		f_plant[plant_id][plant_TYPE] = plant_info_index;
		f_plant[plant_id][plant_STATE] = 1;
		f_plant[plant_id][plant_TIMER] = 0;
		format(f_plant[plant_id][plant_OWNER_NAME], 24, "%s", PLAYER_TEMP[playerid][pt_NAME]);
		f_plant[plant_id][plant_GROW_PROGRESS] = 0.0;

		GetPlayerPos(playerid, PI[playerid][pi_POS_X], PI[playerid][pi_POS_Y], PI[playerid][pi_POS_Z]);
		f_plant[plant_id][plant_MODELID] = CreateDynamicObject(plant_info[ f_plant[plant_id][plant_TYPE] ][plant_info_MODEL], PI[playerid][pi_POS_X], PI[playerid][pi_POS_Y], PI[playerid][pi_POS_Z] - 1.0, 0.0, 0.0, 0.0, 0, 0, -1, 600.0, 600.0);
		f_plant[plant_id][plant_TEXT] = CreateDynamic3DTextLabel("", -1, PI[playerid][pi_POS_X], PI[playerid][pi_POS_Y], PI[playerid][pi_POS_Z], 20.0, .testlos = true, .worldid = 0, .interiorid = 0);

		UpdatePlantInfo(plant_id);
		return 1;
	}

    new Float:value = (20.0 + floatmul(PLAYER_WORKS[playerid][WORK_FARMER][pwork_LEVEL], 0.1));
    if (value > 35.0) value = 35.0;

	PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] += value;
	if (PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] > 100.0) PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] = 100.0;

	UpdatePlayerPlantingTdBar(playerid);

	if (GetPlayerAnimationIndex(playerid) != 168) ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, 1, 0, 0, 0, 0, 0);

	PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMER] = SetTimerEx("UpdatePlayerPlanting", 1000, true, "ii", playerid, PLAYER_TEMP[playerid][pt_SELECTED_PLANT_ID]);
	return 1;
}

hook OnPlayerPickUpDynPickup(playerid, pickupid) {
    if (pickupid == PICKUP_TYPE_NONE) return Y_HOOKS_CONTINUE_RETURN_1;
	if (PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] > gettime()) return Y_HOOKS_BREAK_RETURN_0;
	if (Dialog_IsOpen(playerid)) return Y_HOOKS_BREAK_RETURN_0;
    if (MS_IsOpen(playerid)) return Y_HOOKS_BREAK_RETURN_0;

	new info[3], Float:pos[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, info);
	if (info[0] == PICKUP_TYPE_FARMER_BUY_SEEDS) {
		Streamer_GetFloatData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_X, pos[0]);
		Streamer_GetFloatData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_Y, pos[1]);
		Streamer_GetFloatData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_Z, pos[2]);
		if (!IsPlayerInRangeOfPoint(playerid, 1.5, pos[0], pos[1], pos[2])) return 1;
		if (!IsPlayerWork(playerid, WORK_FARMER)) return ShowPlayerMessage(playerid, "Debes ser un agricultor para comprar semillas.");

		Dialog_Show(playerid, Dialog:FarmerSeedList);

		PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 10;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid) {
    p_FarmerProgressTd[playerid][0] = CreatePlayerTextDraw(playerid, 320.000, 340.000, "background");
    PlayerTextDrawLetterSize(playerid, p_FarmerProgressTd[playerid][0], 0.000, 2.500);
    PlayerTextDrawTextSize(playerid, p_FarmerProgressTd[playerid][0], 0.000, 90.000);
    PlayerTextDrawAlignment(playerid, p_FarmerProgressTd[playerid][0], 2);
    PlayerTextDrawColor(playerid, p_FarmerProgressTd[playerid][0], -1);
    PlayerTextDrawUseBox(playerid, p_FarmerProgressTd[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, p_FarmerProgressTd[playerid][0], 255);
    PlayerTextDrawSetShadow(playerid, p_FarmerProgressTd[playerid][0], 1);
    PlayerTextDrawSetOutline(playerid, p_FarmerProgressTd[playerid][0], 1);
    PlayerTextDrawBackgroundColor(playerid, p_FarmerProgressTd[playerid][0], 150);
    PlayerTextDrawFont(playerid, p_FarmerProgressTd[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, p_FarmerProgressTd[playerid][0], 1);

    p_FarmerProgressTd[playerid][1] = CreatePlayerTextDraw(playerid, 246.000, 313.000, "_");
    PlayerTextDrawTextSize(playerid, p_FarmerProgressTd[playerid][1], 70.000, 70.000);
    PlayerTextDrawAlignment(playerid, p_FarmerProgressTd[playerid][1], 1);
    PlayerTextDrawColor(playerid, p_FarmerProgressTd[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, p_FarmerProgressTd[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, p_FarmerProgressTd[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, p_FarmerProgressTd[playerid][1], 0);
    PlayerTextDrawFont(playerid, p_FarmerProgressTd[playerid][1], 5);
    PlayerTextDrawSetProportional(playerid, p_FarmerProgressTd[playerid][1], 0);
    PlayerTextDrawSetPreviewModel(playerid, p_FarmerProgressTd[playerid][1], 2195);
    PlayerTextDrawSetPreviewRot(playerid, p_FarmerProgressTd[playerid][1], -25.000, 0.000, -45.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, p_FarmerProgressTd[playerid][1], 0, 0);

    p_FarmerProgressTd[playerid][2] = CreatePlayerTextDraw(playerid, 323.000, 339.000, "Plantando....");
    PlayerTextDrawLetterSize(playerid, p_FarmerProgressTd[playerid][2], 0.180, 0.900);
    PlayerTextDrawAlignment(playerid, p_FarmerProgressTd[playerid][2], 2);
    PlayerTextDrawColor(playerid, p_FarmerProgressTd[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, p_FarmerProgressTd[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, p_FarmerProgressTd[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, p_FarmerProgressTd[playerid][2], 150);
    PlayerTextDrawFont(playerid, p_FarmerProgressTd[playerid][2], 2);
    PlayerTextDrawSetProportional(playerid, p_FarmerProgressTd[playerid][2], 1);

    p_FarmerProgressTd[playerid][3] = CreatePlayerTextDraw(playerid, 299.000, 353.000, "bar_bg");
    PlayerTextDrawLetterSize(playerid, p_FarmerProgressTd[playerid][3], 0.000, 0.400);
    PlayerTextDrawTextSize(playerid, p_FarmerProgressTd[playerid][3], 360.000, 0.000);
    PlayerTextDrawAlignment(playerid, p_FarmerProgressTd[playerid][3], 1);
    PlayerTextDrawColor(playerid, p_FarmerProgressTd[playerid][3], -1);
    PlayerTextDrawUseBox(playerid, p_FarmerProgressTd[playerid][3], 1);
    PlayerTextDrawBoxColor(playerid, p_FarmerProgressTd[playerid][3], 1433087999);
    PlayerTextDrawSetShadow(playerid, p_FarmerProgressTd[playerid][3], 1);
    PlayerTextDrawSetOutline(playerid, p_FarmerProgressTd[playerid][3], 1);
    PlayerTextDrawBackgroundColor(playerid, p_FarmerProgressTd[playerid][3], 150);
    PlayerTextDrawFont(playerid, p_FarmerProgressTd[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, p_FarmerProgressTd[playerid][3], 1);

    p_FarmerProgressTd[playerid][4] = CreatePlayerTextDraw(playerid, 299.000, 353.000, "bar_progress");
    PlayerTextDrawLetterSize(playerid, p_FarmerProgressTd[playerid][4], 0.000, 0.400);
    PlayerTextDrawTextSize(playerid, p_FarmerProgressTd[playerid][4], 360.000, 0.000); //min 296.0
    PlayerTextDrawAlignment(playerid, p_FarmerProgressTd[playerid][4], 1);
    PlayerTextDrawColor(playerid, p_FarmerProgressTd[playerid][4], -1);
    PlayerTextDrawUseBox(playerid, p_FarmerProgressTd[playerid][4], 1);
    PlayerTextDrawBoxColor(playerid, p_FarmerProgressTd[playerid][4], -1697828097);
    PlayerTextDrawSetShadow(playerid, p_FarmerProgressTd[playerid][4], 1);
    PlayerTextDrawSetOutline(playerid, p_FarmerProgressTd[playerid][4], 1);
    PlayerTextDrawBackgroundColor(playerid, p_FarmerProgressTd[playerid][4], 150);
    PlayerTextDrawFont(playerid, p_FarmerProgressTd[playerid][4], 1);
    PlayerTextDrawSetProportional(playerid, p_FarmerProgressTd[playerid][4], 1);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

DialogCreate:FarmerSeedList(playerid) {
    new dialog[64 * (sizeof(seed_info) + 1)];
    for(new i = 0; i != sizeof seed_info; i ++) {
        if (PLAYER_WORKS[playerid][WORK_FARMER][pwork_LEVEL] >= seed_info[i][seed_info_EXP]) {
            if (seed_info[i][seed_info_TYPE] == TYPE_LEGAL) strcatf(dialog, "{"#GREEN_COLOR"}%s\n", g_inventory_data[ seed_info[i][seed_info_INVENTORY_TYPE] ][g_info_name]);
            else strcatf(dialog, "{"#ERROR_COLOR"}%s\n", g_inventory_data[ seed_info[i][seed_info_INVENTORY_TYPE] ][g_info_name]);
        }
        else strcatf(dialog, "{666666}%s\n", g_inventory_data[ seed_info[i][seed_info_INVENTORY_TYPE] ][g_info_name]);
    }
    
    Dialog_Open(playerid, Dialog:FarmerSeedList, DIALOG_STYLE_TABLIST, "{"#GREEN_COLOR"}Agricultor · {f0f0f0}Semillas", dialog, "Continuar", "Cerrar");
    return 1;
}
DialogResponse:FarmerSeedList(playerid, response, listitem) {
	if (response) {
		if (PLAYER_WORKS[playerid][WORK_FARMER][pwork_LEVEL] < seed_info[listitem][seed_info_EXP]) { Dialog_Show(playerid, Dialog:FarmerSeedList); return 1; }

		PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID] = listitem;
		Dialog_Show(playerid, Dialog:FarmerSeedQuantity);
	}
	return 1;
}

DialogCreate:FarmerSeedQuantity(playerid) {
	new dialog[256];
	format(dialog, sizeof dialog, "{"#GREEN_COLOR"}·{f0f0f0}¿Cuántas semillas de {"#GREEN_COLOR"}%s{f0f0f0} quieres comprar? (Precio: %d{"#GREEN_COLOR"}${f0f0f0} cada una):", g_inventory_data[ seed_info[PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID]][seed_info_INVENTORY_TYPE] ][g_info_name], seed_info[PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID]][seed_info_PRICE_PER_SEED]);
	Dialog_Open(playerid, Dialog:FarmerSeedQuantity, DIALOG_STYLE_INPUT, "{"#GREEN_COLOR"}Agricultor · {f0f0f0}Cantidad", dialog, "Comprar", "Cancelar");
	return 1;
}
DialogResponse:FarmerSeedQuantity(playerid, response, listitem, inputtext[]) {
	if (response) {
		new quantity;
		if (sscanf(inputtext, "d", quantity)) return ShowPlayerMessage(playerid, "Error en los parámetros.");
		if (quantity <= 0 || quantity > 100) return ShowPlayerMessage(playerid, "La cantidad debe ser mayor a 0 y menor a 100.");
		
		new slot = GetPlayerInventorySlot(playerid);
		if (slot == -1) {
			PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
			ShowPlayerMessage(playerid, "No tienes más espacio en tu inventario.");
			return 1;
		}

		new total_price = quantity * seed_info[PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID]][seed_info_PRICE_PER_SEED];
		if (PI[playerid][pi_CASH] < total_price) return ShowPlayerMessage(playerid, "No tienes suficiente dinero para comprar esa cantidad de semillas.");

		if (GivePlayerCash(playerid, -total_price, true, true)) {
			SetPlayerInventorySlotData(playerid, slot, seed_info[PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID]][seed_info_INVENTORY_TYPE], quantity, 0, 0);
			InsertPlayerInventorySlot(playerid, slot);

			ShowPlayerMessage(playerid, "Has comprado %d semillas de %s por -%s"#N_CURR".", quantity, g_inventory_data[ seed_info[PLAYER_TEMP[playerid][pt_SELECTED_BUY_SEED_ID]][seed_info_INVENTORY_TYPE] ][g_info_name], number_format_thousand(total_price));
		}
	}
	return 1;
}

/* Plant System */
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if (newkeys & KEY_YES) {
		if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && PI[playerid][pi_STATE] == ROLEPLAY_STATE_NORMAL) {
			if (IsPlayerInDynamicArea(playerid, Farmer_Area)) {
				if (IsPlayerWork(playerid, WORK_FARMER) && PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] < 1.0) {
					for (new i = 0; i != MAX_PLANTS; i ++) {
						if (!f_plant[i][plant_VALID]) continue;
						if (!IsValidDynamicObject(f_plant[i][plant_MODELID])) continue;
						new Float:x, Float:y, Float:z;
						GetDynamicObjectPos(f_plant[i][plant_MODELID], x, y, z);
						
						if (IsPlayerInRangeOfPoint(playerid, 4.0, x, y, z)) {
							if (f_plant[i][plant_STATE] != 2) return ShowPlayerNotification(playerid, "La planta esta creciendo...");
							new slot = GetPlayerInventorySlot(playerid);
							if (slot == -1) {
								PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
								ShowPlayerNotification(playerid, "No tienes más espacio en tu inventario.");
								return 1;
							}

							ApplyAnimation(playerid, "CARRY", "putdwn05", 4.1, false, false, false, false, 0);

							PLAYER_WORKS[playerid][WORK_FARMER][pwork_LEVEL] ++;
							AddPlayerReputation(playerid, minrand(25, 100));

							SetPlayerInventorySlotData(playerid, slot, plant_result_info[ f_plant[i][plant_TYPE] ][result_TYPE], plant_result_info[ f_plant[i][plant_TYPE] ][result_AMOUNT], 0, 0);
							InsertPlayerInventorySlot(playerid, slot);
							ShowPlayerNotification(playerid, "Has conseguido ~g~%d %s~w~ de esta planta.", plant_result_info[ f_plant[i][plant_TYPE] ][result_AMOUNT], g_inventory_data[ plant_result_info[ f_plant[i][plant_TYPE] ][result_TYPE] ][g_info_name]);
							DestroyFarmerPlant(i);
							return 1;
						}
					}

					Dialog_Show(playerid, Dialog:FarmerSelectSeedForPlant);
				}
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

DialogCreate:FarmerSelectSeedForPlant(playerid) {
	new dialog[400];
	strcat(dialog, "{"#PRIMARY_COLOR"}·{f0f0f0}Abrir menu de interaccion\n");
	for (new i = 0; i != sizeof(plant_info); i ++) {
		strcatf(dialog, "%s\t{f0f0f0}%d Semillas requeridas *\n", plant_info[i][plant_info_NAME], plant_info[i][plant_info_SEEDS]);
	}
	Dialog_Open(playerid, Dialog:FarmerSelectSeedForPlant, DIALOG_STYLE_TABLIST, "{"#GREEN_COLOR"}Agricultor · {f0f0f0}Seleccionar Tipo", dialog, "Continuar", "Cerrar");
	return 1;
}
DialogResponse:FarmerSelectSeedForPlant(playerid, response, listitem, inputtext[]) {
	if (response) {
		if (listitem == 0) {
			return Dialog_Show(playerid, Dialog:InteractionMenu);
		}

		PLAYER_TEMP[playerid][pt_SELECTED_PLANT_ID] = (listitem-1);
		Dialog_Show(playerid, Dialog:FarmerSelectSeedFromInv);
	}
	return 1;
}

DialogCreate:FarmerSelectSeedFromInv(playerid) {
	new results = 0, dialog[64 * MAX_PLAYER_SLOTS];
	for(new i = 0; i != MAX_LISTITEMS; i++ ) PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][i] = -1;
	for (new i = 0; i != MAX_PLAYER_SLOTS; i ++) {
		if (!p_inventory[playerid][i][p_inventory_valid]) continue;
		if (p_inventory[playerid][i][p_inventory_type] != seed_info[ PLAYER_TEMP[playerid][pt_SELECTED_PLANT_ID] ][seed_info_INVENTORY_TYPE]) continue;
		
		strcatf(dialog, "%s {"#GREEN_COLOR"}[%d]\n", g_inventory_data[ p_inventory[playerid][i][p_inventory_type] ][g_info_name], p_inventory[playerid][i][p_inventory_amount]);
		PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][results] = i;
		results ++;
	}
	if (!results) { dialog = "{666666}No tienes semillas de este tipo en tu inventario."; PLAYER_TEMP[playerid][pt_SELECTED_PLANT_ID] = -1; }
	Dialog_Open(playerid, Dialog:FarmerSelectSeedFromInv, DIALOG_STYLE_TABLIST, "{"#GREEN_COLOR"}Agricultor · {f0f0f0}Seleccionar Semilla", dialog, "Continuar", "Cerrar");
}
DialogResponse:FarmerSelectSeedFromInv(playerid, response, listitem) {
	if (response) {
		if (PLAYER_TEMP[playerid][pt_SELECTED_PLANT_ID] == -1) return 1;
		if (PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem] == -1) return 1;

		if (GetValidPlantIndex() == -1) return ShowPlayerMessage(playerid, "No hay mas espacios disponibles para plantar, espera un poco.");
		if (PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] > 0.0) return ShowPlayerMessage(playerid, "Ya estás plantando, espera a terminar para volver a plantar.");

		if (!IsPlayerInDynamicArea(playerid, Farmer_Area)) return ShowPlayerMessage(playerid, "Debes estar en el área de plantacion para plantar.");
		if (!IsPlayerWork(playerid, WORK_FARMER)) return ShowPlayerMessage(playerid, "Debes ser un agricultor para plantar.");
		new Float:p_altitude;
		GetPlayerAltitude(playerid, p_altitude);
		if (p_altitude > 1.1) return ShowPlayerMessage(playerid, "Debes estar en el suelo para plantar esto.");
		if (IsNearAnyFarmerPlant(playerid) != -1) return ShowPlayerMessage(playerid, "Ya hay una planta cerca de ti, no puedes plantar aquí.");
		ShowPlayerTimeOut(PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMEOUT], 60, playerid, "Debes esperar ~b~%s~w~ para volver a plantar.");
		
		new selected_slot = PLAYER_TEMP[playerid][pt_PLAYER_LISTITEM][listitem];
		if (p_inventory[playerid][selected_slot][p_inventory_amount] < plant_info[ PLAYER_TEMP[playerid][pt_SELECTED_PLANT_ID] ][plant_info_SEEDS]) {
			return ShowPlayerMessage(playerid, "No tienes suficientes semillas para plantar esto.~n~Puedes juntar estas semillas con otras desde el inventario.");
		}

		p_inventory[playerid][selected_slot][p_inventory_amount] -= plant_info[ PLAYER_TEMP[playerid][pt_SELECTED_PLANT_ID] ][plant_info_SEEDS];
		if (p_inventory[playerid][selected_slot][p_inventory_amount] <= 0) {
			RemovePlayerInventorySlot(playerid, selected_slot);
		} else {
			SavePlayerInventorySlot(playerid, selected_slot);
		}

		PLAYER_TEMP[playerid][pt_FARMER_PLANTING_PROGRESS] = 0.0;
		ShowPlayerPlantingTd(playerid);

		PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMEOUT] = gettime();
		if (PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMER] != -1) { KillTimer(PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMER]); PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMER] = -1; }

		PLAYER_TEMP[playerid][pt_FARMER_PLANT_TIMER] = SetTimerEx("UpdatePlayerPlanting", 0, true, "ii", playerid, PLAYER_TEMP[playerid][pt_SELECTED_PLANT_ID]);
	}
	return 1;
}
