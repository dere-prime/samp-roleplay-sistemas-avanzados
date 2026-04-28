/*
*
*   Vetra Roleplay
*       Rappelling
*
*/

forward rappeling_timeout(const playerid);

#define RAPPELLING_OBJECTS 100

enum enum_rappelling_data {
    bool:r_rappelling,
    r_rappelling_objectid[RAPPELLING_OBJECTS],
    r_rappelling_checkpoint,
    r_rappelling_timeout
}

new rappelling_data[MAX_PLAYERS][enum_rappelling_data];