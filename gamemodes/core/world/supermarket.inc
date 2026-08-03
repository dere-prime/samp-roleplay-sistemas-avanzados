/* 

    24/7 SUPERMARKET
        PRIME ROLEPLAY

*/

#include <YSI_Coding\y_hooks>


enum Supermarket_Enum {
	product_PRICE,
	product_AMOUNT,
	product_TYPE,
}
static Supermarket_Product_List[][Supermarket_Enum] = {
	{350, 1, INVENTORY_PHONE},
	{200, 1, INVENTORY_PHONE_RESOLVER},
	{80, 3, INVENTORY_BURGER},
	{150, 4, INVENTORY_PIZZA},
	{50, 2, INVENTORY_WATER},
	{2500, 1, INVENTORY_WISKEY},
	{120, 6, INVENTORY_BEER}
};

static Float: supermarket_buy_pos[] = {2189.7678, -551.6679, 901.1849};
static DIALOG_247_LIST_String[34 + 55 * sizeof Supermarket_Product_List];

hook OnScriptInit() {
    CreateDynamic3DTextLabel("{"#PRIMARY_COLOR"}Acercate{FFFFFF} para comprar productos", 0xFFFFFFFF, supermarket_buy_pos[0], supermarket_buy_pos[1], (supermarket_buy_pos[2]), 10.0, .testlos = true, .interiorid = 18);
    new info[3], pickup_id = CreateDynamicPickup(19592, 1, supermarket_buy_pos[0], supermarket_buy_pos[1], (supermarket_buy_pos[2]), -1, 18);
    info[0] = PICKUP_TYPE_SUPERMARKET;
    Streamer_SetArrayData(STREAMER_TYPE_PICKUP, pickup_id, E_STREAMER_EXTRA_ID, info);

	for(new i = 0; i != sizeof Supermarket_Product_List; i ++) {
		strcatf(DIALOG_247_LIST_String, "{FFFFFF}%s {"#PRIMARY_COLOR"}[%d]\t{FFFFFF}%d{"#GREEN_COLOR"}$\n", InvObjName(Supermarket_Product_List[i][product_TYPE]), Supermarket_Product_List[i][product_AMOUNT], Supermarket_Product_List[i][product_PRICE]);
	}
}

hook OnPlayerPickUpDynPickup(playerid, pickupid) {
    if (pickupid == PICKUP_TYPE_NONE) return Y_HOOKS_CONTINUE_RETURN_1;
	if (PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] > gettime()) return Y_HOOKS_BREAK_RETURN_0;
	if (Dialog_IsOpen(playerid)) return Y_HOOKS_BREAK_RETURN_0;

	new info[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, info);
	if (info[0] == PICKUP_TYPE_SUPERMARKET) {
        Dialog_Show(playerid, Dialog:Supermarket_Products);
        PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 5;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

DialogCreate:Supermarket_Products(playerid) {
    Dialog_Open(playerid, Dialog:Supermarket_Products, DIALOG_STYLE_TABLIST, "Tienda · {"#PRIMARY_COLOR"}24/7", DIALOG_247_LIST_String, "Comprar", "Cerrar");
    return 1;
}
DialogResponse:Supermarket_Products(playerid, response, listitem, inputtext[]) {
    if (response) {
        PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 5;
        Dialog_Show(playerid, Dialog:Supermarket_Products);
        if (PI[playerid][pi_CASH] >= Supermarket_Product_List[listitem][product_PRICE]) {
            new slot = GetPlayerInventorySlot(playerid);
            if (slot == -1) {
                PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                ShowPlayerMessage(playerid, "No tienes más espacio en tu inventario.");
                return 1;
            }
            if (GetValidPlayerPhoneNumber(playerid)) {
                if (Supermarket_Product_List[listitem][product_TYPE] == INVENTORY_PHONE) {
                    ShowPlayerMessage(playerid, "Ya tienes un teléfono.");
                    PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                    return 1;
                }
            }
            if (GetValidPlayerInventoryType(playerid, INVENTORY_PHONE_RESOLVER) != -1) {
                if (Supermarket_Product_List[listitem][product_TYPE] == INVENTORY_PHONE_RESOLVER) {
                    ShowPlayerMessage(playerid, "Ya tienes una guia telefonica.");
                    PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
                    return 1;
                }
            }
            
            if (GivePlayerCash(playerid, -Supermarket_Product_List[listitem][product_PRICE], true, true)) {
                if (Supermarket_Product_List[listitem][product_TYPE] == INVENTORY_PHONE) {
                    PI[playerid][pi_PHONE_STATE] = PHONE_STATE_ON;
                    PI[playerid][pi_PHONE_VISIBLE_NUMBER] = true;
                    
                    new phone_number = getPhoneNumber(PI[playerid][pi_ID]);
                    SetPlayerInventorySlotData(playerid, slot, Supermarket_Product_List[listitem][product_TYPE], phone_number, phone_number, 0);
                }
                else SetPlayerInventorySlotData(playerid, slot, Supermarket_Product_List[listitem][product_TYPE], Supermarket_Product_List[listitem][product_AMOUNT], 0, 0);
                InsertPlayerInventorySlot(playerid, slot);

                SendPlayerClientMessage(playerid, "{"#PRIMARY_COLOR"}·{FFFFFF}Has comprado {"#SECONDARY_COLOR"}%s {FFFFFF}por %d{27F587}${FFFFFF}.", InvObjName(Supermarket_Product_List[listitem][product_TYPE]), Supermarket_Product_List[listitem][product_PRICE]);
                PlayerPlaySoundEx(playerid, 1058, 0.0, 0.0, 0.0);
            }
        }
        else {
            PlayerPlaySoundEx(playerid, 1085, 0.0, 0.0, 0.0);
            SendPlayerClientMessage(playerid, "No tienes dinero suficiente, te faltan %s$ para poder comprar este producto.", number_format_thousand(Supermarket_Product_List[listitem][product_PRICE] - PI[playerid][pi_CASH]));
        }
    }
    return 1;
}