/* 

    Weapons Factory
        Prime Roleplay
    
    Mission
*/

#include <YSI_Coding\y_hooks>

#define wfm_MissionUpdater(%0,%1) \
    KillTimer(mission_info[%0][mission_updates_timer]); \
    mission_info[%0][mission_updates_timer]=SetTimerEx("_wfm_MissionUpdater",%1,false,"d",%0)

enum {
    WFM_VEHICLE_NONE,
    WFM_VEHICLE_FORKLIFT
}

enum wfm_objects_enum {
    wobjectid,
    wmodelid,

    Float: wx,
    Float: wy,
    Float: wz,

    Float: wrot_x,
    Float: wrot_y,
    Float: wrot_z
}

enum wfm_vehicles_info_enum {
    vmodelid,

    Float: vx,
    Float: vy,
    Float: vz,
    Float: vangle,

    bool: vstatic_veh, //Si se pone en false se podra usar el auto
    vtype
}

enum global_wfm_data { //Para definir actores y NPCS con el mismo enum.
    set, //actorid, npcid, etc
    extra,
    Float: x,
    Float: y,
    Float: z,
    Float:angle
}

enum wfm_vehicle_enum {
    bool: valid,
    bool: static_veh,
    type,
    pid
}

enum p_wfm_data {
    wfm_cinematic_vehicle,
    wfm_cinematic_npc_player,
    wfm_boss_npc
}

new
    wfm_objects[][wfm_objects_enum] = {
        {INVALID_STREAMER_ID, 12932, -531.149353, 2513.694091, 56.844841, 2.299999, -0.799999, 174.199890},
        {INVALID_STREAMER_ID, 2991, -528.583374, 2533.439208, 53.101757, 0.000000, 0.000000, 0.000000},
        {INVALID_STREAMER_ID, 2991, -528.585266, 2533.436767, 54.355636, 0.000000, 0.000000, 0.000000},
        {INVALID_STREAMER_ID, 19624, -528.930175, 2540.657714, 53.435665, 0.000000, 0.000000, 0.000000},
        {INVALID_STREAMER_ID, 19624, -527.738891, 2544.712158, 52.905628, 0.000000, 0.000000, 59.500007}
    },
    
    Float: wfm_static_actors[][global_wfm_data] = {
        {0, 113, -528.8569, 2540.3955, 53.5156, 277.1041},
        {0, 57, -533.1757, 2543.6409, 53.5156, 1.5795},
        {0, 68, -500.1123, 2539.5747, 53.5156, 351.1607}
    },

    /*
        NOTE: Me dio pereza poner animaciones y mapear las armas en wfm_objects como hice con los maletines, 
        asi que mejor creo npcs estaticos y les doy armas
    */
    Float: wfm_static_npc_weapons[][global_wfm_data] = {
        {0, 117, -508.7785, 2532.5425, 53.5156, 350.6838}, // npcconarma
        {0, 117, -504.6788, 2565.4563, 53.5156, 320.4364}, // npcconarma
        {0, 118, -525.1388, 2558.0176, 53.5703, 246.1990}, // npcconarma
        {0, 120, -540.6816, 2533.8423, 53.5584, 134.8184} // npcconarma
    },

    wfm_vehicles_info[][wfm_vehicles_info_enum] = {
        {530, -519.7207, 2532.7622, 53.2847, 40.0228, false, WFM_VEHICLE_FORKLIFT},
        //{578, -513.2048, 2554.4634, 54.0393, 177.7198, true, WFM_VEHICLE_NONE} //NOTE: espero que el NPC se estacione y se quede, entonces no necesito crearlo en la escena
        {593, -531.2015, 2542.5239, 53.9906, 269.0933, true, WFM_VEHICLE_NONE},
        {515, -501.9819, 2531.8491, 54.6424, 19.9361, true, WFM_VEHICLE_NONE}
    },

    wfm_vehicle[MAX_VEHICLES][wfm_vehicle_enum],

    Float: wfm_truck_route_path[13][3] = {
        {-326.3637, 2517.7290, 35.5327}, // dft
        {-336.4721, 2516.7639, 35.9706}, // dft
        {-341.0236, 2518.4956, 36.2209}, // dft
        {-348.8148, 2525.6519, 36.6159}, // dft
        {-357.3880, 2537.8545, 36.7773}, // dft
        {-374.4435, 2561.6680, 40.6035}, // dft
        {-394.5020, 2586.0212, 42.1412}, // dft
        {-405.7248, 2592.4773, 43.3713}, // dft
        {-420.9367, 2594.5298, 44.7062}, // dft
        {-458.7094, 2595.1567, 50.3041}, // dft
        {-498.2247, 2595.3362, 54.1298}, // dft
        {-512.0184, 2584.2351, 54.0401}, // dft
        {-513.2048, 2554.4634, 54.0393} // dft y ultima posicion
    },
    wfm_truck_path,

    Float: wfm_player_onfoot_path[4][3] = {
        {-510.8965, 2550.9885, 53.4154},
        {-511.0461, 2547.8762, 53.4154},
        {-513.7802, 2547.8770, 53.4141},
        {-515.9008, 2548.0186, 53.4141}
    },
    wfm_player_path,

    Float: wfm_npc_dialog_boss_path[7][3] = {
        {-527.3835, 2544.3635, 53.5156}, // npcpathtraficante57
        {-526.1741, 2544.9995, 53.5156}, // path
        {-524.3531, 2546.2507, 53.5156}, // path
        {-522.2986, 2547.0034, 53.5156}, // path
        {-519.8614, 2546.9001, 53.5156}, // path
        {-517.5948, 2546.8047, 53.5156}, // path
        {-516.2140, 2548.7153, 53.4141} // path
    },
    Float: wfm_npc_boss_return_path[7][3] = {
        {-517.1718, 2548.0972, 53.4141},
        {-519.1637, 2546.8970, 53.5156},
        {-521.6069, 2546.9639, 53.5156},
        {-525.5585, 2547.1040, 53.5156},
        {-529.0993, 2546.9163, 53.5156},
        {-529.2418, 2545.9800, 53.5156},
        {-528.2991, 2544.7190, 53.5156}
    },
    wfm_npc_boss_path,
    wfm_npc_return_path,

    p_wfm[MAX_PLAYERS][p_wfm_data]
;

/*

    PREPARATIVOS

*/

createWfMissionStage(const playerid, const player_worldid) {
    for (new i = 0; i != sizeof(wfm_objects); i ++) {
        wfm_objects[i][wobjectid] = CreateDynamicObject(
            wfm_objects[i][wmodelid], wfm_objects[i][wx], wfm_objects[i][wy], wfm_objects[i][wz], wfm_objects[i][wrot_x], wfm_objects[i][wrot_y], wfm_objects[i][wrot_z], 
            player_worldid, 0
        );
    }
    for (new i = 0; i != sizeof(wfm_static_actors); i ++) {
        wfm_static_actors[i][set] = CreateDynamicActor(
            wfm_static_actors[i][extra], wfm_static_actors[i][x], wfm_static_actors[i][y], wfm_static_actors[i][z], wfm_static_actors[i][angle], .worldid = player_worldid, .interiorid = 0
        );
    }
    for (new i = 0; i != sizeof(wfm_static_npc_weapons); i ++) {
        wfm_static_npc_weapons[i][set] = FCNPC_Createf("%d_wfp_wpn_%d", PI[playerid][pi_ID], i);
        FCNPC_Spawn(wfm_static_npc_weapons[i][set], wfm_static_npc_weapons[i][extra], wfm_static_npc_weapons[i][x], wfm_static_npc_weapons[i][y], wfm_static_npc_weapons[i][z]);
        FCNPC_SetAngle(wfm_static_npc_weapons[i][set], wfm_static_npc_weapons[i][angle]);
        FCNPC_SetVirtualWorld(wfm_static_npc_weapons[i][set], player_worldid);
        FCNPC_SetWeapon(wfm_static_npc_weapons[i][set], 30);
        FCNPC_SetInvulnerable(wfm_static_npc_weapons[i][set], true);
    }

    p_wfm[playerid][wfm_boss_npc] = FCNPC_Createf("%d_wfp_npc_boss", PI[playerid][pi_ID]);
    FCNPC_Spawn(p_wfm[playerid][wfm_boss_npc], 57, wfm_npc_dialog_boss_path[0][0], wfm_npc_dialog_boss_path[0][1], wfm_npc_dialog_boss_path[0][2]);
    FCNPC_SetAngle(p_wfm[playerid][wfm_boss_npc], 313.4510);
    FCNPC_SetVirtualWorld(p_wfm[playerid][wfm_boss_npc], player_worldid);
    FCNPC_SetWeapon(p_wfm[playerid][wfm_boss_npc], 27);
    FCNPC_SetInvulnerable(p_wfm[playerid][wfm_boss_npc], true);

    new vehicleid;
    for (new i = 0; i != sizeof(wfm_vehicles_info); i ++) {
        vehicleid = createMissionVehicle(
            wfm_vehicles_info[i][vmodelid], wfm_vehicles_info[i][vx], wfm_vehicles_info[i][vy], wfm_vehicles_info[i][vz], wfm_vehicles_info[i][vangle],
            player_worldid, 0, 1, 1, wfm_vehicles_info[i][vstatic_veh]
        );
        if (wfm_vehicles_info[i][vstatic_veh]) SetVehicleParamsEx(vehicleid, 0, 0, 0, 1, 0, 0, 0);

        wfm_vehicle[vehicleid][valid] = true;
        wfm_vehicle[vehicleid][static_veh] = wfm_vehicles_info[i][vstatic_veh];
        wfm_vehicle[vehicleid][type] = wfm_vehicles_info[i][vtype];
        wfm_vehicle[vehicleid][pid] = playerid;
    }
    return 1;
}

hook OnScriptInit() {
    wfm_truck_path = FCNPC_CreateMovePath();
	FCNPC_AddPointsToMovePath(wfm_truck_path, wfm_truck_route_path);
    wfm_player_path = FCNPC_CreateMovePath();
	FCNPC_AddPointsToMovePath(wfm_player_path, wfm_player_onfoot_path);
    wfm_npc_boss_path = FCNPC_CreateMovePath();
	FCNPC_AddPointsToMovePath(wfm_npc_boss_path, wfm_npc_dialog_boss_path);
    wfm_npc_return_path = FCNPC_CreateMovePath();
	FCNPC_AddPointsToMovePath(wfm_npc_return_path, wfm_npc_boss_return_path);
}

/*

    Mision

*/

Mission:factorySearchWp(playerid, index) {
    ClearPlayerChat(playerid);

    IsPlayerMission(playerid) = true;
    PlayerMissionId(playerid) = index;
    PlayerMissionStep(playerid) = MISSION_STEP_CINEMATIC;
    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
    PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 15;

    MissionUpdates(playerid) = 0;

    PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD] = playerid + PI[playerid][pi_ID] + random(99);
    SetPlayerVirtualWorld(playerid, PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
    createWfMissionStage(playerid, PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
    
    new WeaponsFactory_Cargo_Vehicleid = GetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid");
    if (WeaponsFactory_Cargo_Vehicleid != INVALID_VEHICLE_ID) {
        if (wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid]) {
            wf_is_cargo_vehicle[WeaponsFactory_Cargo_Vehicleid] = false;
            wf_cargo_vehicle_driver[WeaponsFactory_Cargo_Vehicleid] = -1;
            DestroyVehicleEx(WeaponsFactory_Cargo_Vehicleid);
            SetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid", INVALID_VEHICLE_ID);
        }
    }

    TogglePlayerSpectatingEx(playerid, true);

    /* Stage 1 */
    p_wfm[playerid][wfm_cinematic_vehicle] = CreateVehicle(578, wfm_truck_route_path[0][0], wfm_truck_route_path[0][1], wfm_truck_route_path[0][2], 180.0, 1, 1, -1, false);
    SetVehicleParamsEx(p_wfm[playerid][wfm_cinematic_vehicle], 1, 0, 0, 1, 0, 0, 0);
    SetVehicleVirtualWorld(p_wfm[playerid][wfm_cinematic_vehicle], PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
    p_wfm[playerid][wfm_cinematic_npc_player] = FCNPC_Createf("%d_wfm_player", PI[playerid][pi_ID]);
    FCNPC_Spawn(p_wfm[playerid][wfm_cinematic_npc_player], GetPlayerSkin(playerid), wfm_truck_route_path[0][0], wfm_truck_route_path[0][1], wfm_truck_route_path[0][2]);
    FCNPC_SetVirtualWorld(p_wfm[playerid][wfm_cinematic_npc_player], PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
    FCNPC_PutInVehicle(p_wfm[playerid][wfm_cinematic_npc_player], p_wfm[playerid][wfm_cinematic_vehicle], 0);

    EaseShowPlayerBlackScreen(playerid);
    wfm_MissionUpdater(playerid, 2000);
    return 1;
}

forward _wfm_MissionUpdater(playerid);
public _wfm_MissionUpdater(playerid) {
    switch (MissionUpdates(playerid)) {
        case 0: {
            EaseHidePlayerBlackScreen(playerid, 1400);
            InterpolateCameraPos(playerid, -354.251861, 2517.010986, 35.836589, -354.251861, 2517.010986, 35.836589, 1000, CAMERA_CUT);
            InterpolateCameraLookAt(playerid, -341.691802, 2519.890625, 35.516620, -341.691802, 2519.890625, 35.516620, 1000, CAMERA_CUT);
            FCNPC_GoByMovePath(p_wfm[playerid][wfm_cinematic_npc_player], wfm_truck_path, .type = FCNPC_MOVE_TYPE_DRIVE);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 3000);
        }
        case 1: {
            EaseShowPlayerBlackScreen(playerid);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 1500);
        }
        case 2: {
            EaseHidePlayerBlackScreen(playerid, 800);
            InterpolateCameraPos(playerid, -413.476562, 2578.358398, 47.856910, -413.476562, 2578.358398, 47.856910, 1000, CAMERA_CUT);
            InterpolateCameraLookAt(playerid, -391.306701, 2580.548339, 40.606914, -391.306701, 2580.548339, 40.606914, 1000, CAMERA_CUT);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 5000);
        }
        case 3: {
            EaseShowPlayerBlackScreen(playerid);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 1700);
        }
        case 4: {
            EaseHidePlayerBlackScreen(playerid, 800);
            InterpolateCameraPos(playerid, -503.654388, 2557.681884, 61.514053, -503.654388, 2557.681884, 61.514053, 1000, CAMERA_CUT);
            InterpolateCameraLookAt(playerid, -518.184814, 2538.479248, 53.454051, -518.184814, 2538.479248, 53.454051, 1000, CAMERA_CUT);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 4000);
        }
        case 5: {
            EaseShowPlayerBlackScreen(playerid);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 2000);
        }
        case 6: {
            InterpolateCameraPos(playerid, -505.468933, 2531.254394, 58.185619, -505.468933, 2531.254394, 58.185619, 1000, CAMERA_CUT);
            InterpolateCameraLookAt(playerid, -517.128662, 2547.982666, 52.965633, -517.128662, 2547.982666, 52.965633, 1000, CAMERA_CUT);

            FCNPC_GoByMovePath(p_wfm[playerid][wfm_boss_npc], wfm_npc_boss_path, .type = FCNPC_MOVE_TYPE_WALK);

            EaseHidePlayerBlackScreen(playerid);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 3000);
        }
        case 7: {
            FCNPC_ExitVehicle(p_wfm[playerid][wfm_cinematic_npc_player]);
            ShowPlayerMissionDialog(playerid, PI[playerid][pi_SKIN], PI[playerid][pi_NAME], "Siento que algun dia estos sujetos van a matarme, pero bueno ya vere como me las arreglo. A trabajar!!!");
            KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, false);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 2600);
        }
        case 8: {
            FCNPC_GoByMovePath(p_wfm[playerid][wfm_cinematic_npc_player], wfm_player_path, .type = FCNPC_MOVE_TYPE_WALK);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 7400);
        }
        case 9: {
            ShowPlayerMissionDialog(playerid, 57, "Jefe de la mafia", "Muevete, no tengo mucho tiempo tengo que vender a mas personas en otros lugares, trata de no alejarte de la zona ni de hacer algo raro. Estas advertido.");
            KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 6000, false, "ib", playerid, false);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 8000);
        }
        case 10: {
            EaseShowPlayerBlackScreen(playerid);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 1400);
        }
        case 11: {
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 5000);

            TogglePlayerSpectatingEx(playerid, false);

            new Float: px, Float: py, Float: pz, Float: pang;
            FCNPC_GetPosition(p_wfm[playerid][wfm_cinematic_npc_player], px, py, pz);
            pang = FCNPC_GetAngle(p_wfm[playerid][wfm_cinematic_npc_player]);
            FCNPC_Destroy(p_wfm[playerid][wfm_cinematic_npc_player]);

            PlayerMissionStep(playerid) = MISSION_STEP_PLAYABLE;

            SetPlayerPosEx(playerid, px, py, pz, pang, 0, PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
        }
        case 12: {
            EaseHidePlayerBlackScreen(playerid);
            MissionUpdates(playerid) ++;

            FCNPC_GoByMovePath(p_wfm[playerid][wfm_boss_npc], wfm_npc_return_path, .type = FCNPC_MOVE_TYPE_WALK);
            
            ShowPlayerMissionDialog(playerid, 18631, "Informacion", "Subete al montacargas, agarra las cajas y llevalas al camion hasta completarlo. Una vez terminado, subete al camion y alejate de la zona.");
            KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 3000, false, "ib", playerid, false);
        }
    }
    return 1;
}

//Funcion monta cargas

/* 

    IMPORTANT: Funciones de limpieza

*/

clearWfMissionVehicle(const vehicleid) {
    wfm_vehicle[vehicleid][valid] = false;
    wfm_vehicle[vehicleid][static_veh] = false;
    wfm_vehicle[vehicleid][type] = 0;
    wfm_vehicle[vehicleid][pid] = 0;
    static const vfm_vehicle_data[wfm_vehicle_enum]; 
    wfm_vehicle[vehicleid] = vfm_vehicle_data;
    return 1;
}

/*

    Callbacks y otros

*/

hook OnVehicleSpawn(vehicleid) {
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleDeath(vehicleid, killerid) {
    return Y_HOOKS_CONTINUE_RETURN_1;
}
