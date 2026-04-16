/*

	Register
		Module

*/

#include <YSI_Coding\y_hooks>

forward OnPlayerAbsoluteFinishTA(playerid);

#define WELCOME_STRING \
	"{"#PRIMARY_COLOR"}Bienvenido a {"#SECONDARY_COLOR"}"SERVER_NAME"{FFFFFF}\n\n\
	Gracias por elegir jugar aquí. Como viste en la cinemática, no eres de este lugar.\n\
	No conoces la ciudad, no tienes pertenencias y solo cuentas con un poco de dinero\n\
	entregado por las personas del faro.\n\
	\n\
	Empiezas desde cero, elige bien tu camino…\n\
	{"#WARNING_COLOR"}pero recuerda:{FFFFFF} tu pasado te persigue y algunos acontecimientos lo demostrarán.\n\
	\n\
	{"#PRIMARY_COLOR"}INFORMACIÓN ROLEPLAY{FFFFFF}\n\
	Este es un servidor {"#BLUE_COLOR"}RolePlay{FFFFFF}. Puedes jugar RP normalmente, sin embargo,\n\
	algunas misiones y cinemáticas están fuera del contexto del RP,\n\
	ya que se enfocan en tu pasado antes de llegar a la ciudad.\n\
	\n\
	{"#PRIMARY_COLOR"}HAMBRE Y SED{FFFFFF} abajo a la derecha de la pantalla\n\
	• Hambre: icono de {"#GREEN_COLOR"}hamburguesa{FFFFFF}.\n\
	• Sed: icono de {"#BLUE_COLOR"}vaso{FFFFFF}.\n\
	Si bajan demasiado, perderás salud.\n\
	\n\
	{"#PRIMARY_COLOR"}CONTROLES RÁPIDOS{FFFFFF}\n\
	• Inventario: pulsa {"#SECONDARY_COLOR"}N{FFFFFF}\n\
	• Menú de interacción: pulsa {"#SECONDARY_COLOR"}Y{FFFFFF}\n\
	\n\
	{"#PRIMARY_COLOR"}MENÚ DE INTERACCIÓN{FFFFFF}\n\
	Configuraciones básicas, información de cuenta,\n\
	interacción con jugadores e inventario rápido sin comandos.\n\n\
	{"#PRIMARY_COLOR"}COMANDOS ÚTILES{FFFFFF}\n\
	/consumir\n\
	/telefono | /tlf | /movil | /celular\n\
	/gps\n\
	\n\
	{"#PRIMARY_COLOR"}OTROS{FFFFFF}\n\
	• Accesorios: /accesorios\n\
	\n\
	{"#GREEN_COLOR"}¡Eso es todo, disfruta y a jugar!{FFFFFF}\n\
	{666666}Debes esperar 30 segundos para cerrar este mensaje."


/* REG CINEMATIC UPDATE */
#define UpdateRegCinematic(%0,%1) KillTimer(PLAYER_TEMP[%0][pt_REGISTER_UPDATES_TIMER]); PLAYER_TEMP[%0][pt_REGISTER_UPDATES_TIMER]=SetTimerEx("UpdateRegisterCinematic",%1,false,"i",%0)

//....
new WelcomeMessageTime[MAX_PLAYERS];

/* REGISTER CALLBACK */
public OnPlayerRegister(playerid) {
	if (PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] != -1) {
		KillTimer(PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER]);
		PLAYER_TEMP[playerid][pt_LOGIN_KICK_TIMER] = -1;
	}
	PLAYER_TEMP[playerid][pt_USER_LOGGED] = true;

	TogglePlayerSpectatingEx(playerid, false);
	StopAudioStreamForPlayer(playerid);
	ClearPlayerChat(playerid);
	TogglePlayerControllable(playerid, false);
	return 1;
}

/* REGISTER DIALOG */
DialogCreate:PlayerRegisterInputPass(playerid) {
	Dialog_Open(
		playerid, Dialog:PlayerRegisterInputPass, DIALOG_STYLE_INPUT, 
		""SERVER_NAME_FORMAT" · Registrarse", 
		"\
			{FFFFFF}Bienvenido, este usuario no esta registrado.\n\
			\n\
			Si tiene una {"#WARNING_COLOR"}cuenta registrada{FFFFFF} siga estos pasos:\n\
			{"#SECONDARY_COLOR"}• {FFFFFF}Verifique su {"#ERROR_COLOR"}nombre de usuario{FFFFFF} e intente una vez mas.\n\
			{"#SECONDARY_COLOR"}• {FFFFFF}Si todo esta correcto, contacta con el soporte administrativo a traves de discord.\n\
			\n\
			Para crear tu cuenta asegurate de que la clave {"#WARNING_COLOR"}cumpla los siguientes requisitos{FFFFFF}:\n\
			{"#SECONDARY_COLOR"}• {FFFFFF}Caracteres minimos: {"#WARNING_COLOR"}"#MIN_PASS_LENGTH"\n\
			{"#SECONDARY_COLOR"}• {FFFFFF}Caracteres maximos: {"#WARNING_COLOR"}"#MAX_PASS_LENGTH"\n\
		",
		"»", "-"
	);
}
DialogResponse:PlayerRegisterInputPass(playerid, response, listitem, inputtext[]) {
	if (response) {
		if (strlen(inputtext) < MIN_PASS_LENGTH || strlen(inputtext) > MAX_PASS_LENGTH) return Dialog_Show(playerid, Dialog:PlayerRegisterInputPass);
		format(PLAYER_TEMP[playerid][pt_PASSWD], 24, "%s", inputtext);
		
		new salt[16];
		getRandomSalt(salt);
		format(PI[playerid][pi_SALT], 16, "%s", salt);
		SHA256_PassHash(inputtext, PI[playerid][pi_SALT], PI[playerid][pi_PASS], 64 + 1);

		Dialog_Show(playerid, Dialog:PlayerRegisterInputEmail);
	}
	else {
		Dialog_Message(playerid, DIALOG_STYLE_MSGBOX, ""SERVER_NAME_FORMAT" · Boton de salir", "{FFFFFF}Ha cancelado el registro de su cuenta.\n\nSi desea registrarse en otro momento\npuede hacerlo reiniciando el proceso de conexion al servidor.", "Salir");
		KickEx(playerid, 500);
	}
	return 1;
}

/* REGISTER DIALOG EMAIL */
DialogCreate:PlayerRegisterInputEmail(playerid) {
	Dialog_Open(
		playerid, Dialog:PlayerRegisterInputEmail, DIALOG_STYLE_INPUT, 
		""SERVER_NAME_FORMAT" · Correo Electronico", 
		"{FFFFFF}Necesitamos un {"#SECONDARY_COLOR"}correo electrónico{FFFFFF} válido.\n\
		\n\
		El correo electrónico es necesario para:\n\
		{"#SECONDARY_COLOR"}•{FFFFFF} Recuperar la clave en caso de olvido.\n\
		{"#SECONDARY_COLOR"}•{FFFFFF} Enviar notificaciones importantes sobre su cuenta.\n\
		\n\
		{cccccc}Ingrese su correo electrónico a continuación:",
		"»", "-"
	);
}
DialogResponse:PlayerRegisterInputEmail(playerid, response, listitem, inputtext[]) {
	if (response) {
		new mail[32];
		format(mail, 32, "%s", inputtext);
		if (!IsValidEmail(mail)) {
			ShowPlayerMessage(playerid, "El correo no es válido, intente de nuevo.");
			Dialog_Show(playerid, Dialog:PlayerRegisterInputEmail);
			return 1;	
		}

		inline OnCheckEmail() {
			new rows;
			if (cache_get_row_count(rows)) {
				if (rows) {
					ShowPlayerMessage(playerid, "Este correco electrónico ya está en uso, use otro distinto.");
					Dialog_Show(playerid, Dialog:PlayerRegisterInputEmail);
				} else {
					format(PI[playerid][pi_EMAIL], 32, "%s", mail);
					for(new i = 0; i < LOAD_SCREEN_TEXTDRAWS_SIZE; i ++) {
						TextDrawHideForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][i]);
					}

					UpdateRegCinematic(playerid, 700);
				}
			}
			else Kick(playerid);
		}
		mysql_format(handle_db, QUERY_BUFFER, sizeof QUERY_BUFFER, "SELECT id FROM player WHERE email = '%e';", mail);
		MySQL_TQueryInline(handle_db, using inline OnCheckEmail, QUERY_BUFFER);
	} else {
		Dialog_Show(playerid, Dialog:PlayerRegisterInputPass);
	}
	return 1;
}

/* REGISTER CINEMATIC UPDATE */
forward UpdateRegisterCinematic(const playerid);
public UpdateRegisterCinematic(const playerid) {
	switch (PLAYER_TEMP[playerid][pt_REGISTER_UPDATES]) {
		case 0: {
			InterpolateCameraPos(playerid, 221.651855, -1900.747680, 43.653434, 221.651855, -1900.747680, 43.653434, 1000, CAMERA_CUT);
			InterpolateCameraLookAt(playerid, 154.542098, -1952.221923, 24.063442, 154.542098, -1952.221923, 24.063442, 1000, CAMERA_CUT);
			InterpolateCameraPos(playerid, 221.651855, -1900.747680, 43.653434, 154.141998, -1946.399414, 5.633438, 5000, CAMERA_MOVE);
			InterpolateCameraLookAt(playerid, 154.542098, -1952.221923, 24.063442, 154.201995, -1948.101684, 5.603432, 3000, CAMERA_MOVE);

			PLAYER_TEMP[playerid][pt_REGISTER_UPDATES] ++;
			UpdateRegCinematic(playerid, 4000);
		}
		case 1: {
			PlayerTextDrawHide(playerid, PlayerTextdraws[playerid][ptextdraw_BLACK_SCREEN]);
			TextDrawShowForPlayer(playerid, Textdraws[textdraw_BLACK_SCREEN]);
			InterpolateCameraPos(playerid, 2723.763183, 2740.195068, 11.741271, 2723.763183, 2740.195068, 11.741271, 1000, CAMERA_CUT);
			InterpolateCameraLookAt(playerid, 2723.793701, 2743.870849, 10.571274, 2723.793701, 2743.870849, 10.571274, 1000, CAMERA_CUT);
			SetPlayerInterior(playerid, 1);
			SetPlayerVirtualWorld(playerid, playerid + 10);

			PLAYER_TEMP[playerid][pt_REGISTER_UPDATES] ++;
			UpdateRegCinematic(playerid, 2000);
		}
		case 2: {
			PLAYER_TEMP[playerid][pt_REGISTER_UPDATES] = 0;
			KillTimer(PLAYER_TEMP[playerid][pt_REGISTER_UPDATES_TIMER]);
			PLAYER_TEMP[playerid][pt_REGISTER_UPDATES_TIMER] = -1;

			for(new i = 0; i < LOAD_SCREEN_TEXTDRAWS_SIZE; i ++) {
				TextDrawHideForPlayer(playerid, Textdraws[textdraw_LOAD_SCREEN][i]);
			}
			HidePlayerBlackScreen(playerid);

			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][0]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][1]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][2]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][3]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][4]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][5]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][6]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][7]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][8]);
			PlayerTextDrawShow(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9]);
			
			new fmat_text[64];
			format(fmat_text, 24, "%s_NPC", PLAYER_TEMP[playerid][pt_NAME]);
			PLAYER_TEMP[playerid][pt_REGISTER_NPC] = FCNPC_Create(fmat_text);
			FCNPC_Spawn(PLAYER_TEMP[playerid][pt_REGISTER_NPC], 0, 2726.1958, 2742.5093, 10.8313);
			FCNPC_SetInterior(PLAYER_TEMP[playerid][pt_REGISTER_NPC], 1);
			FCNPC_SetVirtualWorld(PLAYER_TEMP[playerid][pt_REGISTER_NPC], playerid + 10);
			FCNPC_SetInvulnerable(PLAYER_TEMP[playerid][pt_REGISTER_NPC]);

			PLAYER_TEMP[ PLAYER_TEMP[playerid][pt_REGISTER_NPC] ][pt_REGISTER_NPC_PLAYER] = playerid;

			PI[playerid][pi_GENDER] = -1;
			PLAYER_TEMP[playerid][pt_REGISTER_SELECTING_SKIN] = true;
			PLAYER_TEMP[playerid][pt_REGISTER_SELECTED_SKIN] = 0;
		
			PlayerTextDrawSetString(playerid, PlayerTextdraws[playerid][ptextdraw_REGISTER_SKIN][9], "_");
			
			SendPlayerClientMessage(playerid, "Elige tu personaje, presionar 'continuar' para finalizar.");
			SelectTextDrawEx(playerid, 0xe8d08fFF);
		}
	}
}

/* Show Player Welcome Message */
DialogCreate:WelcomeMessageDialog(playerid) {
	Dialog_Open(
		playerid, Dialog:WelcomeMessageDialog, DIALOG_STYLE_MSGBOX, 
		""SERVER_NAME_FORMAT" · Bienvenido", 
		WELCOME_STRING,
		"Cerrar", ""
	);
}
DialogResponse:WelcomeMessageDialog(playerid, response, listitem, inputtext[]) {
	if (gettime() < WelcomeMessageTime[playerid]) {
		Dialog_Show(playerid, Dialog:WelcomeMessageDialog);
	}
	return 1;
}
ShowPlayerWelcomeMessage(playerid) {
	WelcomeMessageTime[playerid] = gettime() + 30; // 30 segundos
	Dialog_Show(playerid, Dialog:WelcomeMessageDialog);
}
hook OnPlayerUpdate(playerid) {
	if (PLAYER_TEMP[playerid][pt_USER_LOGGED]) {
		if (gettime() < WelcomeMessageTime[playerid]) {
			if (!Dialog_IsOpen(playerid, Dialog:WelcomeMessageDialog)) {
				Dialog_Show(playerid, Dialog:WelcomeMessageDialog);
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}