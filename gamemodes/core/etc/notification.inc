/*
    Notifications
        Prime Roleplay
*/
#include <YSI_Coding\y_hooks>
#include <td-string-width>

/* Config and Vars */
#define MAX_NTD 6
#define NTD_POS_X 480.000000
#define NTD_WITH_MODEL_X 510.000000
#define NTD_POS_Y 130.000000
#define NTD_MODEL_POS_Y 128.399
#define NTD_FONT 1
#define NTD_LETTER_SIZE_X 0.250000
#define NTD_LETTER_SIZE_Y 1.000000
#define NTD_SIZE 129.000000
#define NTD_MODEL_SIZE 99.000000
#define NTD_COLOR 0xFFFFFFFF
#define NTD_COLOR_BOX 0x141414FF
#define NTD_PROPORTIONAL 1
#define NTD_DISTANCE 5
#define NTD_TIME 10000
#define MAX_NTD_TEXT 800

enum enum_player_notification_td {
    Use,
    UseModelId,
    Line,
    Text[MAX_NTD_TEXT],
    PlayerText:TextDraw,
    PlayerText:TextDrawModel,
    Float:MinPosY,
    Float:MinModelPosY,
    Float:MaxPosY,
    Float:MaxModelPosY,
    Float:rot_x,
    Float:rot_y,
    Float:rot_z,
    Animator,
    Timer
}
static player_notification_td[MAX_PLAYERS][MAX_NTD][enum_player_notification_td];
static selected_animator_cycle[MAX_PLAYERS];

/* Notification TD Management */
static update_player_notifications(playerid) {
    for(new cycle = 0; cycle < MAX_NTD; cycle ++) {
        if(!player_notification_td[playerid][cycle][Use]) {
            if(cycle != MAX_NTD - 1) {
                if(player_notification_td[playerid][cycle + 1][Use]) {
                    player_notification_td[playerid][cycle][Use] = player_notification_td[playerid][cycle + 1][Use];
                    player_notification_td[playerid][cycle][UseModelId] = player_notification_td[playerid][cycle + 1][UseModelId];
                    player_notification_td[playerid][cycle][Line] = player_notification_td[playerid][cycle + 1][Line];
                    strcat(player_notification_td[playerid][cycle][Text], player_notification_td[playerid][cycle + 1][Text], MAX_NTD_TEXT);
                    player_notification_td[playerid][cycle][TextDraw] = player_notification_td[playerid][cycle + 1][TextDraw];
                    player_notification_td[playerid][cycle][TextDrawModel] = player_notification_td[playerid][cycle + 1][TextDrawModel];
                    player_notification_td[playerid][cycle][rot_x] = player_notification_td[playerid][cycle + 1][rot_x];
                    player_notification_td[playerid][cycle][rot_y] = player_notification_td[playerid][cycle + 1][rot_y];
                    player_notification_td[playerid][cycle][rot_z] = player_notification_td[playerid][cycle + 1][rot_z];

                    player_notification_td[playerid][cycle + 1][Use] = 0;
                    player_notification_td[playerid][cycle + 1][UseModelId] = 0;
                    player_notification_td[playerid][cycle + 1][Line] = 0;
                    player_notification_td[playerid][cycle + 1][Text][0] = EOS;
                    player_notification_td[playerid][cycle + 1][TextDraw] = PlayerText:-1;
                    player_notification_td[playerid][cycle + 1][TextDrawModel] = PlayerText:-1;
                    player_notification_td[playerid][cycle + 1][MinPosY] = 0;
                    player_notification_td[playerid][cycle + 1][MinModelPosY] = 0;
                    player_notification_td[playerid][cycle + 1][MaxPosY] = 0;
                    player_notification_td[playerid][cycle + 1][MaxModelPosY] = 0;
                    player_notification_td[playerid][cycle + 1][rot_x] = 0;
                    player_notification_td[playerid][cycle + 1][rot_y] = 0;
                    player_notification_td[playerid][cycle + 1][rot_z] = 0;

                    min_pos_notification_td(playerid, cycle);
                    max_pos_notification_td(playerid, cycle);  
                }
            }
        }
        else if(player_notification_td[playerid][cycle][Use]) {
            if(cycle != 0) {
                if(!player_notification_td[playerid][cycle - 1][Use]) {
                    player_notification_td[playerid][cycle - 1][Use] = player_notification_td[playerid][cycle][Use];
                    player_notification_td[playerid][cycle - 1][UseModelId] = player_notification_td[playerid][cycle][UseModelId];
                    player_notification_td[playerid][cycle - 1][Line] = player_notification_td[playerid][cycle][Line];
                    strcat(player_notification_td[playerid][cycle - 1][Text], player_notification_td[playerid][cycle][Text], MAX_NTD_TEXT);
                    player_notification_td[playerid][cycle - 1][TextDraw] = player_notification_td[playerid][cycle][TextDraw];
                    player_notification_td[playerid][cycle - 1][TextDrawModel] = player_notification_td[playerid][cycle][TextDrawModel];
                    player_notification_td[playerid][cycle - 1][rot_x] = player_notification_td[playerid][cycle][rot_x];
                    player_notification_td[playerid][cycle - 1][rot_y] = player_notification_td[playerid][cycle][rot_y];
                    player_notification_td[playerid][cycle - 1][rot_z] = player_notification_td[playerid][cycle][rot_z];

                    player_notification_td[playerid][cycle][Use] = 0;
                    player_notification_td[playerid][cycle][UseModelId] = 0;
                    player_notification_td[playerid][cycle][Line] = 0;
                    player_notification_td[playerid][cycle][Text][0] = EOS;
                    player_notification_td[playerid][cycle][TextDraw] = PlayerText:-1;
                    player_notification_td[playerid][cycle][TextDrawModel] = PlayerText:-1;
                    player_notification_td[playerid][cycle][MinPosY] = 0;
                    player_notification_td[playerid][cycle][MinModelPosY] = 0;
                    player_notification_td[playerid][cycle][MaxPosY] = 0;
                    player_notification_td[playerid][cycle][MaxModelPosY] = 0;
                    player_notification_td[playerid][cycle][rot_x] = 0;
                    player_notification_td[playerid][cycle][rot_y] = 0;
                    player_notification_td[playerid][cycle][rot_z] = 0;

                    min_pos_notification_td(playerid, cycle - 1);
                    max_pos_notification_td(playerid, cycle - 1);
                }
            }
        }
        create_player_notification(playerid, cycle);
        if (player_notification_td[playerid][cycle][Timer] == -1) {
            KillTimer(player_notification_td[playerid][cycle][Timer]);
            player_notification_td[playerid][cycle][Timer] = SetTimerEx("player_notification_hide_timer", NTD_TIME, false, "i", playerid, cycle);
        }
    }
    return 1;
}
static min_pos_notification_td(playerid, NTD) {
    if(NTD == 0) {
        player_notification_td[playerid][NTD][MinPosY] = NTD_POS_Y;
        player_notification_td[playerid][NTD][MinModelPosY] = NTD_MODEL_POS_Y;
    } else {
        player_notification_td[playerid][NTD][MinPosY] = player_notification_td[playerid][NTD - 1][MaxPosY] + NTD_DISTANCE;
        player_notification_td[playerid][NTD][MinModelPosY] = player_notification_td[playerid][NTD - 1][MaxModelPosY] + NTD_DISTANCE;
    }
    return 1;
}
static max_pos_notification_td(playerid, NTD) {
    player_notification_td[playerid][NTD][MaxPosY] = player_notification_td[playerid][NTD][MinPosY] + (NTD_LETTER_SIZE_Y * 2) + 2 + (NTD_LETTER_SIZE_Y * 5.75 * player_notification_td[playerid][NTD][Line]) + ((player_notification_td[playerid][NTD][Line] - 1) * ((NTD_LETTER_SIZE_Y * 2) + 2 + NTD_LETTER_SIZE_Y)) + NTD_LETTER_SIZE_Y + 3;
    player_notification_td[playerid][NTD][MaxModelPosY] = player_notification_td[playerid][NTD][MinModelPosY] + (NTD_LETTER_SIZE_Y * 2) + 2 + (NTD_LETTER_SIZE_Y * 5.75 * player_notification_td[playerid][NTD][Line]) + ((player_notification_td[playerid][NTD][Line] - 1) * ((NTD_LETTER_SIZE_Y * 2) + 2 + NTD_LETTER_SIZE_Y)) + NTD_LETTER_SIZE_Y + 3;
    return 1;
}
static lines_notification_td(playerid, NTD) {
    new lines = 1, Float:width, lastspace = -1, supr, len = strlen(player_notification_td[playerid][NTD][Text]);
 
    for(new i = 0; i < len; i ++) {
        if(player_notification_td[playerid][NTD][Text][i] == '~') {
            if(supr == 0) {
                supr = 1;
                if(player_notification_td[playerid][NTD][Text][i+2] != '~') SendClientMessage(playerid, -1, "Error: '~' used incorrectly'");
            }
            else if(supr == 1) supr = 0;
        } else {
            if(supr == 1) {
                if(player_notification_td[playerid][NTD][Text][i] == 'n') {
                    lines ++;
                    lastspace = -1;
                    width = 0;
                }
            } else {
                if(player_notification_td[playerid][NTD][Text][i] == ' ') lastspace = i;
 
                width += NTD_LETTER_SIZE_X * GetTextDrawCharacterWidth(player_notification_td[playerid][NTD][Text][i], NTD_FONT, bool:NTD_PROPORTIONAL);
                
                new Float:size = NTD_SIZE;
                if (player_notification_td[playerid][NTD][UseModelId]) { size = NTD_MODEL_SIZE; }

                if(width > size - 3) {
                    if(lastspace != i && lastspace != -1) {
                        lines ++;
                        i = lastspace;
                        lastspace = -1;
                        width = 0;
                    } else {
                        lines ++;
                        lastspace = -1;
                        width = 0;
                    }
                }
            }
        }
    }

    player_notification_td[playerid][NTD][Line] = lines;
    return 1;
}
static create_player_notification(playerid, NTD) {
    if(player_notification_td[playerid][NTD][Use] == 1) {
        if(player_notification_td[playerid][NTD][TextDraw] != PlayerText:-1) {
            PlayerTextDrawDestroy(playerid, player_notification_td[playerid][NTD][TextDraw]);
        }
        if(player_notification_td[playerid][NTD][TextDrawModel] != PlayerText:-1) {
            PlayerTextDrawDestroy(playerid, player_notification_td[playerid][NTD][TextDrawModel]);
        }

        if (player_notification_td[playerid][NTD][UseModelId]) player_notification_td[playerid][NTD][TextDraw] = CreatePlayerTextDraw(playerid, NTD_WITH_MODEL_X, player_notification_td[playerid][NTD][MinPosY], player_notification_td[playerid][NTD][Text]);
        else player_notification_td[playerid][NTD][TextDraw] = CreatePlayerTextDraw(playerid, NTD_POS_X, player_notification_td[playerid][NTD][MinPosY], player_notification_td[playerid][NTD][Text]);
        PlayerTextDrawFont(playerid, player_notification_td[playerid][NTD][TextDraw], NTD_FONT);
        PlayerTextDrawLetterSize(playerid, player_notification_td[playerid][NTD][TextDraw], NTD_LETTER_SIZE_X, NTD_LETTER_SIZE_Y);
        if (player_notification_td[playerid][NTD][UseModelId]) PlayerTextDrawTextSize(playerid, player_notification_td[playerid][NTD][TextDraw], floatadd(NTD_WITH_MODEL_X, NTD_MODEL_SIZE), 1.000000);
        else PlayerTextDrawTextSize(playerid, player_notification_td[playerid][NTD][TextDraw], floatadd(NTD_POS_X, NTD_SIZE), 1.000000);
        PlayerTextDrawSetOutline(playerid, player_notification_td[playerid][NTD][TextDraw], 0);
        PlayerTextDrawSetShadow(playerid, player_notification_td[playerid][NTD][TextDraw], 0);
        PlayerTextDrawAlignment(playerid, player_notification_td[playerid][NTD][TextDraw], 1);
        PlayerTextDrawColor(playerid, player_notification_td[playerid][NTD][TextDraw], NTD_COLOR);
        PlayerTextDrawBackgroundColor(playerid, player_notification_td[playerid][NTD][TextDraw], 0x00000000);
        PlayerTextDrawBoxColor(playerid, player_notification_td[playerid][NTD][TextDraw], NTD_COLOR_BOX);
        PlayerTextDrawUseBox(playerid, player_notification_td[playerid][NTD][TextDraw], 1);
        PlayerTextDrawSetProportional(playerid, player_notification_td[playerid][NTD][TextDraw], NTD_PROPORTIONAL);
        PlayerTextDrawSetSelectable(playerid, player_notification_td[playerid][NTD][TextDraw], 0);
        PlayerTextDrawShow(playerid, player_notification_td[playerid][NTD][TextDraw]);
        
        if (player_notification_td[playerid][NTD][UseModelId]) {
            player_notification_td[playerid][NTD][TextDrawModel] = CreatePlayerTextDraw(playerid, 479.000, player_notification_td[playerid][NTD][MinModelPosY], "_");
            PlayerTextDrawTextSize(playerid, player_notification_td[playerid][NTD][TextDrawModel], 30.000, 30.399);
            PlayerTextDrawAlignment(playerid, player_notification_td[playerid][NTD][TextDrawModel], 1);
            PlayerTextDrawColor(playerid, player_notification_td[playerid][NTD][TextDrawModel], -1);
            PlayerTextDrawSetShadow(playerid, player_notification_td[playerid][NTD][TextDrawModel], 0);
            PlayerTextDrawSetOutline(playerid, player_notification_td[playerid][NTD][TextDrawModel], 0);
            PlayerTextDrawBackgroundColor(playerid, player_notification_td[playerid][NTD][TextDrawModel], NTD_COLOR_BOX);
            PlayerTextDrawFont(playerid, player_notification_td[playerid][NTD][TextDrawModel], 5);
            PlayerTextDrawSetProportional(playerid, player_notification_td[playerid][NTD][TextDrawModel], 0);
            PlayerTextDrawSetPreviewModel(playerid, player_notification_td[playerid][NTD][TextDrawModel], player_notification_td[playerid][NTD][UseModelId]);
            PlayerTextDrawSetPreviewRot(playerid, player_notification_td[playerid][NTD][TextDrawModel], player_notification_td[playerid][NTD][rot_x], player_notification_td[playerid][NTD][rot_y], player_notification_td[playerid][NTD][rot_z], 0.850);
            PlayerTextDrawSetPreviewVehCol(playerid, player_notification_td[playerid][NTD][TextDrawModel], 0, 0);
            PlayerTextDrawShow(playerid, player_notification_td[playerid][NTD][TextDrawModel]);
        }
    }
    return 1;
}
/* Hide Function */
forward player_notification_hide_timer(playerid, cycle);
public player_notification_hide_timer(playerid, cycle) {
    KillTimer(player_notification_td[playerid][cycle][Timer]);
    player_notification_td[playerid][cycle][Timer] = -1;
    if (!player_notification_td[playerid][cycle][Use]) return 0;
    selected_animator_cycle[playerid] = cycle;
    if (!player_notification_td[playerid][cycle][UseModelId]) {
        player_notification_td[playerid][cycle][Animator] = PlayerText_InterpolateBoxColour(playerid, player_notification_td[playerid][cycle][TextDraw], 0x00000000, 1000, EASE_OUT_SINE);
        PlayerText_InterpolateColor(playerid, player_notification_td[playerid][cycle][TextDraw], 0x00000000, 1000, EASE_OUT_SINE);
    } else {
        player_notification_td[playerid][cycle][Animator] = PlayerText_InterpolateColor(playerid, player_notification_td[playerid][cycle][TextDraw], 0x00000000, 1000, EASE_OUT_SINE);
    }
    return 1;
}
hook Animator_OnFinish(playerid, animator, type) {
    new const cycle = selected_animator_cycle[playerid];
    if (animator == player_notification_td[playerid][cycle][Animator]) {
        if (player_notification_td[playerid][cycle][Timer] != -1) KillTimer(player_notification_td[playerid][cycle][Timer]);
        player_notification_td[playerid][cycle][Timer] = -1;
        player_notification_td[playerid][cycle][Use] = 0;
        if(player_notification_td[playerid][cycle][TextDraw] != PlayerText:-1) {
            PlayerTextDrawDestroy(playerid, player_notification_td[playerid][cycle][TextDraw]);
            player_notification_td[playerid][cycle][Line] = 0;
            player_notification_td[playerid][cycle][Text][0] = EOS;
            player_notification_td[playerid][cycle][MinPosY] = 0;
            player_notification_td[playerid][cycle][MaxPosY] = 0;
            player_notification_td[playerid][cycle][MinModelPosY] = 0;
            player_notification_td[playerid][cycle][MaxModelPosY] = 0;
            player_notification_td[playerid][cycle][TextDraw] = PlayerText:-1;
            if (player_notification_td[playerid][cycle][UseModelId]) {
                if (player_notification_td[playerid][cycle][TextDrawModel] != PlayerText:-1) {
                    PlayerTextDrawDestroy(playerid, player_notification_td[playerid][cycle][TextDrawModel]);
                }
                player_notification_td[playerid][cycle][TextDrawModel] = PlayerText:-1;
            }
        }
        update_player_notifications(playerid);
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

/* Show to player */
forward ShowPlayerNotification(playerid, text[], GLOBAL_TAG_TYPES:...); 
public ShowPlayerNotification(playerid, text[], GLOBAL_TAG_TYPES:...) {
    for(new cycle; cycle < MAX_NTD; cycle++) {
        if(!player_notification_td[playerid][cycle][Use]) {
            player_notification_td[playerid][cycle][Text][0] = EOS;

            FixTextDrawString(text);

            if (YSI_CheckNumargs__(2)) {
                format(player_notification_td[playerid][cycle][Text], MAX_NTD_TEXT, text);
            } else {
                format(player_notification_td[playerid][cycle][Text], MAX_NTD_TEXT, text, ___(2));
            }

            player_notification_td[playerid][cycle][Use] = 1;
            player_notification_td[playerid][cycle][UseModelId] = 0;
            lines_notification_td(playerid, cycle);
            min_pos_notification_td(playerid, cycle);
            max_pos_notification_td(playerid, cycle);
            create_player_notification(playerid, cycle);

            if (IsValidAnimator(player_notification_td[playerid][cycle][Animator])) {
                PlayerText_StopMove(player_notification_td[playerid][cycle][Animator]);
            }
            
            KillTimer(player_notification_td[playerid][cycle][Timer]);
            player_notification_td[playerid][cycle][Timer] = SetTimerEx("player_notification_hide_timer", NTD_TIME, false, "i", playerid, cycle);
            return 1;
        }
    }
    return -1;
}
forward ShowPlayerNotificationModel(playerid, modelid, Float:set_rot_x, Float:set_rot_y, Float:set_rot_z, text[], GLOBAL_TAG_TYPES:...); 
public ShowPlayerNotificationModel(playerid, modelid, Float:set_rot_x, Float:set_rot_y, Float:set_rot_z, text[], GLOBAL_TAG_TYPES:...) {
    for(new cycle; cycle < MAX_NTD; cycle++) {
        if(!player_notification_td[playerid][cycle][Use]) {
            player_notification_td[playerid][cycle][Text][0] = EOS;

            FixTextDrawString(text);

            if (YSI_CheckNumargs__(6)) {
                format(player_notification_td[playerid][cycle][Text], MAX_NTD_TEXT, text);
            } else {
                format(player_notification_td[playerid][cycle][Text], MAX_NTD_TEXT, text, ___(6));
            }

            player_notification_td[playerid][cycle][Use] = 1;
            if (modelid != -1) player_notification_td[playerid][cycle][UseModelId] = modelid;
            else player_notification_td[playerid][cycle][UseModelId] = 0;
            player_notification_td[playerid][cycle][rot_x] = set_rot_x;
            player_notification_td[playerid][cycle][rot_y] = set_rot_y;
            player_notification_td[playerid][cycle][rot_z] = set_rot_z;
            lines_notification_td(playerid, cycle);
            min_pos_notification_td(playerid, cycle);
            max_pos_notification_td(playerid, cycle);
            create_player_notification(playerid, cycle);

            if (IsValidAnimator(player_notification_td[playerid][cycle][Animator])) {
                PlayerText_StopMove(player_notification_td[playerid][cycle][Animator]);
            }
            
            KillTimer(player_notification_td[playerid][cycle][Timer]);
            player_notification_td[playerid][cycle][Timer] = SetTimerEx("player_notification_hide_timer", NTD_TIME, false, "i", playerid, cycle);
            return 1;
        }
    }
    return -1;
}

/* Hooks */
hook OnScriptInit() {
    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++) {
        for(new NTD = 0; NTD < MAX_NTD; NTD++) {
            player_notification_td[playerid][NTD][TextDraw] = PlayerText:-1;
            player_notification_td[playerid][NTD][TextDrawModel] = PlayerText:-1;
            player_notification_td[playerid][NTD][Animator] = -1;
            selected_animator_cycle[playerid] = 0;
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}
hook OnPlayerDisconnect(playerid, reason) {
    for(new cycle; cycle < MAX_NTD; cycle++) {
        player_notification_td[playerid][cycle][Use] = 0;
        player_notification_td[playerid][cycle][Line] = 0;
        player_notification_td[playerid][cycle][Text][0] = EOS;
        player_notification_td[playerid][cycle][MinPosY] = 0;
        player_notification_td[playerid][cycle][MaxPosY] = 0;
        player_notification_td[playerid][cycle][MinModelPosY] = 0;
        player_notification_td[playerid][cycle][MaxModelPosY] = 0;
        player_notification_td[playerid][cycle][TextDraw] = PlayerText:-1;
        player_notification_td[playerid][cycle][TextDrawModel] = PlayerText:-1;
        KillTimer(player_notification_td[playerid][cycle][Timer]);
        player_notification_td[playerid][cycle][Timer] = -1;
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}