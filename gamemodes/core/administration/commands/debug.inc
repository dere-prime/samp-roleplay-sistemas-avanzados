/* 

    Debug Production Commands
        Administration

*/

CMD:psound(playerid, params[]) {
	new sound;
	if (sscanf(params, "d", sound)) return ShowPlayerMessage(playerid, "Parametros incorrectos, usa ~y~/psound <ID>");
	PlayerPlaySoundEx(playerid, sound, 0.0, 0.0, 0.0);
	CmdLog("psound");
	return 1;
}