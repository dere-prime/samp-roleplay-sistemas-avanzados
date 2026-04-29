/* 

    NPCs Ambientales

*/

#include <YSI_Coding\y_hooks>

/* GLOBAL */
static const ambient_male_names[][] = {
    "Juan", "Pedro",
    "Luis", "Carlos",
    "Miguel", "Jose",
    "Ramon", "Diego",
    "Jorge", "Manuel",
    "Andres", "Fernando",
    "Ricardo", "Gael",
    "Alberto", "Roberto",
    "Hector", "Mario",
    "Victor", "Eduardo"
};
static const ambient_female_names[][] = {
    "Maria", "Ana",
    "Luisa", "Carla",
    "Micaela", "Josefa",
    "Ramona", "Daniela",
    "Jorgina", "Manuela",
    "Andrea", "Fernanda",
    "Rosa", "Gabriela",
    "Josefina", "Roberta",
    "Helena", "Mariana",
    "Victoria", "Eduarda"
};

static const ambient_male_allowed_skins[] = {
	34, 32, 30, 31, 35, 20, 15, 14, 95, 7, 14, 15, 17, 18, 19, 20, 21, 22, 23, 26, 
    28, 29, 30, 32, 33, 34, 36, 37, 43, 44, 45, 46, 47, 48, 51, 94, 95, 98, 101
};

static const ambient_female_allowed_skins[] = {
	9, 10, 12, 13, 38, 39, 40, 41, 88, 89, 11, 31, 53, 54, 55, 56,
    63, 64, 75, 85, 90, 91, 92, 93, 130, 131, 139, 140, 141, 150
};


/* Actores */
enum enum_ambient_actors {
	Float:pos_x,
	Float:pos_y,
	Float:pos_z,
	Float:facing_angle,
	bool:use_loop_animation,
    animlib[24],
    animname[24],
	actorid,
    bool:use_name,
	Text3D:text3d
}
static const ambient_actors[][enum_ambient_actors] = {
    /* Agricultor */
	{1546.2880, 14.8514, 24.1372, 190.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1541.4342, 13.9642, 24.1335, 190.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1540.5114, 17.9030, 24.1406, 10.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1547.0715, 19.0489, 24.1406, 10.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1544.8754, -9.5234, 22.0993, 116.6623, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1567.4199, 14.0110, 23.8400, 100.0177, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1541.9015, 41.9762, 24.6335, 72.3943, true, "BOMBER", "BOM_Plant_Loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1549.0917, 69.8873, 27.3601, 347.5179, true, "BOMBER", "BOM_Plant_Loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1561.4924, 38.6878, 24.1856, 200.6760, true, "BOMBER", "BOM_Plant_Loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1566.1051, 5.1371, 23.3925, 339.6467, true, "BOMBER", "BOM_Plant_Loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
	{1555.2244, 3.8109, 23.4094, 39.9075, true, "BOMBER", "BOM_Plant_Loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},

    // -- Fabrica de armas
    {-2157.0159, -249.3289, 36.5156, 0.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
    {-2160.7966, -249.3289, 36.5156, 0.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
    {-2164.0559, -249.3289, 36.5156, 0.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
    {-2167.8074, -249.3289, 36.5156, 0.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
    {-2171.2634, -249.3289, 36.5156, 0.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
    {-2174.8193, -249.3289, 36.5156, 0.0000, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},

    {-2147.6965, -250.3289, 36.5156, 270.6916, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
    {-2158.5415, -263.9911, 36.5156, 178.4141, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
    {-2182.7424, -262.7330, 40.7195, 271.7056, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID},
    {-2140.6104, -244.8539, 36.5156, 267.7398, true, "INT_SHOP", "shop_loop", INVALID_ACTOR_ID, true, Text3D:INVALID_STREAMER_ID}
};

/* Global Code */
hook OnScriptInit() {
	for (new i = 0; i != sizeof ambient_actors; i ++) {
        new gender = random(1);
        if (gender) {
            ambient_actors[i][actorid] = CreateDynamicActor(ambient_female_allowed_skins[random(sizeof(ambient_female_allowed_skins))], ambient_actors[i][pos_x], ambient_actors[i][pos_y], ambient_actors[i][pos_z], ambient_actors[i][facing_angle]);
        } else {
            ambient_actors[i][actorid] = CreateDynamicActor(ambient_male_allowed_skins[random(sizeof(ambient_male_allowed_skins))], ambient_actors[i][pos_x], ambient_actors[i][pos_y], ambient_actors[i][pos_z], ambient_actors[i][facing_angle]);
        }
		if (ambient_actors[i][use_loop_animation]) {
			ApplyDynamicActorAnimation(ambient_actors[i][actorid], ambient_actors[i][animlib], ambient_actors[i][animname], 4.1, 1, 1, 1, 0, 0);
			ApplyDynamicActorAnimation(ambient_actors[i][actorid], ambient_actors[i][animlib], ambient_actors[i][animname], 4.1, 1, 1, 1, 0, 0);
		}
        if (ambient_actors[i][use_name]) {
            if (gender) {
                ambient_actors[i][text3d] = CreateDynamic3DTextLabel(ambient_female_names[random(sizeof(ambient_female_names))], 0xCCCCCCCC, ambient_actors[i][pos_x], ambient_actors[i][pos_y], ambient_actors[i][pos_z] + 1.1, 10.0, .testlos = true, .worldid = -1, .interiorid = -1);
            } else {
                ambient_actors[i][text3d] = CreateDynamic3DTextLabel(ambient_male_names[random(sizeof(ambient_male_names))], 0xCCCCCCCC, ambient_actors[i][pos_x], ambient_actors[i][pos_y], ambient_actors[i][pos_z] + 1.1, 10.0, .testlos = true, .worldid = -1, .interiorid = -1);
            }
        }
    }
    return 1;
}