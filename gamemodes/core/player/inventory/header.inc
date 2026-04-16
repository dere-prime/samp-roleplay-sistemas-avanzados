/* 

    Inventory
        Header

*/

#define MAX_PLAYER_INVENTORY_TD 31
#define MAX_PLAYER_SLOTS 16

#define InvObjName(%0) g_inventory_data[%0][g_info_name]

enum e_inv_slot_info {
    i_specific_slot_type
}
new inventory_slot_info[MAX_PLAYER_SLOTS][e_inv_slot_info] = {
    {INVENTORY_NONE}, // Slot 0
    {INVENTORY_NONE}, // Slot 1
    {INVENTORY_NONE}, // Slot 2
    {INVENTORY_NONE}, // Slot 3
    {INVENTORY_NONE}, // Slot 4
    {INVENTORY_NONE}, // Slot 5
    {INVENTORY_NONE}, // Slot 6
    {INVENTORY_NONE}, // Slot 7
    {INVENTORY_NONE}, // Slot 8
    {INVENTORY_NONE}, // Slot 9
    {INVENTORY_NONE}, // Slot 10
    {INVENTORY_NONE}, // Slot 11
    {INVENTORY_PHONE}, // Slot 12
    {INVENTORY_PHONE_RESOLVER}, // Slot 13
    {INVENTORY_NONE}, // Slot 14
    {INVENTORY_NONE}  // Slot 15
};

enum p_inventory_info {
    bool: p_inventory_valid,
    p_inventory_id,
    p_inventory_type,
    p_inventory_amount,
    p_inventory_data,
    p_inventory_extra
}

enum _:E_DroppedInventoryObjects {
    drp_Type,
    drp_Amount,
    drp_Data,
    drp_Extra,
    
    drp_ObjectId,
    Text3D:drp_LabelId,

    drp_World,
    drp_Interior,
    Float:drp_X,
    Float:drp_Y,
    Float:drp_Z
};

new p_inventory[MAX_PLAYERS][MAX_PLAYER_SLOTS][p_inventory_info];
new p_inventory_click_timer[MAX_PLAYERS];

new List:DroppedInventoryObjects;

enum g_inventory_data_enum {
    g_info_name[24],
    bool: g_info_useamount,
    Float: g_info_hungry,
    Float: g_info_thirst,
    g_info_drunk,
    g_info_extra,
    g_info_model,
    Float:g_info_rot_x,
    Float:g_info_rot_y,
    Float:g_info_rot_z,
    Float:g_info_zoom,
    bool: g_info_illegal,
    g_info_description[128]
}

new g_inventory_data[][g_inventory_data_enum] = {
    {"", false, 0.0, 0.0, 0, 0, 0, 0.000000, 0.000000, 0.000000, 1.000000, false, ""},
    {"Hamburguesa", true, 4.8, 0.3, 0, 0, 2703, 60.000000, 0.000000, 0.000000, 1.000000, false, "La hamburguesa regenera 4.8 puntos de hambre y 0.3 puntos de sed."},
    {"Pizza", true, 3.2, 0.0, 0, 0, 2702, -5.000000, -50.000000, 30.000000, 1.000000, false, "La pizza regenera 3.2 puntos de hambre."},
    {"Agua", true, 0.0, 6.0, 0, 0, 19570, 0.000000, 0.000000, 0.000000, 1.000000, false, "Usa agua para hidratar a tu personaje cuando este requiera."},
    {"Wisky", true, 0.0, 1.2, 4000, 0, 19820, 0.000000, 0.000000, 0.000000, 1.000000, false, "Un buen wisky no vendria mal de vez en cuando aunque quite poca sed."},
    {"Cerveza", true, 0.0, 1.2, 1000, 0, 1509, 0.000000, 0.000000, 0.000000, 1.000000, false, "Una rica cerveza refrescante puede subir el estado de animo."},

    {"Telefono", true, 0.0, 0.0, 0, 0, 18874, -80.000000, 30.000000, 175.000000, 1.000000, false, "Telefono movil para comunicarte con otros jugadores."},
    {"Guia Telefonica", true, 0.0, 0.0, 0, 0, 2855, 0.000000, 0.000000, 0.000000, 1.000000, false, "Guia telefonica para consultar los numeros de otros jugadores."},
    {"Bidon de combustible", false, 0.0, 0.0, 0, 0, 1650, 0.000000, 0.000000, 0.000000, 1.000000, false, "Bidon de combustible para rellenar vehiculos."},

    /* ARMAS */
    {"Puño Americano", false, 0.0, 0.0, 0, 1, 331, 0.000000, 0.000000, 0.000000, 1.000000, true, "Arma de impacto que aumenta el daño en combate cuerpo a cuerpo."},
    {"Palo de Golf", false, 0.0, 0.0, 0, 2, 333, 0.000000, 0.000000, 0.000000, 1.000000, false, "Palo deportivo usado como arma contundente."},
    {"Porra", false, 0.0, 0.0, 0, 3, 334, 0.000000, 0.000000, 0.000000, 1.000000, true, "Bastón policial diseñado para reducir sospechosos."},
    {"Cuchillo", false, 0.0, 0.0, 0, 4, 335, 0.000000, 0.000000, 0.000000, 1.000000, true, "Arma blanca eficaz a corta distancia."},
    {"Bate", false, 0.0, 0.0, 0, 5, 336, 0.000000, 0.000000, 0.000000, 1.000000, true, "Bate de béisbol ideal para golpes contundentes."},
    {"Pala", false, 0.0, 0.0, 0, 6, 337, 0.000000, 0.000000, 0.000000, 1.000000, true, "Herramienta pesada que puede usarse como arma."},
    {"Palo de Billar", false, 0.0, 0.0, 0, 7, 338, 0.000000, 0.000000, 0.000000, 1.000000, false, "Palo largo, ligero pero efectivo en manos hábiles."},
    {"Katana", false, 0.0, 0.0, 0, 8, 339, 0.000000, 0.000000, 0.000000, 1.000000, true, "Espada afilada capaz de causar cortes letales."},
    {"Motosierra", false, 0.0, 0.0, 0, 9, 341, 0.000000, 0.000000, 0.000000, 1.000000, true, "Herramienta motorizada extremadamente peligrosa."},
    {"Dildo", false, 0.0, 0.0, 0, 10, 321, 0.000000, 0.000000, 0.000000, 1.000000, true, "Objeto improvisado utilizado como arma."},
    {"Dildo 2", false, 0.0, 0.0, 0, 11, 322, 0.000000, 0.000000, 0.000000, 1.000000, true, "Objeto improvisado utilizado como arma."},
    {"Vibrador", false, 0.0, 0.0, 0, 12, 323, 0.000000, 0.000000, 0.000000, 1.000000, true, "Objeto improvisado utilizado como arma."},
    {"Vibrador 2", false, 0.0, 0.0, 0, 13, 324, 0.000000, 0.000000, 0.000000, 1.000000, true, "Objeto improvisado utilizado como arma."},
    {"Flor", false, 0.0, 0.0, 0, 14, 325, 0.000000, 0.000000, 0.000000, 1.000000, true, "Elemento decorativo con poco uso en combate."},
    {"Baston", false, 0.0, 0.0, 0, 15, 326, 0.000000, 0.000000, 0.000000, 1.000000, true, "Bastón elegante pero útil para defenderse."},
    {"Granada", false, 0.0, 0.0, 0, 16, 342, 0.000000, 0.000000, 0.000000, 1.000000, true, "Explosivo de mano que detona tras lanzarlo."},
    {"Gas Lacrimogeno", false, 0.0, 0.0, 0, 17, 343, 0.000000, 0.000000, 0.000000, 1.000000, true, "Dispositivo que libera gas irritante."},
    {"Molotov", false, 0.0, 0.0, 0, 18, 344, 0.000000, 0.000000, 0.000000, 1.000000, true, "Botella incendiaria de fabricación casera."},
    {"Colt 45", false, 0.0, 0.0, 0, 22, 346, 0.000000, 0.000000, 0.000000, 1.000000, true, "Pistola estándar, equilibrada y confiable."},
    {"9mm Silenciada", false, 0.0, 0.0, 0, 23, 347, 0.000000, 0.000000, 0.000000, 1.000000, true, "Pistola con supresor para disparos discretos."},
    {"Desert Eagle", false, 0.0, 0.0, 0, 24, 348, 0.000000, 0.000000, 0.000000, 1.000000, true, "Pistola de alto calibre con gran potencia."},
    {"Escopeta", false, 0.0, 0.0, 0, 25, 349, 0.000000, 0.000000, 0.000000, 1.000000, true, "Arma devastadora a corta distancia."},
    {"Escopeta Recortada", false, 0.0, 0.0, 0, 26, 350, 0.000000, 0.000000, 0.000000, 1.000000, true, "Versión compacta con disparo amplio."},
    {"Escopeta de Combate", false, 0.0, 0.0, 0, 27, 351, 0.000000, 0.000000, 0.000000, 1.000000, true, "Escopeta automática para enfrentamientos intensos."},
    {"UZI", false, 0.0, 0.0, 0, 28, 352, 0.000000, 0.000000, 0.000000, 1.000000, true, "Subfusil ligero con alta cadencia de tiro."},
    {"MP5", false, 0.0, 0.0, 0, 29, 353, 0.000000, 0.000000, 0.000000, 1.000000, true, "Subfusil preciso y estable."},
    {"AK-47", false, 0.0, 0.0, 0, 30, 355, 0.000000, 0.000000, 0.000000, 1.000000, true, "Rifle de asalto potente y resistente."},
    {"M4", false, 0.0, 0.0, 0, 31, 356, 0.000000, 0.000000, 0.000000, 1.000000, true, "Rifle de asalto con buena precisión."},
    {"TEC-9", false, 0.0, 0.0, 0, 32, 372, 0.000000, 0.000000, 0.000000, 1.000000, true, "Pistola automática compacta."},
    {"Rifle", false, 0.0, 0.0, 0, 33, 357, 0.000000, 0.000000, 0.000000, 1.000000, true, "Rifle ideal para media y larga distancia."},
    {"Francotirador", false, 0.0, 0.0, 0, 34, 358, 0.000000, 0.000000, 0.000000, 1.000000, true, "Arma de precisión extrema a larga distancia."},
    {"Lanzacohetes", false, 0.0, 0.0, 0, 35, 359, 0.000000, 0.000000, 0.000000, 1.000000, true, "Arma pesada que dispara proyectiles explosivos."},
    {"Heatseeker", false, 0.0, 0.0, 0, 36, 360, 0.000000, 0.000000, 0.000000, 1.000000, true, "Lanzador con sistema de seguimiento de objetivos."},
    {"Lanzallamas", false, 0.0, 0.0, 0, 37, 361, 0.000000, 0.000000, 0.000000, 1.000000, true, "Proyecta fuego continuo a corta distancia."},
    {"Minigun", false, 0.0, 0.0, 0, 38, 362, 0.000000, 0.000000, 0.000000, 1.000000, true, "Ametralladora pesada de altísima cadencia."},
    {"Spray", false, 0.0, 0.0, 0, 41, 365, 0.000000, 0.000000, 0.000000, 1.000000, true, "Lata de pintura en aerosol."},
    {"Extintor", false, 0.0, 0.0, 0, 42, 366, 0.000000, 0.000000, 0.000000, 1.000000, true, "Herramienta para apagar incendios."},
    {"Camara", false, 0.0, 0.0, 0, 43, 367, 0.000000, 0.000000, 0.000000, 1.000000, false, "Permite capturar fotografías."},

    {"Piezas de mecanico", false, 0.0, 0.0, 0, 100, 3052, 0.0, 0.0, 0.0, 1.0, false, "Utilizadas por mecánicos para reparar y mejorar vehículos."},
    {"Semillas medicinales", false, 0.0, 0.0, 0, 100, 1579, 0.0, 0.0, 0.0, 1.0, false, "Semillas usadas para cultivar plantas medicinales."},
    {"Semillas de marihuana", false, 0.0, 0.0, 0, 100, 1578, 0.0, 0.0, 0.0, 1.0, true, "Semillas utilizadas para el cultivo de marihuana."},
    {"Semillas de coca", false, 0.0, 0.0, 0, 100, 1575, 0.0, 0.0, 0.0, 1.0, true, "Semillas utilizadas para el cultivo de crack."},
    {"Medicamentos", true, 0.0, 0.0, 0, 1000, 11736, 0.0, 0.0, 0.0, 1.0, false, "Ayuda a subir la salud de el mersonaje."},
    {"Marihuana", true, 0.0, 0.0, 0, 1000, 19896, 0.0, 0.0, 0.0, 1.0, true, "Puede alterar el estado del personaje y mejorar su salud."},
    {"Crack", true, 0.0, 0.0, 0, 1000, 19897, 0.0, 0.0, 0.0, 1.0, true, "Una droga con posibilidad de mejorar la resistencia de el persona."},
    {"Kit de reparacion", false, 0.0, 0.0, 0, 100, 19921, 0.0, 0.0, 0.0, 1.0, false, "Utilizable para reparar vehículos averiados lejos de el taller."},
    {"Botiquin médico", false, 0.0, 0.0, 0, 0, 11738, 0.0, 0.0, 0.0, 1.0, false, "Botiquin medico para atender personas abatidas sin un medico."}
};


new i_CurrentSlot[MAX_PLAYERS];
new bool: i_PlayerInventoryShow[MAX_PLAYERS];
new PlayerText: p_InventoryTd[MAX_PLAYERS][MAX_PLAYER_INVENTORY_TD] = {{PlayerText:INVALID_TEXT_DRAW, ...}, ...};