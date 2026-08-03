/*

    Global Help System
        Prime Roleplay

*/

/* ETC */
static DIALOG_CREDIT_String[2000];

/* GAME HELP */
CMD:ayuda(playerid, params[]) {
    switch (YHash(params, false)) {
        case _I<general>: Dialog_Show(playerid, Dialog:PlayerHelpGeneral);
        case _I<informacion>: Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Informacion", DIALOG_CREDIT_String, "Volver", "");
        case _I<propiedades>: Dialog_Show(playerid, Dialog:PlayerHelpProperties);
        case _I<reportar>: Dialog_Show(playerid, Dialog:PlayerHelpReport);
        case _I<trabajos>: Dialog_Show(playerid, Dialog:PlayerWorksHelp);
        case _I<vehiculos>: Dialog_Show(playerid, Dialog:PlayerHelpVehicles);
        case _I<bandas>: Dialog_Show(playerid, Dialog:PlayerHelpCrews);
        case _I<vip>: Dialog_Show(playerid, Dialog:PlayerHelpMembership);
        default: Dialog_Show(playerid, Dialog:PlayerHelpMenu);
    }
    return 1;
}

/* INFORMATION */
hook OnScriptInit() {
    format(DIALOG_CREDIT_String, sizeof DIALOG_CREDIT_String,
        "\
        {"#PRIMARY_COLOR"}"SERVER_NAME"{FFFFFF}\n\n\
        Servidor RolePlay medio/bajo con misiones fuera de rol y dentro de rol.\n\
        Con este servidor, se espera que disfrutes de las misiones y el rol con buenos sistemas hechos con <3\n\
        La mayoría de misiones están fuera de contexto con tu rol principal con otros usuarios,\n\
        pero algunas misiones pueden o ser necesario hacerse en conjunto con otras personas y podrían formar parte del rol.\n\
        Aunque la idea es que hagas tu rol aparte, las misiones son un extra para mejorar un poco el entretenimiento.\n\
        \n\
        {"#PRIMARY_COLOR"}INFORMACIÓN Y CRÉDITOS{FFFFFF}\n\
        Desarrollado por: {"#SECONDARY_COLOR"}Dere.Prime{FFFFFF}\n\
        Ayuda mínima de: {"#SECONDARY_COLOR"}Joseph1416{FFFFFF}\n\
        Número de compilación (versión): {"#SECONDARY_COLOR"}#%d{FFFFFF}\n\
        \n\
        {"#PRIMARY_COLOR"}Encuéntranos en:{FFFFFF}\n\
        Discord: {"#SECONDARY_COLOR"}"SERVER_WEBSITE"{FFFFFF}\n\
        \n\
        {"#PRIMARY_COLOR"}Librerías y plugins:{FFFFFF}\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} ColAndreas, Crashdetect, FCNPC, MySQL, PawnCMD, PawnPlus\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} PawnRaknet, PawnRegex, SSCANF, streamer, YSF\n\
        \n\
        {"#SECONDARY_COLOR"}•{FFFFFF} YSI, pawn-easing-functions, mdialog, logger, TextureStudio, samp-textdraw-editor\n\
        \n\
        {"#PRIMARY_COLOR"}Mapeos: {FFFFFF}\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} La mayoría fueron sacados de internet (foros, Discord, etc.), y algunos mapeos simples fueron propios.\n\
        \n\
        {cccccc}¡Es todo!\n\
        ", g_last_build_version
	);
}

/* DIALOGS GENERAL INFO */
DialogCreate:PlayerHelpMenu(playerid) {
    Dialog_Open(playerid, Dialog:PlayerHelpMenu, DIALOG_STYLE_TABLIST, ""SERVER_NAME_FORMAT" · Ayuda",
    "{"#SECONDARY_COLOR"}•{FFFFFF} Informacion\n\
    {"#SECONDARY_COLOR"}•{FFFFFF} Reportar un Problema\n\
    {"#SECONDARY_COLOR"}•{FFFFFF} General\n\
    {"#SECONDARY_COLOR"}•{FFFFFF} Propiedades\n\
    {"#SECONDARY_COLOR"}•{FFFFFF} Trabajos\n\
    {"#SECONDARY_COLOR"}•{FFFFFF} Vehículos\n\
    {"#SECONDARY_COLOR"}•{FFFFFF} Bandas\n\
    {"#SECONDARY_COLOR"}•{FFFFFF} Membresia VIP\n"
    , "Continuar", "Cerrar");
    return 1;
}
DialogResponse:PlayerHelpMenu(playerid, response, listitem, inputtext[]) {
    if (response) {
        switch(listitem) {
            case 0: Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Informacion", DIALOG_CREDIT_String, "Volver", "");
            case 1: Dialog_Show(playerid, Dialog:PlayerHelpReport);
            case 2: Dialog_Show(playerid, Dialog:PlayerHelpGeneral);
            case 3: Dialog_Show(playerid, Dialog:PlayerHelpProperties);
            case 4: Dialog_Show(playerid, Dialog:PlayerWorksHelp);
            case 5: Dialog_Show(playerid, Dialog:PlayerHelpVehicles);
            case 6: Dialog_Show(playerid, Dialog:PlayerHelpCrews);
            case 7: Dialog_Show(playerid, Dialog:PlayerHelpMembership);
        }
    }
    return 1;
}
DialogResponse:ReturnToMainHelpMenu(playerid, response, listitem, inputtext[]) { return Dialog_Show(playerid, Dialog:PlayerHelpMenu); }
/* REPORT */
DialogCreate:PlayerHelpReport(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Reportar", 
        "{FFFFFF}Para reportar a un jugador, utiliza el comando {"#PRIMARY_COLOR"}/reportar <ID>{FFFFFF}.\n\
        Tu reporte será enviado a la administración y será visible para el rango {"#ADMIN_COLOR"}Moderador{FFFFFF} en adelante.\n\
        \n\
        La ID del jugador es el número que aparece junto a su nombre, ilustrado de la siguiente manera:\n\
        Nombre_Apellido (ID)\n\
        \n\
        Si necesitas reportar algún bug del servidor o cualquier conflicto que no esté relacionado con un jugador,\n\
        hazlo directamente desde nuestro servidor de {"#PRIMARY_COLOR"}Discord{FFFFFF}.",
        "Volver", ""
    );
    return 1;
}
/* GENERAL */
DialogCreate:PlayerHelpGeneral(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}General", 
        "{FFFFFF}Un día del juego dura tres horas reales.\n\
        \n\
        Cada hora recibirás entre {"#PRIMARY_COLOR"}100 y 500 de experiencia{FFFFFF}.\n\
        La experiencia se obtiene trabajando y realizando distintas actividades dentro del servidor.\n\
        \n\
        Cada vez que tu experiencia cambie, se mostrará una barra dinámica con tu progreso actual.\n\
        Cuando la barra se complete, subirás de nivel automáticamente.\n\
        \n\
        El pago de juego (payday) se recibe cada {"#PRIMARY_COLOR"}3 horas{FFFFFF}.\n\
        \n\
        A excepción de los comandos de seguridad, todas las funciones del jugador\n\
        se gestionan desde el menú de interacción {"#PRIMARY_COLOR"}(tecla Y){FFFFFF}.\n\
        Desde allí puedes acceder a:\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} GPS\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Teléfono\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Guía telefónica\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Consumir medicamento, marihuana o crack\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Dar dinero u objetos\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Vender o tirar objetos\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Estadísticas\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Experiencia\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Inventario\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Configuración\n\
        \n\
        Para interactuar con opciones como (dar, vender, tirar) debes ser {"#PRIMARY_COLOR"}Nivel 15{FFFFFF}\n\
        \n\
        Utiliza {"#YELLOW_COLOR"}/panel{FFFFFF}, {"#YELLOW_COLOR"}/nombre{FFFFFF}, {"#YELLOW_COLOR"}/correo{FFFFFF} o {"#YELLOW_COLOR"}/pass{FFFFFF}\n\
        para gestionar la seguridad de tu cuenta y configuracion.\n\
        \n\
        Para ver el tiempo restante en la cárcel, utiliza {"#YELLOW_COLOR"}/tiempo{FFFFFF}.\n\
        \n\
        Para enviar una duda, usa el comando {"#YELLOW_COLOR"}/duda{FFFFFF} seguido de tu mensaje.\n",
        "Volver", ""
    );
    return 1;
}
/* PROPIEDADES */
DialogCreate:PlayerHelpProperties(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Propiedades",
        "{FFFFFF}Puedes encontrar diferentes propiedades en todo {"#SECONDARY_COLOR"}San Andreas{FFFFFF}.\n\
        Para comprar una, debes conocer la ID de la propiedad y acudir a cualquier banco;\n\
        el dinero se descontará automáticamente de tu cuenta bancaria.\n\
        \n\
        Puedes localizar tus propiedades utilizando el GPS.\n\
        Cualquier persona puede tocar el timbre de tu propiedad y tú decides si deseas permitirle la entrada.\n\
        \n\
        El comando {d1d1d1}/casa{FFFFFF} puede utilizarse desde cualquier lugar.\n\
        Desde allí podrás:\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Cambiar el nombre de la propiedad.\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Echar a un jugador que se encuentre dentro.\n\
        {"#SECONDARY_COLOR"}•{FFFFFF} Expulsar a todos los huéspedes.\n\
        \n\
        Si deseas vender tu propiedad, puedes hacerlo en {"#PRIMARY_COLOR"}\"Gobierno San Andreas\"{FFFFFF};\n\
        localízalo desde el GPS.\n\
        Puedes venderla al banco de inmediato, pero solo recibirás el {"#SECONDARY_COLOR"}25% del valor original{FFFFFF}\n\
        ({"#SECONDARY_COLOR"}50% si eres VIP{FFFFFF}).\n\
        \n\
        También puedes venderla a otro jugador dentro de un rango de precio preestablecido.\n",
        "Cerrar", ""
    );
    return 1;
}
/* VEHICULOS */
DialogCreate:PlayerHelpVehicles(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Vehiculos",
        "{"#PRIMARY_COLOR"}GENERAL\n\
        {FFFFFF}La gestión del vehículo se realiza desde el {"#SECONDARY_COLOR"}menú de interacción (Y){FFFFFF}.\n\
        \n\
        Si estás como {"#GREEN_COLOR"}conductor{FFFFFF}, tendrás acceso al menú vehicular con:\n\
        - Motor\n\
        - Luces\n\
        - Puertas\n\
        - Combustible\n\
        - Estado del motor\n\
        - {"#RED_COLOR"}Echar{FFFFFF}\n\
        - Opciones de trabajo (si aplica)\n\
        - Otras opciones\n\
        \n\
        También puedes usar directamente:\n\
        {"#BLUE_COLOR"}/motor{FFFFFF} • {"#BLUE_COLOR"}/luces{FFFFFF} • {"#BLUE_COLOR"}/seguro{FFFFFF}\n\
        \n\
        El combustible se reposta en {"#RED_COLOR"}gasolineras{FFFFFF}.\n\
        Si usas un bidón, debes {"#WARNING_COLOR"}estar fuera del vehículo{FFFFFF}\n\
        y hacerlo desde el {"#SECONDARY_COLOR"}Inventario{FFFFFF}.\n\
        \n\
        {"#PRIMARY_COLOR"}VEHÍCULOS PERSONALES\n\
        {FFFFFF}Se compran en {"#YELLOW_COLOR"}concesionarios{FFFFFF} con dinero de tu {"#GREEN_COLOR"}cuenta bancaria{FFFFFF}.\n\
        Puedes localizarlos con el GPS.\n\
        \n\
        El maletero se gestiona desde el {"#SECONDARY_COLOR"}Inventario{FFFFFF}.\n\
        Si el vehículo es destruido, reaparecerá en su último estacionamiento.\n\
        Las modificaciones se realizan en el {"#YELLOW_COLOR"}mecánico{FFFFFF}.\n\
        \n\
        Para venderlo, acude a {"#PRIMARY_COLOR"}\"Gobierno San Andreas\"{FFFFFF}.\n\
        Recibirás {"#WARNING_COLOR"}25%{FFFFFF} del valor ({"#WARNING_COLOR"}50% VIP{FFFFFF}).\n\
        \n\
        {"#ERROR_COLOR"}Importante:{FFFFFF} Se vende con todo lo que haya en el maletero.\n\
        ",
        "Cerrar", ""
    );
    return 1;
}
/* BANDAS */
DialogCreate:PlayerHelpCrews(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Bandas",
        "\
        {FFFFFF}Utiliza {"#BLUE_COLOR"}/banda{FFFFFF} para crear una banda o administrar la tuya.\n\
        Al entrar a una banda podrás conquistar territorios por todo {"#PRIMARY_COLOR"}San Andreas{FFFFFF}.\n\
        \n\
        Visita {"#SECONDARY_COLOR"}"SERVER_WEBSITE"{FFFFFF} para más información.\n\
        \n\
        ",
        "Cerrar", ""
    );
    return 1;
}
/* MEMBERSHIP */
DialogCreate:PlayerHelpMembership(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Membresia",
        "{"#MEMBERSHIP_COLOR"}MEMBRESÍA VIP (30 DÍAS)\n\
        {FFFFFF}Puedes comprar VIP por 30 días con el comando {"#BLUE_COLOR"}/vip{FFFFFF}.\n\
        Se adquiere con {"#YELLOW_COLOR"}"SERVER_COIN"{FFFFFF}; para saber cómo obtener esta moneda,\n\
        visita el servidor de discord oficial.\n\
        \n\
        {"#PRIMARY_COLOR"}VENTAJAS\n\
        {FFFFFF}- Hasta {"#GREEN_COLOR"}"#MAX_SU_WORKS"{FFFFFF} trabajos.\n\
        - Hasta {"#GREEN_COLOR"}"#MAX_SU_VEHICLES"{FFFFFF} vehículos.\n\
        - Hasta {"#GREEN_COLOR"}"#MAX_SU_TOYS"{FFFFFF} accesorios.\n\
        - Hasta {"#GREEN_COLOR"}"#MAX_SU_PROPERTIES"{FFFFFF} propiedades.\n\
        - No pagas {"#WARNING_COLOR"}seguro{FFFFFF} de propiedades ni vehículos en el Payday.\n\
        - No pierdes {"#BLUE_COLOR"}chaleco{FFFFFF} al morir.\n\
        - Al vender una propiedad o vehículo al banco recibes el {"#GREEN_COLOR"}50%{FFFFFF}.\n\
        - Recuperación {"#GREEN_COLOR"}doble de rápida{FFFFFF} en el hospital.\n\
        - Al recuperarte, tu vida vuelve al {"#GREEN_COLOR"}100%{FFFFFF}.\n\
        - Reducción de condena al {"#GREEN_COLOR"}50%{FFFFFF}.\n\
        - Cuando un médico te cure, recuperarás {"#GREEN_COLOR"}50% de vida{FFFFFF}.\n\
        \n\
        Visita {"#PRIMARY_COLOR"}"SERVER_WEBSITE"{FFFFFF} para más información.\n\
        \n\
        ",
        "Cerrar", ""
    );
    return 1;
}
