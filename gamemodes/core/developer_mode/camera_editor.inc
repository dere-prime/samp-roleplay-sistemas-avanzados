/*

    CameraEditor

*/

#include <YSI_Coding\y_hooks>

#define MAX_CAMS 100 //Edit this in case you want less/more cameras
#define white_col "{f0f0f0}"

//Variables down bellow
enum c_Info {
	c_Created, 
	c_Interpolated, 
	Float:c_Posx, 
	Float:c_Posy, 
	Float:c_Posz, 
	Float:c_Intposx, 
	Float:c_Intposy, 
	Float:c_Intposz, 
	c_Postime, 
	Float:c_Lookx, 
	Float:c_Looky, 
	Float:c_Lookz, 
	Float:c_Intlookx, 
	Float:c_Intlooky, 
	Float:c_Intlookz, 
	c_Looktime
};

new 
    DB: db_Cam,
    cams,
    Float: p_LastPos[MAX_PLAYERS],
    bool: Debug = false,
    editMode[MAX_PLAYERS],
    editId[MAX_PLAYERS] = -1,
    camInfo[MAX_CAMS][c_Info],
    Text3D: label_Pos[MAX_CAMS],
    obj_Pos[MAX_CAMS][2],
    obj_Look[MAX_CAMS][2]
;

//Forwards down bellow
forward LoadCamDb();
forward SaveCamera(camid, stuff);
forward CreateGizmos(camid);
forward DeleteGizmos(camid);
forward UpdateGizmo(gizmo, camid);
forward ToggleGizmos(bool:tog);
forward UpdateSpectator(playerid, camid);
forward Export(string[]);

hook OnScriptInit() {
	LoadCamDb();
	print("\n--------------------------------------");
	print(" !Camera Editor loaded!");
	print("--------------------------------------\n");
}

//Commands down bellow
CMD:newcam(playerid, params[]) {
	#pragma unused params
	cams ++;
	if(cams >= MAX_CAMS) { SendClientMessage(playerid, 0xfce679ff, "(!) Max cameras limit excedeed!"); cams --; return 1; }
	new query[512], Float:p_Pos[3]; GetPlayerPos(playerid, p_Pos[0], p_Pos[1], p_Pos[2]);
	format(query, sizeof(query), "INSERT INTO `cameras` (`camid`, `interpolated`, `posx`, `posy`, `posz`, `intposx`, `intposy`, `intposz`, `postime`, `lookx`, `looky`, `lookz`, `intlookx`, `intlooky`, `intlookz`, `looktime`) VALUES('%d', '%d', '%f', '%f', '%f', '%f', '%f', '%f', '%d', '%f', '%f', '%f', '%f', '%f', '%f', '%d')", 
	cams, 0, p_Pos[0], p_Pos[1], p_Pos[2], p_Pos[0], p_Pos[1], p_Pos[2], 1000, p_Pos[0]+5, p_Pos[1], p_Pos[2], p_Pos[0]+5, p_Pos[1], p_Pos[2], 1000);
	db_free_result(db_query(db_Cam, query));
	camInfo[cams][c_Interpolated] = 0; camInfo[cams][c_Postime] = 1000; camInfo[cams][c_Looktime] = 1000;
	
	camInfo[cams][c_Posx] = p_Pos[0]; camInfo[cams][c_Posy] = p_Pos[1]; camInfo[cams][c_Posz] = p_Pos[2];
	camInfo[cams][c_Intposx] = p_Pos[0]; camInfo[cams][c_Intposy] = p_Pos[1]; camInfo[cams][c_Intposz] = p_Pos[2];

	camInfo[cams][c_Lookx] = p_Pos[0]+5; camInfo[cams][c_Looky] = p_Pos[1]; camInfo[cams][c_Lookz] = p_Pos[2];
	camInfo[cams][c_Intlookx] = p_Pos[0]+5; camInfo[cams][c_Intlooky] = p_Pos[1]; camInfo[cams][c_Intlookz] = p_Pos[2];
	CreateGizmos(cams); camInfo[cams][c_Created] = 1;
	SendClientMessage(playerid, 0x98f442ff, "* New camera created.");
	return 1;
}

CMD:camint(playerid, params[]) {
	new camid;
	if(sscanf(params, "d", camid)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /camint [camera id]");
	if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
	editId[playerid] = camid;
	ShowPlayerDialog(playerid, DIALOG_CAMERAEDITOR, DIALOG_STYLE_LIST, "Edit Interpolation", "1. No interpolation\n2. Interpolate camera pos\n3. Interpolate camera look at\n4. Interpolate both", "Select", "Close");
	return 1;
}

CMD:editcam(playerid, params[]) {
	if(GetPlayerState(playerid) == 9) return SendClientMessage(playerid, 0xfce679ff, "(!) You can't edit in camera mode!");
	if(editMode[playerid] != 0) return SendClientMessage(playerid, 0xfce679ff, "(!) You are already editing something. Please finish editing or type /editstop!");
	new camid;
	if(sscanf(params, "d", camid)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /editcam [camera id]");
	if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
	if(!IsValidDynamicObject(obj_Pos[camid][0])) return SendClientMessage(playerid, 0xfce679ff, "(!) This object is not valid!");
	SetDynamicObjectRot(obj_Pos[camid][0], 0, 0, 0);
	new str[24];
	format(str, sizeof(str), "* Editing camera %d.", camid);
	SendClientMessage(playerid, 0x98f442ff, str);
	editMode[playerid] = 1;
	editId[playerid] = camid;
	EditDynamicObject(playerid, obj_Pos[camid][0]);
	return 1;
}

CMD:editlook(playerid, params[]) {
    if(GetPlayerState(playerid) == 9) return SendClientMessage(playerid, 0xfce679ff, "(!) You can't edit in camera mode!");
	if(editMode[playerid] != 0) return SendClientMessage(playerid, 0xfce679ff, "(!) You are already editing something. Please finish editing or type /editstop!");
	new camid;
	if(sscanf(params, "d", camid)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /editlook [camera id]");
	if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
	if(!IsValidDynamicObject(obj_Look[camid][0])) return SendClientMessage(playerid, 0xfce679ff, "(!) This object is not valid!");
	new str[24];
	format(str, sizeof(str), "* Editing look at from camera %d.", camid);
	SendClientMessage(playerid, 0x98f442ff, str);
	editMode[playerid] = 2;
	editId[playerid] = camid;
	EditDynamicObject(playerid, obj_Look[camid][0]);
	return 1;
}

CMD:ieditcam(playerid, params[]) {
    if(GetPlayerState(playerid) == 9) return SendClientMessage(playerid, 0xfce679ff, "(!) You can't edit in camera mode!");
	if(editMode[playerid] != 0) return SendClientMessage(playerid, 0xfce679ff, "(!) You are already editing something. Please finish editing or type /editstop!");
	new camid;
	if(sscanf(params, "d", camid)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /ieditcam [camera id]");
	if(camInfo[camid][c_Interpolated] == 1 || camInfo[camid][c_Interpolated] == 3) {
		if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
		if(!IsValidDynamicObject(obj_Pos[camid][1])) return SendClientMessage(playerid, 0xfce679ff, "(!) This object is not valid!");
		SetDynamicObjectRot(obj_Pos[camid][1], 0, 0, 0);
		new str[24];
		format(str, sizeof(str), "* Editing interpolated camera %d.", camid);
		SendClientMessage(playerid, 0x98f442ff, str);
		editMode[playerid] = 3;
		editId[playerid] = camid;
		EditDynamicObject(playerid, obj_Pos[camid][1]);
	}
	return 1;
}

CMD:ieditlook(playerid, params[]) {
    if(GetPlayerState(playerid) == 9) return SendClientMessage(playerid, 0xfce679ff, "(!) You can't edit in camera mode!");
	if(editMode[playerid] != 0) return SendClientMessage(playerid, 0xfce679ff, "(!) You are already editing something. Please finish editing or type /editstop!");
	new camid;
	if(sscanf(params, "d", camid)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /ieditlook [camera id]");
	if(camInfo[camid][c_Interpolated] == 2 || camInfo[camid][c_Interpolated] == 3) {
		if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
		if(!IsValidDynamicObject(obj_Look[camid][1])) return SendClientMessage(playerid, 0xfce679ff, "(!) This object is not valid!");
		
		new str[24];
		format(str, sizeof(str), "* Editing interpolated look at from camera %d.", camid);
		SendClientMessage(playerid, 0x98f442ff, str);
		editMode[playerid] = 4;
		editId[playerid] = camid;
		EditDynamicObject(playerid, obj_Look[camid][1]);
	}
	return 1;
}

CMD:camtime(playerid, params[]) {
	new camid, ammount;
	if(sscanf(params, "dd", camid, ammount)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /camtime [camera id][time(seconds)]");
	if(camInfo[camid][c_Interpolated] == 1 || camInfo[camid][c_Interpolated] == 3) {
	    if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
	    camInfo[camid][c_Postime] = ammount*1000;
	    SendClientMessage(playerid, 0x98f442ff, "* Camera position interpolation time updated.");
	    SaveCamera(camid, 0);
	}
	else SendClientMessage(playerid, 0xfce679ff, "(!) You camera needs to be in interpolation mode 2 or 4!");
	return 1;
}

CMD:looktime(playerid, params[]) {
	new camid, ammount;
	if(sscanf(params, "dd", camid, ammount)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /looktime [camera id][time(seconds)]");
	if(camInfo[camid][c_Interpolated] == 2 || camInfo[camid][c_Interpolated] == 3) {
	    if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
	    camInfo[camid][c_Looktime] = ammount*1000;
	    SendClientMessage(playerid, 0x98f442ff, "* Camera look at interpolation time updated.");
	    SaveCamera(camid, 0);
	}
	else SendClientMessage(playerid, 0xfce679ff, "(!) You camera needs to be in interpolation mode 3 or 4!");
	return 1;
}

CMD:playcam(playerid, params[]) {
	new camid;
	if(sscanf(params, "d", camid)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /playcam [camera id]");
	if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
	if(GetPlayerState(playerid) == 0) return SendClientMessage(playerid, 0xfce679ff, "(!) Invalid state!");
	if(GetPlayerState(playerid) != 9) {
		GetPlayerPos(playerid, p_LastPos[0], p_LastPos[1], p_LastPos[2]);
		TogglePlayerSpectating(playerid, true);
		ToggleGizmos(false);
	}
	new str[26];
	format(str, sizeof(str), "* Playing camera %d...", camid);
	SendClientMessage(playerid, 0x98f442ff, str);
	SetTimerEx("UpdateSpectator", 1000, false, "dd", playerid, camid);
	
	return 1;
}

CMD:stopcam(playerid, params[]) {
	#pragma unused params
	if(GetPlayerState(playerid) == 9) {
	    TogglePlayerSpectating(playerid, false);
	    ToggleGizmos(true);
	    SetTimerEx("UpdateSpectator", 1000, false, "dd", playerid, 0);
	}
	return 1;
}

CMD:exportcam(playerid, params[]) {
	new camid;
	if(sscanf(params, "d", camid)) return SendClientMessage(playerid, 0xfce679ff, "(!) Please type: /exportcam [camera id]");
	if(camid < 1 || camid > cams) return SendClientMessage(playerid, 0xfce679ff, "(!) This camera does not exist!");
	
	new str[256];
	format(str, sizeof(str), "InterpolateCameraPos(playerid, %f, %f, %f, %f, %f, %f, 1000, CAMERA_CUT);", camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz], camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz]);
	Export(str);
	format(str, sizeof(str), "InterpolateCameraLookAt(playerid, %f, %f, %f, %f, %f, %f, 1000, CAMERA_CUT);", camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz]);
	Export(str);

	if(camInfo[camid][c_Interpolated] == 1) {
 		format(str, sizeof(str), "InterpolateCameraPos(playerid, %f, %f, %f, %f, %f, %f, %d, CAMERA_MOVE);", camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz], camInfo[camid][c_Intposx], camInfo[camid][c_Intposy], camInfo[camid][c_Intposz], camInfo[camid][c_Postime]);
 		Export(str);
	} else if(camInfo[camid][c_Interpolated] == 2) {
 		format(str, sizeof(str), "InterpolateCameraLookAt(playerid, %f, %f, %f, %f, %f, %f, %d, CAMERA_MOVE);", camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], camInfo[camid][c_Looktime]);
 		Export(str);
	} else if(camInfo[camid][c_Interpolated] == 3) {
 		format(str, sizeof(str), "InterpolateCameraPos(playerid, %f, %f, %f, %f, %f, %f, %d, CAMERA_MOVE);", camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz], camInfo[camid][c_Intposx], camInfo[camid][c_Intposy], camInfo[camid][c_Intposz], camInfo[camid][c_Postime]);
 		Export(str);
   		format(str, sizeof(str), "InterpolateCameraLookAt(playerid, %f, %f, %f, %f, %f, %f, %d, CAMERA_MOVE);", camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], camInfo[camid][c_Looktime]);
 		Export(str);
	}
	Export("=================================================================");
	SendClientMessage(playerid, 0x98f442ff, "* Camera code lines exported. Please check scriptfiles/cameraexports.txt!");
	return 1;
}

CMD:chelp(playerid, params[]) {
	#pragma unused params
	ShowPlayerDialog(playerid, DIALOG_CAMERAEDITOR+1, DIALOG_STYLE_LIST, "Camera Help", "1./newcam\n2./camint\n3./editcam\n4./editlook\n5./ieditcam\n6./ieditlook\n7./camtime\n8./looktime\n9./playcam\n10./stopcam\n11./exportcam\n12./editstop", "Select", "Cancel");
	return 1;
}

CMD:editstop(playerid, params[]) {
	#pragma unused params
	SendClientMessage(playerid, 0x98f442ff, "* Camera editor has been stoped.");
	editMode[playerid] = 0;
	return 1;
}

//Callbacks down bellow
public LoadCamDb() {
	//Creating database if does not exist
	db_Cam = db_open("camdb.db");
	db_free_result(db_query(db_Cam, "CREATE TABLE IF NOT EXISTS `cameras` (`camid`, `interpolated`, `posx`, `posy`, `posz`, `intposx`, `intposy`, `intposz`, `postime`, `lookx`, `looky`, `lookz`, `intlookx`, `intlooky`, `intlookz`, `looktime`)"));
	
	//Loading the data
	for(new i=1; i < MAX_CAMS; i++) {
	    new query[48], DBResult:res;
 		format(query, sizeof(query), "SELECT * FROM `cameras` WHERE `camid` = '%d'", i);
        res = db_query(db_Cam, query);
        
        if(db_num_rows(res)) {
            cams ++;
            new field[50];
            
            db_get_field_assoc(res, "interpolated", field, 50 );
            camInfo[i][c_Interpolated] = strval(field);
            db_get_field_assoc(res, "postime", field, 50 );
            camInfo[i][c_Postime] = strval(field);
            db_get_field_assoc(res, "looktime", field, 50 );
            camInfo[i][c_Looktime] = strval(field);
            
            db_get_field_assoc(res, "posx", field, 50 );
            camInfo[i][c_Posx] = floatstr(field);
            db_get_field_assoc(res, "posy", field, 50 );
            camInfo[i][c_Posy] = floatstr(field);
            db_get_field_assoc(res, "posz", field, 50 );
            camInfo[i][c_Posz] = floatstr(field);
            db_get_field_assoc(res, "intposx", field, 50 );
            camInfo[i][c_Intposx] = floatstr(field);
            db_get_field_assoc(res, "intposy", field, 50 );
            camInfo[i][c_Intposy] = floatstr(field);
            db_get_field_assoc(res, "intposz", field, 50 );
            camInfo[i][c_Intposz] = floatstr(field);

            db_get_field_assoc(res, "lookx", field, 50 );
            camInfo[i][c_Lookx] = floatstr(field);
            db_get_field_assoc(res, "looky", field, 50 );
            camInfo[i][c_Looky] = floatstr(field);
            db_get_field_assoc(res, "lookz", field, 50 );
            camInfo[i][c_Lookz] = floatstr(field);
            db_get_field_assoc(res, "intlookx", field, 50 );
            camInfo[i][c_Intlookx] = floatstr(field);
            db_get_field_assoc(res, "intlooky", field, 50 );
            camInfo[i][c_Intlooky] = floatstr(field);
            db_get_field_assoc(res, "intlookz", field, 50 );
            camInfo[i][c_Intlookz] = floatstr(field);
            camInfo[i][c_Created] = 1;
            CreateGizmos(i);
            
			if(Debug)
			    printf("[Debug]:Camera %d loaded.", i);
        }
        else break;
	}
	return 1;
}

public SaveCamera(camid, stuff) {
	if(stuff == 0) {
		new query[128];
		format(query, sizeof(query), "UPDATE `cameras` SET `interpolated` = '%d', `postime` = '%d', `looktime` = '%d' WHERE `camid` = '%d'", 
		camInfo[camid][c_Interpolated], camInfo[camid][c_Postime], camInfo[camid][c_Looktime], camid);
		db_free_result(db_query(db_Cam, query));
	}
	else if(stuff == 1) {
	    new query[256];
		format(query, sizeof(query), "UPDATE `cameras` SET `posx` = '%f', `posy` = '%f', `posz` = '%f', `intposx` = '%f', `intposy` = '%f', `intposz` = '%f' WHERE `camid` = '%d'", 
		camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz], camInfo[camid][c_Intposx], camInfo[camid][c_Intposy], camInfo[camid][c_Intposz], camid);
		db_free_result(db_query(db_Cam, query));
	}
	else if(stuff == 2) {
	    new query[256];
		format(query, sizeof(query), "UPDATE `cameras` SET `lookx` = '%f', `looky` = '%f', `lookz` = '%f', `intlookx` = '%f', `intlooky` = '%f', `intlookz` = '%f' WHERE `camid` = '%d'", 
		camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], camid);
		db_free_result(db_query(db_Cam, query));
	}
	return 1;
}

public CreateGizmos(camid) {
	if(!IsValidDynamicObject(obj_Pos[camid][0])) obj_Pos[camid][0] = CreateDynamicObject(367, camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz], 0, 0, 0, -1, -1, -1, 300, 300);
    if(!IsValidDynamicObject(obj_Look[camid][0]))obj_Look[camid][0] = CreateDynamicObject(1318, camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], 0, 0, 0);
    SetObjectFaceCoords3D(obj_Pos[camid][0], camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], 0, -90, 90);
    
    if(!IsValidDynamic3DTextLabel(label_Pos[camid]))
    {
	    new str[32];
	    format(str, sizeof(str), "cam Id: "white_col"%d", camid);
	    label_Pos[camid] = CreateDynamic3DTextLabel(str, 0x98f442ff, camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz], 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 5.0);
	}
	
	if(camInfo[camid][c_Interpolated] == 1)//If camera interpolates the position
 	{
  		obj_Pos[camid][1] = CreateDynamicObject(367, camInfo[camid][c_Intposx], camInfo[camid][c_Intposy], camInfo[camid][c_Intposz], 0, 0, 0, -1, -1, -1, 300, 300);
    	SetDynamicObjectMaterial(obj_Pos[camid][1], 0, -1, "none", "none", 0xFFFF0000);
     	SetObjectFaceCoords3D(obj_Pos[camid][1], camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], 0, -90, 90);
	}
	else if(camInfo[camid][c_Interpolated] == 2)//If camera interpolates the look at
 	{
  		obj_Look[camid][1] = CreateDynamicObject(1318, camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], 0, 0, 0, -1, -1, -1, 300, 300);
    	SetDynamicObjectMaterial(obj_Look[camid][1], 0, -1, "none", "none", 0xFFFF0000);
	}
	else if(camInfo[camid][c_Interpolated] == 3)//If camera interpolates the position and look at`
 	{
  		obj_Pos[camid][1] = CreateDynamicObject(367, camInfo[camid][c_Intposx], camInfo[camid][c_Intposy], camInfo[camid][c_Intposz], 0, 0, 0, -1, -1, -1, 300, 300);
    	SetDynamicObjectMaterial(obj_Pos[camid][1], 0, -1, "none", "none", 0xFFFF0000);
     	SetObjectFaceCoords3D(obj_Pos[camid][1], camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], 0, -90, 90);
      	obj_Look[camid][1] = CreateDynamicObject(1318, camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], 0, 0, 0, -1, -1, -1, 300, 300);
       	SetDynamicObjectMaterial(obj_Look[camid][1], 0, -1, "none", "none", 0xFFFF0000);
	}
	if(Debug)
		printf("[Debug]: All the gizmos for camera %d got created.", camid);
	return 1;
}

public DeleteGizmos(camid) {
    DestroyDynamicObject(obj_Pos[camid][0]);
    DestroyDynamicObject(obj_Look[camid][0]);
    DestroyDynamic3DTextLabel(label_Pos[camid]);
    if(camInfo[camid][c_Interpolated] == 1) DestroyDynamicObject(obj_Pos[camid][1]);
    if(camInfo[camid][c_Interpolated] == 2) DestroyDynamicObject(obj_Look[camid][1]);
    if(camInfo[camid][c_Interpolated] == 3) DestroyDynamicObject(obj_Pos[camid][1]), DestroyDynamicObject(obj_Look[camid][1]);
    if(Debug)
        printf("[Debug]:All the gizmos for camera %d got destroyed.", camid);
	return 1;
}

public UpdateGizmo(gizmo, camid) {
	if(gizmo == 0) {
    	SetObjectFaceCoords3D(obj_Pos[camid][0], camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], 0, -90, 90);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, label_Pos[camid], E_STREAMER_X, camInfo[camid][c_Posx]);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, label_Pos[camid], E_STREAMER_Y, camInfo[camid][c_Posy]);
        Streamer_SetFloatData(STREAMER_TYPE_3D_TEXT_LABEL, label_Pos[camid], E_STREAMER_Z, camInfo[camid][c_Posz]);
    	if(Debug)
	    	printf("[Debug]:Gizmos %d from camera %d updated.", gizmo, camid);
	}
	else if(gizmo == 1) {
 		if(camInfo[camid][c_Interpolated] == 1)SetObjectFaceCoords3D(obj_Pos[camid][1], camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], 0, -90, 90);
   		else if(camInfo[camid][c_Interpolated] == 3)SetObjectFaceCoords3D(obj_Pos[camid][1], camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], 0, -90, 90);
    	if(Debug)
    		printf("[Debug]:Gizmos %d from camera %d updated.", gizmo, camid);
	}
	return 1;
}

public ToggleGizmos(bool:tog) {
	if(tog) {
	    for(new i = 0; i < MAX_CAMS; i++)
	    {
	        if(camInfo[i][c_Created] == 1)
	        	CreateGizmos(i);
	    }
	}
	else
	{
	    for(new i = 0; i < MAX_CAMS; i++)
	    {
            if(camInfo[i][c_Created] == 1)
	        	DeleteGizmos(i);
	    }
	}
	return 1;
}

public UpdateSpectator(playerid, camid) {
	if(GetPlayerState(playerid) == 9) {
		SetPlayerCameraPos(playerid, camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz]);
		SetPlayerCameraLookAt(playerid, camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz]);
		if(camInfo[camid][c_Interpolated] == 1) {
		    InterpolateCameraPos(playerid, camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz], camInfo[camid][c_Intposx], camInfo[camid][c_Intposy], camInfo[camid][c_Intposz], camInfo[camid][c_Postime], CAMERA_MOVE);
		    //InterpolateCameraLookAt(playerid, camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], camInfo[camid][c_Looktime], CAMERA_MOVE);
		}
		else if(camInfo[camid][c_Interpolated] == 2) {
		    InterpolateCameraLookAt(playerid, camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], camInfo[camid][c_Looktime], CAMERA_MOVE);
		}
		else if(camInfo[camid][c_Interpolated] == 3) {
		    InterpolateCameraPos(playerid, camInfo[camid][c_Posx], camInfo[camid][c_Posy], camInfo[camid][c_Posz], camInfo[camid][c_Intposx], camInfo[camid][c_Intposy], camInfo[camid][c_Intposz], camInfo[camid][c_Postime], CAMERA_MOVE);
		    InterpolateCameraLookAt(playerid, camInfo[camid][c_Lookx], camInfo[camid][c_Looky], camInfo[camid][c_Lookz], camInfo[camid][c_Intlookx], camInfo[camid][c_Intlooky], camInfo[camid][c_Intlookz], camInfo[camid][c_Looktime], CAMERA_MOVE);
  		}
 	}
	else SetPlayerPos(playerid, p_LastPos[0], p_LastPos[1], p_LastPos[2]), SetCameraBehindPlayer(playerid);
	return 1;
}

public Export(string[]) {
    new entry[256];
    format(entry, sizeof(entry), "%s\n", string);
    new File:hFile;
    hFile = fopen("cameraexports.txt", io_append);
    fwrite(hFile, entry);
    fclose(hFile);
}

hook OnPlayerConnect(playerid) {
	editMode[playerid] = 0;
	editId[playerid] = -1;
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	if(dialogid == DIALOG_CAMERAEDITOR) {
	    if(!response) return editId[playerid] = -1;
	    else{
	        SendClientMessage(playerid, 0x98f442ff, "* Interpolation updated.");
	        new camid = editId[playerid];
			if(listitem == 0) {
			    DeleteGizmos(camid);
	  			camInfo[camid][c_Interpolated] = 0;
	  			SaveCamera(camid, 0);
	  			CreateGizmos(camid);
			}
			else if(listitem == 1) {
			    DeleteGizmos(camid);
   				camInfo[camid][c_Interpolated] = 1;
   				SaveCamera(camid, 0);
	        	CreateGizmos(camid);
			}
			else if(listitem == 2) {
			    DeleteGizmos(camid);
				camInfo[camid][c_Interpolated] = 2;
				SaveCamera(camid, 0);
    			CreateGizmos(camid);
			}
			else if(listitem == 3) {
			    DeleteGizmos(camid);
   				camInfo[camid][c_Interpolated] = 3;
   				SaveCamera(camid, 0);
	        	CreateGizmos(camid);
			}
	    }
        return Y_HOOKS_BREAK_RETURN_1;
	}

	if(dialogid == DIALOG_CAMERAEDITOR + 1) {
	    if(response) {
	    	if(listitem == 0) SendClientMessage(playerid, 0xfce679ff, "» /newcam - This command is used to create a new camera.");
	    	else if(listitem == 1) SendClientMessage(playerid, 0xfce679ff, "» /camint - This will let you change the interpolation state of your camera.");
			else if(listitem == 2) SendClientMessage(playerid, 0xfce679ff, "» /editcam - This will display the edit UI, wich you can use to change camera's position.");
			else if(listitem == 3) SendClientMessage(playerid, 0xfce679ff, "» /editlook - This will display the edit UI, wich you can use to change the position where your camera is looking.");
			else if(listitem == 4) SendClientMessage(playerid, 0xfce679ff, "» /ieditcam - This will display the edit UI, wich you can use to change interpolated camera's position.");
			else if(listitem == 5) SendClientMessage(playerid, 0xfce679ff, "» /ieditlook - This will display the edit UI, wich you can use to change the interpolated position where your camera is gonna look.");
			else if(listitem == 6) SendClientMessage(playerid, 0xfce679ff, "» /camtime - This edits the camera position interpolation time.");
			else if(listitem == 7) SendClientMessage(playerid, 0xfce679ff, "» /looktime - This edits the camera look at interpolation time.");
			else if(listitem == 8) SendClientMessage(playerid, 0xfce679ff, "» /playcam - This will play the camera animation.");
			else if(listitem == 9) SendClientMessage(playerid, 0xfce679ff, "» /stopcam - This will get you out of camera mode.");
			else if(listitem == 10) SendClientMessage(playerid, 0xfce679ff, "» /exportcam - This will export the code lines you need to implement a camera animation in your script.");
			else if(listitem == 11) SendClientMessage(playerid, 0xfce679ff, "» /editstop - In case the camera edit is stuck on, this will help fix the problem.");
	    }
        return Y_HOOKS_BREAK_RETURN_1;
	}

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEditDynObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz) {
	if(editMode[playerid] != 0) {
		new Float:oldPos[3]; GetDynamicObjectPos(objectid, oldPos[0], oldPos[1], oldPos[2]);
	    if(response == 0) { SetDynamicObjectPos(objectid, oldPos[0], oldPos[1], oldPos[2]); UpdateGizmo(0, editId[playerid]); UpdateGizmo(1, editId[playerid]); editMode[playerid] = 0; editId[playerid] = -1; SendClientMessage(playerid, 0x98f442ff, "* Edit mode canceld."); }
	    if(response == 1) {
	        if(editMode[playerid] == 1) {
	            new camid = editId[playerid];
	            SetDynamicObjectPos(objectid, x, y, z);
	            camInfo[camid][c_Posx] = x; camInfo[camid][c_Posy] = y; camInfo[camid][c_Posz] = z;
	            SendClientMessage(playerid, 0x98f442ff, "* Camera position updated.");
	            UpdateGizmo(0, camid);
	            SaveCamera(camid, 1);
	            editMode[playerid] = 0; editId[playerid] = -1;
	        }
	        else if(editMode[playerid] == 2) {
	            new camid = editId[playerid];
	            SetDynamicObjectPos(objectid, x, y, z);
	            camInfo[camid][c_Lookx] = x; camInfo[camid][c_Looky] = y; camInfo[camid][c_Lookz] = z;
	            SendClientMessage(playerid, 0x98f442ff, "* Camera look at updated.");
	            UpdateGizmo(0, camid);
	            SaveCamera(camid, 2);
	            editMode[playerid] = 0; editId[playerid] = -1;
	        }
	        else if(editMode[playerid] == 3) {
	            new camid = editId[playerid];
	            SetDynamicObjectPos(objectid, x, y, z);
	            camInfo[camid][c_Intposx] = x; camInfo[camid][c_Intposy] = y; camInfo[camid][c_Intposz] = z;
	            SendClientMessage(playerid, 0x98f442ff, "* Interpolated camera position updated.");
	            UpdateGizmo(1, camid);
	            SaveCamera(camid, 1);
	            editMode[playerid] = 0; editId[playerid] = -1;
	        }
	        else if(editMode[playerid] == 4) {
	            new camid = editId[playerid];
	            SetDynamicObjectPos(objectid, x, y, z);
	            camInfo[camid][c_Intlookx] = x; camInfo[camid][c_Intlooky] = y; camInfo[camid][c_Intlookz] = z;
	            SendClientMessage(playerid, 0x98f442ff, "* Camera interpolated look at updated.");
	            UpdateGizmo(1, camid);
	            SaveCamera(camid, 2);
	            editMode[playerid] = 0; editId[playerid] = -1;
	        }
	    }
		return Y_HOOKS_BREAK_RETURN_1;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//Stocks down bellow
stock SetObjectFaceCoords3D(iObject, Float: fX, Float: fY, Float: fZ, Float: fRollOffset = 0.0, Float: fPitchOffset = 0.0, Float: fYawOffset = 0.0) {
	new
		Float: fOX, 
		Float: fOY, 
		Float: fOZ, 
		Float: fPitch
	;
	GetDynamicObjectPos(iObject, fOX, fOY, fOZ);

	fPitch = floatsqroot(floatpower(fX - fOX, 2.0) + floatpower(fY - fOY, 2.0));
	fPitch = floatabs(atan2(fPitch, fZ - fOZ));

	fZ = atan2(fY - fOY, fX - fOX) - 90.0; // Yaw

	SetDynamicObjectRot(iObject, fRollOffset, fPitch + fPitchOffset, fZ + fYawOffset);
}