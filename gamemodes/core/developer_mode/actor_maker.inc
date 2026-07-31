/*

    Actor Maker
        Debug Mode

*/

#include <YSI_Coding\y_hooks>

#define MAX_ACTORS_MAKER 50

new actor_maker_index[MAX_ACTORS_MAKER] = -1;
new Text3D:actor_maker_3dtextlabel[MAX_ACTORS_MAKER] = Text3D:INVALID_STREAMER_ID;

getValidActorMakerIndex() {
    for (new i = 0; i != MAX_ACTORS_MAKER; i ++) {
        if (actor_maker_index[i] != -1) continue;
        return i;
    }
    return -1;
}

CMD:cactor(playerid, params[]) {
    new skin;
    if(sscanf(params, "d", skin)) return ShowPlayerMessage(playerid, "Uso: /cactor <skin>");

    new Float: pos[4];
    GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
    GetPlayerFacingAngle(playerid, pos[3]);

    new index = getValidActorMakerIndex();
    if (index == -1) return Logger_Log("Error al asignar un index al actor en actor_maker.");

    if (IsValidDynamic3DTextLabel(actor_maker_3dtextlabel[index])) DestroyDynamic3DTextLabel(actor_maker_3dtextlabel[index]);

    new str_text[16];
    format(str_text, sizeof(str_text), "index: %d", index);
    
    actor_maker_index[index] = CreateDynamicActor(skin, pos[0], pos[1], pos[2], pos[3], .worldid = GetPlayerVirtualWorld(playerid), .interiorid = GetPlayerInterior(playerid));
    actor_maker_3dtextlabel[index] = CreateDynamic3DTextLabel(str_text, 0xCCCCCCCC, pos[0], pos[1], pos[2] + 1.1, 15.0, .testlos = true, .worldid = -1, .interiorid = -1);

    Logger_Log("Actor ha sido creado exitosamente desde actor_maker.", Logger_I("index", index), Logger_I("Value", actor_maker_index[index]));
    printf("[actor_maker] index: %d, pos: %f, %f, %f, %f", index, pos[0], pos[1], pos[2], pos[3]);
    return 1;
}

CMD:ractor(playerid, params[]) {
    new index;
    if(sscanf(params, "d", index)) return ShowPlayerMessage(playerid, "Uso: /ractor <index>");
    if (index < 0 || index > MAX_ACTORS_MAKER) return ShowPlayerMessage(playerid, "Error al borrar el actor, verifica que el index sea el correcto");

    if (IsValidDynamic3DTextLabel(actor_maker_3dtextlabel[index])) DestroyDynamic3DTextLabel(actor_maker_3dtextlabel[index]);
    if (IsValidDynamicActor(actor_maker_index[index])) DestroyDynamicActor(actor_maker_index[index]);

    Logger_Log("Actor y variables han sido borradas exitosamente desde actor_maker.", Logger_I("index", index), Logger_I("Value", actor_maker_index[index]));
    
    actor_maker_index[index] = -1;
    actor_maker_3dtextlabel[index] = Text3D:INVALID_STREAMER_ID;
    return 1;
}

hook OnScriptInit() {
    for (new i = 0; i != MAX_ACTORS_MAKER; i ++) {
        actor_maker_index[i] = -1;
        actor_maker_3dtextlabel[i] = Text3D:INVALID_STREAMER_ID;
    }
}
