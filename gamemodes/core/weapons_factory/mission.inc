/* 

    Weapons Factory
        Prime Roleplay
    
    Mission
*/

#include <YSI_Coding\y_hooks>

/* 

    Macros

*/

#define MAX_WFM_OBJECTS 15

#define wfm_MissionUpdater(%0,%1) \
    KillTimer(mission_info[%0][mission_updates_timer]); \
    mission_info[%0][mission_updates_timer]=SetTimerEx("_wfm_MissionUpdater",%1,false,"d",%0)

/* 

    Enums

*/

enum {
    WFM_VEHICLE_NONE,
    WFM_VEHICLE_FORKLIFT
}
enum wfm_objects_enum {
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
enum global_wfm_data {
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
    wfm_boss_npc,
    wfm_interruption_counter,
    wfm_mission_objects[MAX_WFM_OBJECTS],
    wfm_mission_actor[MAX_WFM_OBJECTS],
    wfm_mission_npcs[MAX_WFM_OBJECTS],
    wfm_mission_cp,
    wfm_mission_cp_state, //0 - buscar / 1 - llevar al dft
    wfm_mission_progress, //10 recorridos de cajas
    wfm_area_leave_timer
}

/*

    Variables

*/

new
    wfm_objects[][wfm_objects_enum] = {
        {12932, -531.149353, 2513.694091, 56.844841, 2.299999, -0.799999, 174.199890},
        {2991, -528.583374, 2533.439208, 53.101757, 0.000000, 0.000000, 0.000000},
        {2991, -528.585266, 2533.436767, 54.355636, 0.000000, 0.000000, 0.000000},
        {19624, -528.930175, 2540.657714, 53.435665, 0.000000, 0.000000, 0.000000},
        {19624, -527.738891, 2544.712158, 52.905628, 0.000000, 0.000000, 59.500007}
    },
    Float: wfm_static_actors[][global_wfm_data] = {
        {113, -528.8569, 2540.3955, 53.5156, 277.1041},
        {57, -533.1757, 2543.6409, 53.5156, 1.5795},
        {68, -500.1123, 2539.5747, 53.5156, 351.1607}
    },
    Float: wfm_static_npc_weapons[][global_wfm_data] = { //Solo son estaticos pero tienen armas en sus manos para mas ambiente
        {117, -508.7785, 2532.5425, 53.5156, 350.6838}, // npcconarma
        {117, -504.6788, 2565.4563, 53.5156, 320.4364}, // npcconarma
        {118, -525.1388, 2558.0176, 53.5703, 246.1990}, // npcconarma
        {120, -540.6816, 2533.8423, 53.5584, 134.8184} // npcconarma
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
    Float: wfm_player_onfoot_path[6][3] = {
        {-509.4834, 2555.2451, 53.4154}, // path
        {-509.5096, 2552.6106, 53.4154}, // path
        {-511.1872, 2551.4043, 53.4154}, // path
        {-513.6643, 2550.2781, 53.4141}, // path
        {-514.5038, 2550.3826, 53.4141}, // path
        {-515.5588, 2549.4897, 53.4141} // path
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
    p_wfm[MAX_PLAYERS][p_wfm_data],
    Float: wfm_forklift_cargo_cp[3] = {-527.2520, 2534.7957, 53.5170},
    Float: wfm_dft_cargo_cp[3] = {-512.6239, 2564.9097, 53.1781},
    wf_mission_zone
;

/*

    PREPARATIVOS

*/

createWfMissionStage(const playerid, const player_worldid) {
    for (new i = 0; i != sizeof(wfm_objects); i ++) {
        p_wfm[playerid][wfm_mission_objects][i] = CreateDynamicObject(
            wfm_objects[i][wmodelid], wfm_objects[i][wx], wfm_objects[i][wy], wfm_objects[i][wz], wfm_objects[i][wrot_x], wfm_objects[i][wrot_y], wfm_objects[i][wrot_z], 
            player_worldid, 0
        );
    }
    for (new i = 0; i != sizeof(wfm_static_actors); i ++) {
        p_wfm[playerid][wfm_mission_actor][i] = CreateDynamicActor(
            wfm_static_actors[i][extra], wfm_static_actors[i][x], wfm_static_actors[i][y], wfm_static_actors[i][z], wfm_static_actors[i][angle], .worldid = player_worldid, .interiorid = 0
        );
    }
    for (new i = 0; i != sizeof(wfm_static_npc_weapons); i ++) {
        p_wfm[playerid][wfm_mission_npcs][i] = FCNPC_Createf("%d_wfp_wpn_%d", PI[playerid][pi_ID], i);
        FCNPC_Spawn(p_wfm[playerid][wfm_mission_npcs][i], wfm_static_npc_weapons[i][extra], wfm_static_npc_weapons[i][x], wfm_static_npc_weapons[i][y], wfm_static_npc_weapons[i][z]);
        FCNPC_SetAngle(p_wfm[playerid][wfm_mission_npcs][i], wfm_static_npc_weapons[i][angle]);
        FCNPC_SetVirtualWorld(p_wfm[playerid][wfm_mission_npcs][i], player_worldid);
        FCNPC_SetWeapon(p_wfm[playerid][wfm_mission_npcs][i], 30);
        FCNPC_SetInvulnerable(p_wfm[playerid][wfm_mission_npcs][i], true);
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
        GLOBAL_VEHICLES[vehicleid][gb_vehicle_VALID] = true;
        if (wfm_vehicles_info[i][vstatic_veh]) SetVehicleParamsEx(vehicleid, 0, 0, 0, 1, 0, 0, 0);

        wfm_vehicle[vehicleid][valid] = true;
        wfm_vehicle[vehicleid][static_veh] = wfm_vehicles_info[i][vstatic_veh];
        wfm_vehicle[vehicleid][type] = wfm_vehicles_info[i][vtype];
        wfm_vehicle[vehicleid][pid] = playerid;
    }
    return 1;
}

/*

    FUNCIONES DE LA MISION PRINCIPAL

*/

Mission:factorySearchWp(playerid, index) {
    ClearPlayerChat(playerid);

    IsPlayerMission(playerid) = true;
    PlayerMissionId(playerid) = index;
    PlayerMissionStep(playerid) = MISSION_STEP_CINEMATIC;
    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
    PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 15;

    MissionUpdates(playerid) = 0;
    p_wfm[playerid][wfm_interruption_counter] = 0;

    p_wfm[playerid][wfm_mission_cp_state] = 0;

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

    ShowPlayerNotification(playerid, "Pausar el juego u otras acciones no cancelaran la cinematica, ya que se ejecuta del lado del servidor. Sin embargo, si abandonas la partida o no completas la mision, esta se cancelara por completo.");

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

            EaseHidePlayerBlackScreen(playerid, 1000);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 3000);
        }
        case 7: {
            FCNPC_ExitVehicle(p_wfm[playerid][wfm_cinematic_npc_player]);
            ShowPlayerMissionDialog(playerid, PI[playerid][pi_SKIN], PI[playerid][pi_NAME], "Siento que algun dia estos sujetos van a matarme, pero bueno ya vere como me las arreglo. A trabajar!!!");
            KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, false);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 900);
        }
        case 8: {
            FCNPC_SetPosition(p_wfm[playerid][wfm_cinematic_npc_player], wfm_player_onfoot_path[0][0], wfm_player_onfoot_path[0][1], wfm_player_onfoot_path[0][2]);        
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 2600);
        }
        case 9: {
            FCNPC_GoByMovePath(p_wfm[playerid][wfm_cinematic_npc_player], wfm_player_path, .type = FCNPC_MOVE_TYPE_WALK);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 7400);
        }
        case 10: {
            ShowPlayerMissionDialog(playerid, 57, "Jefe de la mafia", "Muevete, no tengo mucho tiempo tengo que vender a mas personas en otros lugares, trata de no alejarte de la zona ni de hacer algo raro. Estas advertido.");
            KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 6000, false, "ib", playerid, false);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 8000);
        }
        case 11: {
            EaseShowPlayerBlackScreen(playerid);
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 1400);
        }
        case 12: {
            MissionUpdates(playerid) ++;
            wfm_MissionUpdater(playerid, 5000);

            PlayerMissionStep(playerid) = MISSION_STEP_PLAYABLE;
            TogglePlayerSpectatingEx(playerid, false);

            new Float: px, Float: py, Float: pz, Float: pang;
            FCNPC_GetPosition(p_wfm[playerid][wfm_cinematic_npc_player], px, py, pz);
            pang = FCNPC_GetAngle(p_wfm[playerid][wfm_cinematic_npc_player]);
            FCNPC_Destroy(p_wfm[playerid][wfm_cinematic_npc_player]);

            SetPlayerPosEx(playerid, px, py, pz, pang, 0, PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
        }
        case 13: {
            EaseHidePlayerBlackScreen(playerid, 1000);
            MissionUpdates(playerid) ++;

            FCNPC_GoByMovePath(p_wfm[playerid][wfm_boss_npc], wfm_npc_return_path, .type = FCNPC_MOVE_TYPE_WALK);
            
            ShowPlayerMissionDialog(playerid, 18631, "Informacion", "Subete al montacargas, agarra las cajas y llevalas al camion hasta completarlo. Una vez terminado, subete al camion y alejate de la zona.");
            KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
            PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 3000, false, "ib", playerid, false);
        }
    }
    return 1;
}
hook OnPlayerLeaveDynamicArea(playerid, areaid) {
    if (!mission_info[playerid][mission_current]) return Y_HOOKS_CONTINUE_RETURN_1;
    if (mission_info[playerid][mission_id] != MISSION_WEAPONS_FACTORY) return Y_HOOKS_CONTINUE_RETURN_1;
    if (mission_info[playerid][mission_step] != MISSION_STEP_PLAYABLE) return Y_HOOKS_CONTINUE_RETURN_1;

	new info[2];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);

    if (info[0] == AREA_TYPE_NONE) {
        if (areaid == wf_mission_zone) {
            KillTimer(p_wfm[playerid][wfm_area_leave_timer]);
            p_wfm[playerid][wfm_area_leave_timer] = SetTimerEx("wfMissionAreaLeaveTimeOut", 15000, false, "i", playerid);

            ShowPlayerNotification(playerid, "Has salido de la zona de la mision, si no regresas en ~y~15 segundos~w~~h~ la mision sera cancelada.");
            ShowPlayerNotification(playerid, "Has salido de la zona de la mision, si no regresas en ~y~15 segundos~w~~h~ la mision sera cancelada.");
            ShowPlayerNotification(playerid, "Has salido de la zona de la mision, si no regresas en ~y~15 segundos~w~~h~ la mision sera cancelada.");
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerEnterDynamicArea(playerid, areaid) {
	new info[2];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, info);

    if (info[0] == AREA_TYPE_NONE) {
        if (areaid == wf_mission_zone) {
            KillTimer(p_wfm[playerid][wfm_area_leave_timer]);
            p_wfm[playerid][wfm_area_leave_timer] = -1;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
forward wfMissionAreaLeaveTimeOut(playerid);
public wfMissionAreaLeaveTimeOut(playerid) {
    if (!mission_info[playerid][mission_current]) return 0;
    if (mission_info[playerid][mission_id] != MISSION_WEAPONS_FACTORY) return 0;

    ShowPlayerNotification(playerid, "Has tardado en volver al lugar de la mision por lo que esta se ha cancelado.");
    cancelWFMission(playerid);
    return 1;
}
setPlayerLiftCheckpoint(const playerid) {
    if (!mission_info[playerid][mission_current]) return 0;

    if (IsValidDynamicCP(p_wfm[playerid][wfm_mission_cp])) {
        DestroyDynamicCP(p_wfm[playerid][wfm_mission_cp]);
    }

    switch (p_wfm[playerid][wfm_mission_cp_state]) {
        case 0: {
            p_wfm[playerid][wfm_mission_cp] = CreateDynamicCP(wfm_forklift_cargo_cp[0], wfm_forklift_cargo_cp[1], wfm_forklift_cargo_cp[2], 3.0, PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD], 0, playerid, 9999999999.0);
            new info[1];
            info[0] = CHECKPOINT_TYPE_WF_FORKLIFT;
            Streamer_SetArrayData(STREAMER_TYPE_CP, p_wfm[playerid][wfm_mission_cp], E_STREAMER_EXTRA_ID, info);
        }
        case 1: {
            p_wfm[playerid][wfm_mission_cp] = CreateDynamicCP(wfm_dft_cargo_cp[0], wfm_dft_cargo_cp[1], wfm_dft_cargo_cp[2], 3.0, PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD], 0, playerid, 9999999999.0);
            new info[1];
            info[0] = CHECKPOINT_TYPE_WF_FORKLIFT;
            Streamer_SetArrayData(STREAMER_TYPE_CP, p_wfm[playerid][wfm_mission_cp], E_STREAMER_EXTRA_ID, info);
        }
    }
    return 1;
}

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
clearWfMissionPlayer(const playerid) {
    static const p_wfm_info[p_wfm_data]; 
    p_wfm[playerid] = p_wfm_info;

    p_wfm[playerid][wfm_cinematic_vehicle] = INVALID_VEHICLE_ID;
    p_wfm[playerid][wfm_cinematic_npc_player] = INVALID_PLAYER_ID;
    p_wfm[playerid][wfm_boss_npc] = INVALID_PLAYER_ID;
    p_wfm[playerid][wfm_interruption_counter] = 0;
    p_wfm[playerid][wfm_mission_cp] = INVALID_STREAMER_ID;
    p_wfm[playerid][wfm_mission_cp_state] = 0;
    p_wfm[playerid][wfm_mission_progress] = 0;
    return 1;
}
destroyWfMissionVehicles(const playerid) {
    foreach (new vehicleid : Vehicle) {
        if (!wfm_vehicle[vehicleid][valid]) continue;
        if (wfm_vehicle[vehicleid][pid] != playerid) continue;

        clearWfMissionVehicle(vehicleid);
        DestroyVehicleEx(vehicleid);
    }

    if (Vehicle_IsValid(p_wfm[playerid][wfm_cinematic_vehicle])) {
        DestroyVehicle(p_wfm[playerid][wfm_cinematic_vehicle]);
    }
    return 1;
}
destroyWfMissionNPCs(const playerid) {
    if (FCNPC_IsValid(p_wfm[playerid][wfm_boss_npc])) {
        FCNPC_Destroy(p_wfm[playerid][wfm_boss_npc]);
    }
    if (FCNPC_IsValid(p_wfm[playerid][wfm_cinematic_npc_player])) {
        FCNPC_Destroy(p_wfm[playerid][wfm_cinematic_npc_player]);
    }

    for (new i = 0; i != MAX_WFM_OBJECTS; i ++) {
        if (FCNPC_IsValid(p_wfm[playerid][wfm_mission_npcs][i])) {
            FCNPC_Destroy(p_wfm[playerid][wfm_mission_npcs][i]);
        }
    }
    return 1;
}
destroyWfMissionObjects(const playerid) {
    for (new i = 0; i != MAX_WFM_OBJECTS; i ++) {
        if (IsValidDynamicObject(p_wfm[playerid][wfm_mission_objects][i])) {
            DestroyDynamicObject(p_wfm[playerid][wfm_mission_objects][i]);
        }
    }
    return 1;
}
destroyWfMissionActors(const playerid) {
    for (new i = 0; i != MAX_WFM_OBJECTS; i ++) {
        if (IsValidDynamicActor(p_wfm[playerid][wfm_mission_actor][i])) {
            DestroyDynamicActor(p_wfm[playerid][wfm_mission_actor][i]);
        }
    }
    return 1;
}
cancelWFMission(const playerid) {
    destroyWfMissionVehicles(playerid);
    destroyWfMissionActors(playerid);
    destroyWfMissionObjects(playerid);
    destroyWfMissionNPCs(playerid);
    clearWfMissionPlayer(playerid);

    if (IsValidDynamicCP(p_wfm[playerid][wfm_mission_cp])) {
        DestroyDynamicCP(p_wfm[playerid][wfm_mission_cp]);
    }

    mission_info[playerid][mission_current] = false;
    mission_info[playerid][mission_id] = -1;
    PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS] = true;
    SetPlayerHud(playerid);
    TogglePlayerControllableEx(playerid, true);
    HidePlayerMissionDialog(playerid, false);
    TogglePlayerSpectatingEx(playerid, false);

    PI[playerid][pi_STATE] = ROLEPLAY_STATE_NORMAL;
    PI[playerid][pi_LOCAL_INTERIOR] = 0;
    PI[playerid][pi_POS_X] = -354.251861;
    PI[playerid][pi_POS_Y] = 2517.010986;
    PI[playerid][pi_POS_Z] = 35.836589;
    PI[playerid][pi_ANGLE] = frandom(360.0, 0.0, 2);
    PI[playerid][pi_INTERIOR] = 0;

    SetPlayerPosEx(playerid, PI[playerid][pi_POS_X], PI[playerid][pi_POS_Y], PI[playerid][pi_POS_Z], PI[playerid][pi_ANGLE], PI[playerid][pi_INTERIOR], 0);

    SetCameraBehindPlayer(playerid);
    SetPlayerVirtualWorld(playerid, 0);
    ClearAnimations(playerid);

	ShowPlayerBlackScreen(playerid);
    SetTimerEx("HidePlayerBlackScreen", 2000, false, "d", playerid);

    SetTimerEx("pc_cmd_sentarse", 500, false, "d", playerid);

    ShowPlayerNotification(playerid, "La mision de la busqueda de las piezas de armas ha sido interrumpida.");
    return 1;
}

/* 

    Callbacks definidos

*/

forward FinishWFMission(playerid, vehicleid);
public FinishWFMission(playerid, vehicleid) {
    player_mission[playerid][MISSION_WEAPONS_FACTORY][m_completed] = true;

    //PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_BLACK_SCREEN]);

    destroyWfMissionVehicles(playerid);
    destroyWfMissionActors(playerid);
    destroyWfMissionObjects(playerid);
    destroyWfMissionNPCs(playerid);
    clearWfMissionPlayer(playerid);

    if (IsValidDynamicCP(p_wfm[playerid][wfm_mission_cp])) {
        DestroyDynamicCP(p_wfm[playerid][wfm_mission_cp]);
    }

    mission_info[playerid][mission_current] = false;
    mission_info[playerid][mission_id] = -1;
    mission_info[playerid][mission_step] = MISSION_STEP_NONE;
    PLAYER_TEMP[playerid][pt_HUD_TEXTDRAWS] = true;
    SetPlayerHud(playerid);
    TogglePlayerControllableEx(playerid, true);
    HidePlayerMissionDialog(playerid, false);
    TogglePlayerSpectatingEx(playerid, false);

    if (PI[playerid][pi_WANTED_LEVEL] < 2) SetPlayerWantedLevelEx(playerid, 2);

    SetCameraBehindPlayer(playerid);
    SetPlayerVirtualWorld(playerid, 0);
    SetVehicleVirtualWorldEx(vehicleid, 0);

    if (IsValidDynamicObject(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT])) DestroyDynamicObject(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT]);
    GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT] = CreateDynamicObject(2991,0.0,0.0,-1000.0,0.0,0.0,0.0);
    AttachDynamicObjectToVehicle(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT], vehicleid, 0.000, 0.000, 0.360, 0.000, 0.000, 90.000);

    PutPlayerInVehicleEx(playerid, vehicleid, 0);

    ShowPlayerNotification(playerid, "~y~¡Felicidades!~w~~h~~n~has finalizado la etapa de la mision de la busqueda de las piezas de armas.~n~~n~Regresa a la fabrica para recibir tu paga y entregar las ~b~piezas de armas~w~~h~.");
    SetWeaponsFactoryCheckpoint(playerid);

    EaseHidePlayerBlackScreen(playerid, 1400);
    return 1;
}

/*

    Callbacks y otros

*/

hook OnScriptInit() {
    wfm_truck_path = FCNPC_CreateMovePath();
	FCNPC_AddPointsToMovePath(wfm_truck_path, wfm_truck_route_path);
    wfm_player_path = FCNPC_CreateMovePath();
	FCNPC_AddPointsToMovePath(wfm_player_path, wfm_player_onfoot_path);
    wfm_npc_boss_path = FCNPC_CreateMovePath();
	FCNPC_AddPointsToMovePath(wfm_npc_boss_path, wfm_npc_dialog_boss_path);
    wfm_npc_return_path = FCNPC_CreateMovePath();
	FCNPC_AddPointsToMovePath(wfm_npc_return_path, wfm_npc_boss_return_path);

    wf_mission_zone = CreateDynamicRectangle( -544.5, 2530.0, -489.5, 2585.0);
}
hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) {
    if (IsPlayerMission(playerid) == true) {
        if (PlayerMissionId(playerid) == MISSION_WEAPONS_FACTORY) {
            if (!ispassenger) {
                if (wf_is_cargo_vehicle[vehicleid]) {
                    EaseShowPlayerBlackScreen(playerid);
                    mission_info[playerid][mission_step] = MISSION_STEP_CINEMATIC; //Bloquear funciones tempororalmente
                    SetTimerEx("FinishWFMission", 4000, false, "dd", playerid, vehicleid);
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerStateChange(playerid, newstate, oldstate) {
    if (newstate == PLAYER_STATE_DRIVER) {
        if (IsPlayerMission(playerid) == true) {
            if (PlayerMissionId(playerid) == MISSION_WEAPONS_FACTORY) {
                new vehicleid = GetPlayerVehicleID(playerid);
                PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID] = vehicleid;
                if (wfm_vehicle[vehicleid][valid] && !wfm_vehicle[vehicleid][static_veh] && wfm_vehicle[vehicleid][type] == WFM_VEHICLE_FORKLIFT) {
                    HidePlayerMissionDialog(playerid, false);
                    if (p_wfm[playerid][wfm_interruption_counter] < 1) ShowPlayerNotification(playerid, "Ve al punto rojo para recoger las cajas y llevarlas al DFT.~n~~n~Recuerda terminar antes del tiempo establecido para poder completar la mision.");
                    setPlayerLiftCheckpoint(playerid);
                } else {
                    if (p_wfm[playerid][wfm_mission_progress] < 10 || !wf_is_cargo_vehicle[vehicleid]) {
                        if (ac_Info[CHEAT_MISSION_MANIPULATION][ac_Enabled]) {
                            if (gettime() > PLAYER_AC_INFO[playerid][CHEAT_MISSION_MANIPULATION][p_ac_info_IMMUNITY]) {
                                if (!ac_Info[CHEAT_MISSION_MANIPULATION][ac_Interval]) OnPlayerCheatDetected(playerid, CHEAT_MISSION_MANIPULATION, 0.0);
                                else {
                                    if (gettime() - PLAYER_AC_INFO[playerid][CHEAT_MISSION_MANIPULATION][p_ac_info_LAST_DETECTION] > ac_Info[CHEAT_MISSION_MANIPULATION][ac_Interval]) PLAYER_AC_INFO[playerid][CHEAT_MISSION_MANIPULATION][p_ac_info_DETECTIONS] = 0;
                                    else PLAYER_AC_INFO[playerid][CHEAT_MISSION_MANIPULATION][p_ac_info_DETECTIONS] ++;
                                    
                                    PLAYER_AC_INFO[playerid][CHEAT_MISSION_MANIPULATION][p_ac_info_LAST_DETECTION] = gettime();
                                    if (PLAYER_AC_INFO[playerid][CHEAT_MISSION_MANIPULATION][p_ac_info_DETECTIONS] >= ac_Info[CHEAT_MISSION_MANIPULATION][ac_Detections]) OnPlayerCheatDetected(playerid, CHEAT_MISSION_MANIPULATION, 0.0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (oldstate == PLAYER_STATE_DRIVER) {
        new vehicleid = PLAYER_TEMP[playerid][pt_LAST_VEHICLE_ID];
        if (wfm_vehicle[vehicleid][valid] && !wfm_vehicle[vehicleid][static_veh] && wfm_vehicle[vehicleid][type] == WFM_VEHICLE_FORKLIFT) {
            if (p_wfm[playerid][wfm_mission_progress] < 10) {
                if (p_wfm[playerid][wfm_interruption_counter] > 2) {
                    cancelWFMission(playerid);
                }
                else {
                    PutPlayerInVehicleEx(playerid, vehicleid, 0);
                    ShowPlayerNotification(playerid, "Primero debes terminar de subir todas las cajas al camion antes de bajarte del montacargas.~n~~n~Si sigues ~r~abandonando el montacargas~w~~h~ antes de completar la carga, la mision sera ~y~cancelada~w~~h~.");
                    p_wfm[playerid][wfm_interruption_counter] ++;
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnVehicleDeath(vehicleid, killerid) {
    if (wfm_vehicle[vehicleid][valid]) {
        if (IsPlayerConnected(wfm_vehicle[vehicleid][pid]) && PLAYER_TEMP[ wfm_vehicle[vehicleid][pid] ][pt_USER_LOGGED] && PlayerMissionId(wfm_vehicle[vehicleid][pid]) == MISSION_WEAPONS_FACTORY) {
            cancelWFMission(wfm_vehicle[vehicleid][pid]);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerDisconnect(playerid, reason) {
    if (mission_info[playerid][mission_current]) {
        if (mission_info[playerid][mission_id] == MISSION_WEAPONS_FACTORY) {
            destroyWfMissionVehicles(playerid);
            destroyWfMissionActors(playerid);
            destroyWfMissionObjects(playerid);
            destroyWfMissionNPCs(playerid);
            clearWfMissionPlayer(playerid);

            if (IsValidDynamicCP(p_wfm[playerid][wfm_mission_cp])) {
                DestroyDynamicCP(p_wfm[playerid][wfm_mission_cp]);
            }

            mission_info[playerid][mission_current] = false;
            mission_info[playerid][mission_id] = -1;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerEnterDynamicCP(playerid, checkpointid) {
    StreamerCPHeader(info);

    if (info[0] == CHECKPOINT_TYPE_WF_FORKLIFT) {
        if (!mission_info[playerid][mission_current]) return 1;
        if (mission_info[playerid][mission_id] != MISSION_WEAPONS_FACTORY) return 1;
        if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
        new vehicleid = GetPlayerVehicleID(playerid);
        if (!Vehicle_IsValid(vehicleid)) return 1;
        if (!wfm_vehicle[vehicleid][valid]) return 1;
        if (wfm_vehicle[vehicleid][pid] != playerid) return 1;
        if (wfm_vehicle[vehicleid][type] != WFM_VEHICLE_FORKLIFT) return 1;

        switch (p_wfm[playerid][wfm_mission_cp_state]) {
            case 0: {
                if (IsValidDynamicObject(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT])) DestroyDynamicObject(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT]);

                GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT] = CreateDynamicObject(2912,0.0,0.0,-1000.0,0.0,0.0,0.0,PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
                AttachDynamicObjectToVehicle(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT], vehicleid, -0.009, 0.919, -0.049, 0.000, 0.000, 0.000);

                p_wfm[playerid][wfm_mission_cp_state] = 1;
                setPlayerLiftCheckpoint(playerid);
            }
            case 1: {
                if (IsValidDynamicObject(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT])) DestroyDynamicObject(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT]);

                p_wfm[playerid][wfm_mission_progress] ++;
                
                if (p_wfm[playerid][wfm_mission_progress] >= 10) {
                    p_wfm[playerid][wfm_mission_cp_state] = 2;

                    SetVehicleParamsEx(vehicleid, 1, 0, 0, 1, 0, 0, 0);
                    RemovePlayerFromVehicle(playerid);

                    ShowPlayerMissionDialog(playerid, 57, "Jefe de la mafia", "VAMOS!!! VAMOS!!! ya terminaste que haces aqui?, VETE DE UNA VEZ!.");
                    KillTimer(PLAYER_TEMP[playerid][pt_TIMERS][22]);
                    PLAYER_TEMP[playerid][pt_TIMERS][22] = SetTimerEx("HidePlayerMissionDialog", 4000, false, "ib", playerid, false);

                    /* Crear DFT */
                    new Float: st_pos[4];
                    GetVehiclePos(p_wfm[playerid][wfm_cinematic_vehicle], st_pos[0], st_pos[1], st_pos[2]);
                    GetVehicleZAngle(p_wfm[playerid][wfm_cinematic_vehicle], st_pos[3]);
                    if (IsValidDynamicObject(GLOBAL_VEHICLES[ p_wfm[playerid][wfm_cinematic_vehicle] ][gb_vehicle_OBJECT])) DestroyDynamicObject(GLOBAL_VEHICLES[ p_wfm[playerid][wfm_cinematic_vehicle] ][gb_vehicle_OBJECT]);
                    DestroyVehicle(p_wfm[playerid][wfm_cinematic_vehicle]);
                    p_wfm[playerid][wfm_cinematic_vehicle] = INVALID_VEHICLE_ID;

                    new vehicle_id = INVALID_VEHICLE_ID;
                    vehicle_id = CreateVehicle(578, st_pos[0], st_pos[1], st_pos[2], st_pos[3], 1, 1, -1, false);
                    if (!Vehicle_IsValid(vehicle_id)) return ShowPlayerMessage(playerid, "Error, intentalo de nuevo mas tarde.");
                    GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_NONE; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = 578;
                    format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "%04d %c%c%c", vehicle_id, getRandomLetter(), getRandomLetter(), getRandomLetter());
                    GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = st_pos[0]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = st_pos[1]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = st_pos[2]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = st_pos[3];
                    GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
                    GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
                    GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
                    GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = 1; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = 1; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3;
                    GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = VEHICLE_INFO[578 - 400][vehicle_info_MAX_GAS]; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS];
                    GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_INTERIOR] = 0; GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD];
                    SetVehicleVirtualWorldEx(vehicle_id, PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
                    SetVehicleToRespawnEx(vehicle_id);
                    wf_is_cargo_vehicle[vehicle_id] = true;
                    wf_cargo_vehicle_driver[vehicle_id] = PI[playerid][pi_ID];
                    SetPVarInt(playerid, "WeaponsFactory_Cargo_Vehicleid", vehicle_id);

                    GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT] = CreateDynamicObject(2991,0.0,0.0,-1000.0,0.0,0.0,0.0,PLAYER_TEMP[playerid][pt_MISSION_VIRTUAL_WORLD]);
                    AttachDynamicObjectToVehicle(GLOBAL_VEHICLES[vehicleid][gb_vehicle_OBJECT], vehicle_id, 0.000, 0.000, 0.360, 0.000, 0.000, 90.000);

                    if (WeaponsFactoryPlayer[playerid][wf_cargo_timer] != -1) {
                        KillTimer(WeaponsFactoryPlayer[playerid][wf_cargo_timer]);
                    }
                    WeaponsFactoryPlayer[playerid][wf_cargo_timer] = -1;

                    WeaponsFactoryPlayer[playerid][wf_player_cargo_state] = PLAYER_CARGO_STATE_RETURN_WARE;

                    setPlayerLiftCheckpoint(playerid);
                    return 1;
                }

                p_wfm[playerid][wfm_mission_cp_state] = 0;
                setPlayerLiftCheckpoint(playerid);
            }
        }
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
