/* 

    Soluccion alternativa al TogglePlayerControllable
        Para los android

*/

#include <YSI_Coding\y_hooks>

new
    bool: v_ActiveStaticPos[MAX_VEHICLES],
    v_StaticPos_Timer[MAX_VEHICLES],
    Float: v_StaticPos_Coords[MAX_VEHICLES][4];

hook OnVehicleSpawn(vehicleid) {
    v_ActiveStaticPos[vehicleid] = false;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

stock IsVehicleActiveStaticPos(const vehicleid) { return v_ActiveStaticPos[vehicleid]; }

stock SetVehicleStaticPos(const vehicleid, Float: x, Float: y, Float: z, Float: angle) {
    v_StaticPos_Coords[vehicleid][0] = x;
    v_StaticPos_Coords[vehicleid][1] = y;
    v_StaticPos_Coords[vehicleid][2] = z;
    v_StaticPos_Coords[vehicleid][3] = angle;
    return v_ActiveStaticPos[vehicleid];
}

forward SetVehicleActiveStaticPos(const vehicleid);
public SetVehicleActiveStaticPos(const vehicleid) {
    GetVehiclePos(vehicleid, v_StaticPos_Coords[vehicleid][0], v_StaticPos_Coords[vehicleid][1], v_StaticPos_Coords[vehicleid][2]);
    GetVehicleZAngle(vehicleid, v_StaticPos_Coords[vehicleid][3]);
    v_ActiveStaticPos[vehicleid] = true;
    KillTimer(v_StaticPos_Timer[vehicleid]);
    return v_ActiveStaticPos[vehicleid];
}

stock DisableVehicleActiveStaticPos(const vehicleid) {
    v_ActiveStaticPos[vehicleid] = false;
    return v_ActiveStaticPos[vehicleid];
}


hook OnUnoccupiedVehicleUpd(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z) {
    if (v_ActiveStaticPos[vehicleid]) {
        if (GetVehicleDistanceFromPoint(vehicleid, v_StaticPos_Coords[vehicleid][0], v_StaticPos_Coords[vehicleid][1], v_StaticPos_Coords[vehicleid][2]) > 4.0) {
            SetVehiclePos(vehicleid, v_StaticPos_Coords[vehicleid][0], v_StaticPos_Coords[vehicleid][1], v_StaticPos_Coords[vehicleid][2]);
            SetVehicleZAngle(vehicleid, v_StaticPos_Coords[vehicleid][3]);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleStreamIn(vehicleid, forplayerid) {
    if (v_ActiveStaticPos[vehicleid]) {
        if (GetVehicleDistanceFromPoint(vehicleid, v_StaticPos_Coords[vehicleid][0], v_StaticPos_Coords[vehicleid][1], v_StaticPos_Coords[vehicleid][2]) > 4.0) {
            SetVehiclePos(vehicleid, v_StaticPos_Coords[vehicleid][0], v_StaticPos_Coords[vehicleid][1], v_StaticPos_Coords[vehicleid][2]);
            SetVehicleZAngle(vehicleid, v_StaticPos_Coords[vehicleid][3]);
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
