/*

    Work Info
        Prime Roleplay
            Header

*/

/* Defines */
#define NECESSARY_TIME_BETWEEN_WORKS 600
#define POLICE_PAYMENT_MULTIPLIER 1000 //Multiplicador de PayDay SAPD.
#define VEHICLE_SPAWNED_TIMEOUT SecondsToMs(120)

/* Forward */
forward OnPlayerObtainWork(playerid, work);

/* WORKS INFO */
new male_medic_skins[] = {274, 275, 276};

enum {
	WORK_TYPE_NONE,
	WORK_TYPE_NORMAL,
	WORK_TYPE_FAMILY,
}
enum _:E_WORKS {
	WORK_NONE = 0,
	WORK_TAXI = 1,
	WORK_TRUCK = 2,
	WORK_MECHANIC = 3,
	WORK_HARVESTER = 4,
	WORK_FUMIGATOR = 5,
	WORK_TRASH = 6,
	WORK_LUMBERJACK = 7,
	WORK_FARMER = 8,
	WORK_POLICE = 9,
	WORK_PIZZA = 10,
	WORK_MEDIC = 11
};

enum work_info_info {
	work_info_TYPE,
	work_info_LEVEL,
	work_info_NAME[24],
	bool: work_info_NEED_DUTY,
	work_info_COLOR[24],
}
new const work_info[][work_info_info] = {
	{WORK_TYPE_NONE, 0, "ninguno", false, "000000"},
	{WORK_TYPE_NORMAL, 1, "taxista", false, "F4E00B"},
	{WORK_TYPE_NORMAL, 1, "camionero", false, "F59127"},
	{WORK_TYPE_NORMAL, 5, "mecanico", true, "09C8B8"},
	{WORK_TYPE_NORMAL, 1, "cosechador", false, "63F8A6"},
	{WORK_TYPE_NORMAL, 5, "fumigador", false, "F4DC0B"},
	{WORK_TYPE_NORMAL, 1, "basurero", true, "6CC809"},
	{WORK_TYPE_NORMAL, 1, "leñador", false, "702505"},
	{WORK_TYPE_NORMAL, 15, "agricultor", false, "0BF49B"},
	{WORK_TYPE_FAMILY, 0, "policía", true, "0965C8"},
	{WORK_TYPE_NORMAL, 1, "pizzero", true, "F40B0B"},
	{WORK_TYPE_NORMAL, 15, "médico", true, "F40B3E"}
};

enum obtain_work_coords_info {
	bool:obtain_work_AVAILABLE,
	bool:obtain_work_LABELS,
	Float:obtain_work_X,
	Float:obtain_work_Y,
	Float:obtain_work_Z,
	obtain_work_INTERIOR,
	bool:obtain_work_MAP_ICON,
	obtain_work_MAP_ICON_ID,
	Float:obtain_work_MAP_ICON_X,
	Float:obtain_work_MAP_ICON_Y,
	Float:obtain_work_MAP_ICON_Z,
}
new const Float:obtain_work_coords[][obtain_work_coords_info] = {
	{false, false, 0.0, 0.0, 0.0, 0, false, 0, 0.0, 0.0, 0.0},
	{true, true, -1735.9442, 468.9559, 2166.2393, 3, true, 61, 1752.603881, -1894.155883, 13.557376}, //taxista ls
	{true, true, -1735.9442, 468.9559, 2166.2393, 4, true, 61, -49.8563, -269.3626, 6.6332}, //caminero
	{true, true, 1831.917602, -1446.153564, 13.592857, 0, true, 27, 1831.917602, -1446.153564, 13.592857}, //mecanico
	{true, true, -372.126922, -1435.706298, 25.726562, 0,	true, 61, -372.126922, -1435.706298, 25.726562}, //Cosechador
	{true, true, -1342.295898, -2195.076660, 23.591030, 0, true, 61, -1342.295898, -2195.076660, 23.591030}, //Fumigador
	{true, true, -1884.997314, -1636.733642, 21.750000, 0, true, 61, -1884.997314, -1636.733642, 21.750000}, //Basurero
	{true, true, -532.029846, -97.514228, 63.296875, 0, true, 61, -532.029846, -97.514228, 63.296875}, //leñador
	{true, true, 1564.864257, 23.255302, 24.164062, 0, true, 61, 1564.864257, 23.255302, 24.164062},
	{true, true, -510.972015, 324.242736, 2004.585937, 20,	false, 0, 1480.966918, -1772.065673, 18.795755}, // policia
	{true, true, 377.902313, -119.416114, 1001.492187, 5, false, 0, 2105.485107, -1806.400878, 13.554687},
	{true, true, -2033.237304, -117.411125, 1035.171875, 3,	false, 0, 0.0, 0.0, 0.0}
};

enum enum_PLAYER_WORKS {
	pwork_SET,
	pwork_LEVEL
};
new PLAYER_WORKS[MAX_PLAYERS][ sizeof(work_info) ][enum_PLAYER_WORKS];

/* VEHICLES */
enum enum_work_vehicles_spawn {
	v_pickup_id,
	Text3D: v_text_3d,
	v_work,
	v_required_exp,
	bool: v_use_interface,
	v_interface_id,
	v_vehicle_model,
	v_color1,
	v_color2,
	Float: v_x,
	Float: v_y,
	Float: v_z,
	Float: v_angle
}
enum enum_work_vehicles_required_exp {
	r_work,
	r_modelid,
	r_required_exp
}

new const work_vehicles_spawn[][enum_work_vehicles_spawn] = {
	{INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, WORK_TAXI, 0, true, MODEL_SELECTION_WORK_TAXI, 0, 6, 6, 1795.3862, -1883.3627, 13.5725, 180.0},
    {INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, WORK_TRUCK, 0, false, MODEL_SELECTION_NONE, 456, 1, 1, -23.4199, -270.0182, 5.4297, 175.8792},
    {INVALID_STREAMER_ID, Text3D:INVALID_3DTEXT_ID, WORK_TRUCK, 0, false, MODEL_SELECTION_NONE, 456, 1, 1, -14.105, -270.0182, 5.4297, 175.8792}
};
new const work_vehicles_required_exp[][enum_work_vehicles_required_exp] = {
	{WORK_TAXI, 420, 0},
	{WORK_TAXI, 438, 15}
};

enum Work_Vehicles_Info {
	bool:work_vehicle_VALID,
	work_vehicle_WORK,
	work_vehicle_EXP,
	bool:work_vehicle_NEED_DUTY,
	work_vehicle_TIMEOUT,
	work_vehicle_PID
}
new WORK_VEHICLES[MAX_VEHICLES][Work_Vehicles_Info];