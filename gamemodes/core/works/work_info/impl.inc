/*

    Work Info
        Prime Roleplay
            Impl

*/

#include <YSI_Coding\y_hooks>

/* WORK INFO HELP */
GetWorkInfo(work, string[]) {
    switch (work) {
        case WORK_NONE: {
            return 0;
        }
        case WORK_TAXI: {
            format(string, 1800,
                "{FFFFFF}Trabaja llevando personas de un destino a otro.\n\
                \n\
                Sacas un vehículo del garaje de la compañía y esperas a que te llegue trabajo.\n\
                Si no hay jugadores dispuestos a usar un taxi, puedes esperar a que llegue un trabajo NPC.\n\
                La experiencia te hace conseguir clientes NPC mas rapido.\n\
                \n\
                Los trabajos NPC son totalmente idénticos a los de jugadores; te saldrá un punto rojo en el mapa para ir.\n\
                Cuando llevas un NPC no podrás elegir tu tarifa, por lo que no es un trabajo preferiblemente rentable comparado con llevar personas.\n\
                \n\
                Desbloqueos y ventajas de la Membresía: Ninguna.\n"
            );
        }
        case WORK_TRUCK: {
            format(string, 1800,
                "{FFFFFF}Trabaja llevando cargas de un lado a otro.\n\
                \n\
                Para empezar saca un camión del garaje cerca de la oficina del trabajo.\n\
                Estaciónalo cerca del punto marcado en rojo, ni tan cerca ni tan lejos.\n\
                Luego ve a la zona de carga a pie y cuando agarres la caja regresa al camión hasta completar la barra de carga.\n\
                \n\
                Una vez completado, lleva el camión al destino indicado y regresa para recibir tu paga.\n\
                Los viajes que te tocarán serán completamente aleatorios.\n\
                \n\
                Desbloqueos y ventajas de la Membresía: Ninguna.\n"
            );
        }
        case WORK_FARMER: {
            format(string, 1800,
                "{FFFFFF}Compra semillas de distintos tipos.\n\
                Plántalas en el terreno marcado con una bandera en el mapa y espera a que crezcan.\n\
                Cuando estén listas, cosecha la planta antes de que el tiempo restante finalice.\n\
                Luego puedes vender la producción o consumirla a tu antojo.\n\
                Ganas experiencia cada vez que plantas.\n\
                \n\
                Desbloqueos:\n\
                {"#SECONDARY_COLOR"}•{FFFFFF} Cada +1 de exp plantas más rápido, hasta un límite de +150 experiencia.\n\
                {"#SECONDARY_COLOR"}•{FFFFFF} Las semillas de marihuana se desbloquean desde +100 de experiencia.\n\
                {"#SECONDARY_COLOR"}•{FFFFFF} Las semillas de crack se desbloquean desde +200 de experiencia.\n"
            );
        }
    }
    return 1;
}
DialogCreate:PlayerWorksHelp(playerid) {
    new dialog[45 * sizeof work_info];
    for(new i = 1; i < E_WORKS; i ++) {
        strcatf(dialog, "{"#SECONDARY_COLOR"}•{FFFFFF} %c%s\n", toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1]);
    }
    Dialog_Open(playerid, Dialog:PlayerWorksHelp, DIALOG_STYLE_TABLIST, "Ayuda · {"#PRIMARY_COLOR"}Trabajos", dialog, "Ver", "Atras");
    return 1;
}
DialogResponse:PlayerWorksHelp(playerid, response, listitem, inputtext[]) {
    if (response) {
        new work = (listitem + 1), dialog[1801], caption[48];
        format(caption, sizeof(caption), "Ayuda · {"#PRIMARY_COLOR"}%c%s", toupper(work_info[work][work_info_NAME][0]), work_info[work][work_info_NAME][1]);
        GetWorkInfo(work, dialog);

        Dialog_Open(playerid, Dialog:PlayerWorksHelpWork, DIALOG_STYLE_MSGBOX, caption, dialog, "Volver", "");
    } else {
        Dialog_Show(playerid, Dialog:PlayerHelpMenu);
    }
    return 1;
}
DialogResponse:PlayerWorksHelpWork(playerid, response, listitem, inputtext[]) { Dialog_Show(playerid, Dialog:PlayerWorksHelp); return 1; }

/* CALLBACK OBTAIN PLAYER JOB */
hook OnPlayerObtainWork(playerid, work) {
    ShowPlayerMessage(playerid, "Ahora trabajas como ~g~%s~w~~h~.", work_info[work][work_info_NAME]);
}

/* OBTAIN WORK PICKUP AND ETC */
hook OnPlayerPickUpDynPickup(playerid, pickupid) {
    if (pickupid == PICKUP_TYPE_NONE) return Y_HOOKS_CONTINUE_RETURN_1;
	if (PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] > gettime()) return Y_HOOKS_BREAK_RETURN_0;
	if (Dialog_IsOpen(playerid)) return Y_HOOKS_BREAK_RETURN_0;
    if (MS_IsOpen(playerid)) return Y_HOOKS_BREAK_RETURN_0;

	new info[3];
	Streamer_GetArrayData(STREAMER_TYPE_PICKUP, pickupid, E_STREAMER_EXTRA_ID, info);
	if (info[0] == PICKUP_TYPE_OBTAIN_WORK) {
        if (work_info[ info[1] ][work_info_TYPE] == WORK_TYPE_FAMILY) return Y_HOOKS_BREAK_RETURN_1;

        if (GetPlayerInterior(playerid) == obtain_work_coords[ info[1] ][obtain_work_INTERIOR]) {
            if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return ShowPlayerMessage(playerid, "Debes estar a pie.");
            if (PI[playerid][pi_WANTED_LEVEL] > 0) return ShowPlayerMessage(playerid, "No puedes obtener un trabajo cuando estás en búsqueda.");
            ShowPlayerTimeOut(PLAYER_TEMP[playerid][pt_LAST_GOT_WORK_TIME], NECESSARY_TIME_BETWEEN_WORKS, playerid, "Debes esperar %s para volver a solicitar otro trabajo.");
            new player_jobs = CountPlayerJobs(playerid);
            if (PI[playerid][pi_VIP]) {
                if (player_jobs >= MAX_SU_WORKS) return ShowPlayerNotification(playerid, "Actualmente tienes ~p~%d trabajos~w~~h~, usa ~y~/renunciar~w~~h~ para dejar un trabajo.", player_jobs);
            } else {
                if (player_jobs >= MAX_NU_WORKS) return ShowPlayerNotification(playerid, "Actualmente tienes ~p~%d trabajos~w~~h~, usa ~y~/renunciar~w~~h~ para dejar un trabajo.~n~~n~Con la membresia puedes tener hasta ~y~%d trabajos~w~~h~, usa ~y~/ayuda~w~~h~ si necesitas mas informacion.", player_jobs, MAX_SU_WORKS);
            }

            if (IsPlayerInRangeOfPoint(playerid, 2.0, obtain_work_coords[ info[1] ][obtain_work_X], obtain_work_coords[ info[1] ][obtain_work_Y], obtain_work_coords[ info[1] ][obtain_work_Z])) {
                if (work_info[ info[1] ][work_info_LEVEL] > PI[playerid][pi_LEVEL]) return ShowPlayerMessage(playerid, "Necesitas ser al menos nivel %d para obtener este trabajo.", work_info[ info[1] ][work_info_LEVEL]);
                if (PLAYER_WORKS[playerid][ info[1] ][pwork_SET]) return ShowPlayerMessage(playerid, "Ya eres %s.", work_info[ info[1] ][work_info_NAME]);
                
                mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "INSERT INTO pworks (id_player, id_work, `set`, level) VALUES(%d, %d, 1, 1) ON DUPLICATE KEY UPDATE `set` = 1;", PI[playerid][pi_ID], info[1]);
                mysql_tquery(handle_db, QUERY_BUFFER);

                PLAYER_WORKS[playerid][ info[1] ][pwork_SET] = true;
                PLAYER_TEMP[playerid][pt_LAST_GOT_WORK_TIME] = gettime();
                CallLocalFunction("OnPlayerObtainWork", "ii", playerid, info[1]);
                
                PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 5;
            }
        }
        return Y_HOOKS_BREAK_RETURN_1;
    }
    else if (info[0] == PICKUP_TYPE_VEHICLE_SPAWNER) {
        new const index = info[1];
        new const current_work_vehicle = GetPVarInt(playerid, "CurrentWorkVehicleid");
        if (Vehicle_IsValid(current_work_vehicle)) return ShowPlayerMessage(playerid, "Ya tienes un vehiculo de trabajo, estacionalo cerca de el punto para guardarlo.");
        if (!IsPlayerWork(playerid, work_vehicles_spawn[index][v_work])) return ShowPlayerMessage(playerid, "No eres ~g~%s~w~~h~.", work_info[ work_vehicles_spawn[index][v_work] ][work_info_NAME]);
        if (work_info[ work_vehicles_spawn[index][v_work] ][work_info_NEED_DUTY]) {
            if (IsPlayerNotWorking(playerid, work_vehicles_spawn[index][v_work])) return ShowPlayerMessage(playerid, "Necesitas estar de servicio como ~g~%s~w~~h~.", work_info[ work_vehicles_spawn[index][v_work] ][work_info_NAME]);
        }
        if (work_vehicles_spawn[index][v_required_exp] > PLAYER_WORKS[playerid][ work_vehicles_spawn[index][v_work] ][pwork_LEVEL]) return ShowPlayerMessage(playerid, "Necesitas tener %d de experiencia o mas como %s.", work_vehicles_spawn[index][v_required_exp], work_info[ work_vehicles_spawn[index][v_work] ][work_info_NAME]);
        
        PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 5;

        if (work_vehicles_spawn[index][v_use_interface]) {
            SetPVarInt(playerid, "VehicleSpawnerIndex", index);
            ShowPlayerWorkVehicleSelMenu(playerid, work_vehicles_spawn[index][v_work]);
        } else {
            new Float:x_offset, Float:y_offset;

            x_offset = work_vehicles_spawn[index][v_x];
            y_offset = work_vehicles_spawn[index][v_y];
            x_offset += (7.0 * floatsin(-work_vehicles_spawn[index][v_angle], degrees));
            y_offset += (7.0 * floatcos(-work_vehicles_spawn[index][v_angle], degrees));

            CreateWorkVehicle(
                work_vehicles_spawn[index][v_work], work_vehicles_spawn[index][v_required_exp], work_vehicles_spawn[index][v_vehicle_model],
                x_offset, y_offset, work_vehicles_spawn[index][v_z], work_vehicles_spawn[index][v_angle], 
                work_vehicles_spawn[index][v_color1], work_vehicles_spawn[index][v_color2], 0, 0, true, playerid
            );

            ShowPlayerNotification(playerid, "Has sacado un ~g~%s~w~~h~, cuando no lo quieras, estacionalo cerca de el punto de spawn.~n~Si te bajas, el vehiculo desaparecera en ~r~2 minutos~w~~h~.", VEHICLE_INFO[ work_vehicles_spawn[index][v_vehicle_model]-400 ][vehicle_info_NAME]);
            ShowPlayerNotificationModel(playerid, work_vehicles_spawn[index][v_vehicle_model], 0.0, 0.0, 45.0, "~n~%s~n~~n~", VEHICLE_INFO[ work_vehicles_spawn[index][v_vehicle_model]-400 ][vehicle_info_NAME]);
                
            PLAYER_TEMP[playerid][pt_PICKUP_TIMEOUT] = gettime() + 5;
        }
        return Y_HOOKS_BREAK_RETURN_1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnScriptInit() {
    static info[3], pickup_id, label_str[256];
	for(new i = 1; i < E_WORKS; i ++) {
		if (!obtain_work_coords[i][obtain_work_AVAILABLE]) continue;
		switch(work_info[i][work_info_TYPE]) {
			case WORK_TYPE_NORMAL: {
				if (obtain_work_coords[i][obtain_work_LABELS]) {
					format(label_str, sizeof label_str, "{%s}%c%s\n{FFFFFF}Nivel: {%s}%d\n\n{"#PRIMARY_COLOR"}Acercate{FFFFFF} para conseguir el trabajo", work_info[i][work_info_COLOR], toupper(work_info[i][work_info_NAME][0]), work_info[i][work_info_NAME][1], work_info[i][work_info_COLOR], work_info[i][work_info_LEVEL]);
					CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, obtain_work_coords[i][obtain_work_X], obtain_work_coords[i][obtain_work_Y], obtain_work_coords[i][obtain_work_Z], 10.0, .testlos = true, .interiorid = obtain_work_coords[i][obtain_work_INTERIOR]);
					pickup_id = CreateDynamicPickup(1314, 1, obtain_work_coords[i][obtain_work_X], obtain_work_coords[i][obtain_work_Y], (obtain_work_coords[i][obtain_work_Z] - 0.5), -1, obtain_work_coords[i][obtain_work_INTERIOR]);
                    info[0] = PICKUP_TYPE_OBTAIN_WORK;
                    info[1] = i; // Index
                    info[2] = 0; // Nada
                    Streamer_SetArrayData(STREAMER_TYPE_PICKUP, pickup_id, E_STREAMER_EXTRA_ID, info);
                }
				if (obtain_work_coords[i][obtain_work_MAP_ICON]) CreateDynamicMapIcon(obtain_work_coords[i][obtain_work_MAP_ICON_X], obtain_work_coords[i][obtain_work_MAP_ICON_Y], obtain_work_coords[i][obtain_work_MAP_ICON_Z], obtain_work_coords[i][obtain_work_MAP_ICON_ID], -1, 0, 0);
			}
		}
	}
    for (new i = 0; i != sizeof(work_vehicles_spawn); i ++) {
        format(label_str, sizeof(label_str), "{%s}%c%s\n{"#PRIMARY_COLOR"}Acercate{FFFFFF} para sacar un vehiculo", work_info[ work_vehicles_spawn[i][v_work] ][work_info_COLOR], toupper(work_info[ work_vehicles_spawn[i][v_work] ][work_info_NAME][0]), work_info[ work_vehicles_spawn[i][v_work] ][work_info_NAME][1]);
        work_vehicles_spawn[i][v_text_3d] = CreateDynamic3DTextLabel(label_str, 0xFFFFFFFF, work_vehicles_spawn[i][v_x], work_vehicles_spawn[i][v_y], (work_vehicles_spawn[i][v_z]), 10.0, .testlos = true, .worldid = 0, .interiorid = 0);
        work_vehicles_spawn[i][v_pickup_id] = CreateDynamicPickup(1098, 1, work_vehicles_spawn[i][v_x], work_vehicles_spawn[i][v_y], (work_vehicles_spawn[i][v_z]), 0, 0);
        info[0] = PICKUP_TYPE_VEHICLE_SPAWNER;
        info[1] = i; // Index
        info[2] = 0; // Nada
        Streamer_SetArrayData(STREAMER_TYPE_PICKUP, work_vehicles_spawn[i][v_pickup_id], E_STREAMER_EXTRA_ID, info);
    }
}

/* VEHICLE SPAWNER */
getWorkVehicleRequiredExp(work, modelid) {
    for (new i = 0; i != sizeof(work_vehicles_required_exp); i ++) {
        if (work_vehicles_required_exp[i][r_work] != work) continue;
        if (work_vehicles_required_exp[i][r_modelid] != modelid) continue;
        return (work_vehicles_required_exp[i][r_required_exp]);
    }
    return 0;
}

CreateWorkVehicle(work, required_exp, modelid, Float: X, Float: Y, Float: Z, Float: Angle, Color1, Color2, interior, world, bool: put, playerid) {
	new vehicle_id = INVALID_VEHICLE_ID;
	vehicle_id = CreateVehicle(modelid, X, Y, Z, Angle, Color1, Color2, -1, false);
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_VALID] = true;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_TYPE] = VEHICLE_TYPE_WORK;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MODELID] = modelid;
	format(GLOBAL_VEHICLES[vehicle_id][gb_vehicle_NUMBER_PLATE], 32, "%04d %c%c%c", PLAYER_VEHICLES[vehicle_id][player_vehicle_ID], getRandomLetter(), getRandomLetter(), getRandomLetter());
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X] = X;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y] = Y;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z] = Z;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_ANGLE] = Angle;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][0] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_X];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][1] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Y];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_POS][2] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_SPAWN_Z];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_HEALTH] = 1000.0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_PANELS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_DOORS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_LIGHTS] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_DAMAGE_TIRES] = 0;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_1] = Color1;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_COLOR_2] = Color2;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_PAINTJOB] = 3; // No paintjob
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS] = VEHICLE_INFO[modelid - 400][vehicle_info_MAX_GAS];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_GAS] = GLOBAL_VEHICLES[vehicle_id][gb_vehicle_MAX_GAS];
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_STATE] = VEHICLE_STATE_NORMAL;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_INTERIOR] = interior;
	GLOBAL_VEHICLES[vehicle_id][gb_vehicle_WORLD] = 0;
	WORK_VEHICLES[vehicle_id][work_vehicle_VALID] = true;
	WORK_VEHICLES[vehicle_id][work_vehicle_WORK] = work;
	WORK_VEHICLES[vehicle_id][work_vehicle_EXP] = required_exp;
	WORK_VEHICLES[vehicle_id][work_vehicle_NEED_DUTY] = work_info[work][work_info_NEED_DUTY];
	WORK_VEHICLES[vehicle_id][work_vehicle_TIMEOUT] = gettime();
    WORK_VEHICLES[vehicle_id][work_vehicle_PID] = PI[playerid][pi_ID];
	SetVehicleVirtualWorldEx(vehicle_id, world);
	SetVehicleToRespawnEx(vehicle_id);
    SetPVarInt(playerid, "CurrentWorkVehicleid", vehicle_id);
    if (put) {
        PutPlayerInVehicleEx(playerid, vehicle_id, 0);
    }
	return vehicle_id;
}

/* VEHICLE SPAWNER MENUS */
ShowPlayerWorkVehicleSelMenu(const playerid, const work) {
    switch (work) {
        case WORK_TAXI: {
            new List:TaxiVehicles = list_new();

            AddModelMenuItem(TaxiVehicles, 420, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6); AddModelMenuItem(TaxiVehicles, 420, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6);
            AddModelMenuItem(TaxiVehicles, 420, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6); AddModelMenuItem(TaxiVehicles, 420, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6);
            AddModelMenuItem(TaxiVehicles, 420, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6); AddModelMenuItem(TaxiVehicles, 420, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6);

            AddModelMenuItem(TaxiVehicles, 438, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6); AddModelMenuItem(TaxiVehicles, 438, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6);
            AddModelMenuItem(TaxiVehicles, 438, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6); AddModelMenuItem(TaxiVehicles, 438, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6);
            AddModelMenuItem(TaxiVehicles, 438, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6); AddModelMenuItem(TaxiVehicles, 438, .usingrotation = true, .rotz = 45.0, .vehiclecolor1 = 6, .vehiclecolor2 = 6);

            ShowModelSelectionMenu(playerid, "Vehiculos - Taxista", MODEL_SELECTION_WORK_TAXI, TaxiVehicles);
            return work;
        }
    }
    return 1;
}
hook OnModelSelectionResponse(playerid, extraid, index, modelid, response) {
    switch (extraid) {
        case MODEL_SELECTION_WORK_TAXI: {
            new vehicle_spawner_index = GetPVarInt(playerid, "VehicleSpawnerIndex");

            if (getWorkVehicleRequiredExp(work_vehicles_spawn[vehicle_spawner_index][v_work], modelid) > PLAYER_WORKS[playerid][ work_vehicles_spawn[vehicle_spawner_index][v_work] ][pwork_LEVEL]) return ShowPlayerMessage(playerid, "Necesitas tener %d de experiencia o mas como %s.", getWorkVehicleRequiredExp(work_vehicles_spawn[vehicle_spawner_index][v_work], modelid), work_info[ work_vehicles_spawn[vehicle_spawner_index][v_work] ][work_info_NAME]);
        
            new Float:x_offset, Float:y_offset;
            x_offset = work_vehicles_spawn[vehicle_spawner_index][v_x];
            y_offset = work_vehicles_spawn[vehicle_spawner_index][v_y];
            x_offset += (7.0 * floatsin(-work_vehicles_spawn[vehicle_spawner_index][v_angle], degrees));
            y_offset += (7.0 * floatcos(-work_vehicles_spawn[vehicle_spawner_index][v_angle], degrees));

            CreateWorkVehicle(
                work_vehicles_spawn[vehicle_spawner_index][v_work], getWorkVehicleRequiredExp(work_vehicles_spawn[vehicle_spawner_index][v_work], modelid), modelid,
                x_offset, y_offset, work_vehicles_spawn[vehicle_spawner_index][v_z], work_vehicles_spawn[vehicle_spawner_index][v_angle], 
                work_vehicles_spawn[vehicle_spawner_index][v_color1], work_vehicles_spawn[vehicle_spawner_index][v_color2], 0, 0, true, playerid
            );

            ShowPlayerNotification(playerid, "Has sacado un ~g~%s~w~~h~, cuando no lo quieras, estacionalo cerca de el punto de spawn.~n~Si te bajas, el vehiculo desaparecera en ~r~2 minutos~w~~h~.", VEHICLE_INFO[ modelid-400 ][vehicle_info_NAME]);
            ShowPlayerNotificationModel(playerid, modelid, 0.0, 0.0, 45.0, "~n~%s~n~~n~", VEHICLE_INFO[ modelid-400 ][vehicle_info_NAME]);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

