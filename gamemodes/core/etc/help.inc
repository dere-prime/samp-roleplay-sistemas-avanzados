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
        {"#PRIMARY_COLOR"}"SERVER_NAME"{f0f0f0}\n\n\
        Servidor RolePlay medio/bajo con misiones fuera de rol y dentro de rol.\n\
        Con este servidor, se espera que disfrutes de las misiones y el rol con buenos sistemas hechos con <3\n\
        La mayoría de misiones están fuera de contexto con tu rol principal con otros usuarios,\n\
        pero algunas misiones pueden o ser necesario hacerse en conjunto con otras personas y podrían formar parte del rol.\n\
        Aunque la idea es que hagas tu rol aparte, las misiones son un extra para mejorar un poco el entretenimiento.\n\
        \n\
        {"#PRIMARY_COLOR"}INFORMACIÓN Y CRÉDITOS{f0f0f0}\n\
        Desarrollado por: {"#SECONDARY_COLOR"}Dere.Prime{f0f0f0}\n\
        Ayuda mínima de: {"#SECONDARY_COLOR"}Joseph1416{f0f0f0}\n\
        Número de compilación (versión): {"#SECONDARY_COLOR"}#%d{f0f0f0}\n\
        \n\
        {"#PRIMARY_COLOR"}Encuéntranos en:{f0f0f0}\n\
        Discord: {"#SECONDARY_COLOR"}"SERVER_WEBSITE"{f0f0f0}\n\
        \n\
        {"#PRIMARY_COLOR"}Librerías y plugins:{f0f0f0}\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} ColAndreas, Crashdetect, FCNPC, MySQL, PawnCMD, PawnPlus\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} PawnRaknet, PawnRegex, SSCANF, streamer, YSF\n\
        \n\
        {"#SECONDARY_COLOR"}•{f0f0f0} YSI, pawn-easing-functions, mdialog, logger, TextureStudio, samp-textdraw-editor\n\
        \n\
        {"#PRIMARY_COLOR"}Mapeos: {f0f0f0}\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} La mayoría fueron sacados de internet (foros, Discord, etc.), y algunos mapeos simples fueron propios.\n\
        \n\
        {cccccc}¡Es todo!\n\
        ", g_last_build_version
	);
}

/* DIALOGS GENERAL INFO */
DialogCreate:PlayerHelpMenu(playerid) {
    Dialog_Open(playerid, Dialog:PlayerHelpMenu, DIALOG_STYLE_TABLIST, ""SERVER_NAME_FORMAT" · Ayuda",
    "{"#SECONDARY_COLOR"}•{f0f0f0} Informacion\n\
    {"#SECONDARY_COLOR"}•{f0f0f0} Reportar un Problema\n\
    {"#SECONDARY_COLOR"}•{f0f0f0} General\n\
    {"#SECONDARY_COLOR"}•{f0f0f0} Propiedades\n\
    {"#SECONDARY_COLOR"}•{f0f0f0} Trabajos\n\
    {"#SECONDARY_COLOR"}•{f0f0f0} Vehículos\n\
    {"#SECONDARY_COLOR"}•{f0f0f0} Bandas\n\
    {"#SECONDARY_COLOR"}•{f0f0f0} Membresia VIP\n"
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
        "{f0f0f0}Para reportar a un jugador, utiliza el comando {"#PRIMARY_COLOR"}/reportar <ID>{f0f0f0}.\n\
        Tu reporte será enviado a la administración y será visible para el rango {"#ADMIN_COLOR"}Moderador{f0f0f0} en adelante.\n\
        \n\
        La ID del jugador es el número que aparece junto a su nombre, ilustrado de la siguiente manera:\n\
        Nombre_Apellido (ID)\n\
        \n\
        Si necesitas reportar algún bug del servidor o cualquier conflicto que no esté relacionado con un jugador,\n\
        hazlo directamente desde nuestro servidor de {"#PRIMARY_COLOR"}Discord{f0f0f0}.",
        "Volver", ""
    );
    return 1;
}
/* GENERAL */
DialogCreate:PlayerHelpGeneral(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}General", 
        "{f0f0f0}Un día del juego dura tres horas reales.\n\
        \n\
        Cada hora recibirás entre {"#PRIMARY_COLOR"}100 y 500 de experiencia{f0f0f0}.\n\
        La experiencia se obtiene trabajando y realizando distintas actividades dentro del servidor.\n\
        \n\
        Cada vez que tu experiencia cambie, se mostrará una barra dinámica con tu progreso actual.\n\
        Cuando la barra se complete, subirás de nivel automáticamente.\n\
        \n\
        El pago de juego (payday) se recibe cada {"#PRIMARY_COLOR"}3 horas{f0f0f0}.\n\
        \n\
        A excepción de los comandos de seguridad, todas las funciones del jugador\n\
        se gestionan desde el menú de interacción {"#PRIMARY_COLOR"}(tecla Y){f0f0f0}.\n\
        Desde allí puedes acceder a:\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} GPS\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Teléfono\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Guía telefónica\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Consumir medicamento, marihuana o crack\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Dar dinero u objetos\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Vender o tirar objetos\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Estadísticas\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Experiencia\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Inventario\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Configuración\n\
        \n\
        Para interactuar con opciones como (dar, vender, tirar) debes ser {"#PRIMARY_COLOR"}Nivel 15{f0f0f0}\n\
        \n\
        Utiliza {"#YELLOW_COLOR"}/panel{f0f0f0}, {"#YELLOW_COLOR"}/nombre{f0f0f0}, {"#YELLOW_COLOR"}/correo{f0f0f0} o {"#YELLOW_COLOR"}/pass{f0f0f0}\n\
        para gestionar la seguridad de tu cuenta y configuracion.\n\
        \n\
        Para ver el tiempo restante en la cárcel, utiliza {"#YELLOW_COLOR"}/tiempo{f0f0f0}.\n\
        \n\
        Para enviar una duda, usa el comando {"#YELLOW_COLOR"}/duda{f0f0f0} seguido de tu mensaje.\n",
        "Volver", ""
    );
    return 1;
}
/* PROPIEDADES */
DialogCreate:PlayerHelpProperties(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Propiedades",
        "{f0f0f0}Puedes encontrar diferentes propiedades en todo {"#SECONDARY_COLOR"}San Andreas{f0f0f0}.\n\
        Para comprar una, debes conocer la ID de la propiedad y acudir a cualquier banco;\n\
        el dinero se descontará automáticamente de tu cuenta bancaria.\n\
        \n\
        Puedes localizar tus propiedades utilizando el GPS.\n\
        Cualquier persona puede tocar el timbre de tu propiedad y tú decides si deseas permitirle la entrada.\n\
        \n\
        El comando {d1d1d1}/casa{f0f0f0} puede utilizarse desde cualquier lugar.\n\
        Desde allí podrás:\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Cambiar el nombre de la propiedad.\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Echar a un jugador que se encuentre dentro.\n\
        {"#SECONDARY_COLOR"}•{f0f0f0} Expulsar a todos los huéspedes.\n\
        \n\
        Si deseas vender tu propiedad, puedes hacerlo en {"#PRIMARY_COLOR"}\"Gobierno San Andreas\"{f0f0f0};\n\
        localízalo desde el GPS.\n\
        Puedes venderla al banco de inmediato, pero solo recibirás el {"#SECONDARY_COLOR"}25% del valor original{f0f0f0}\n\
        ({"#SECONDARY_COLOR"}50% si eres VIP{f0f0f0}).\n\
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
        {f0f0f0}La gestión del vehículo se realiza desde el {"#SECONDARY_COLOR"}menú de interacción (Y){f0f0f0}.\n\
        \n\
        Si estás como {"#GREEN_COLOR"}conductor{f0f0f0}, tendrás acceso al menú vehicular con:\n\
        - Motor\n\
        - Luces\n\
        - Puertas\n\
        - Combustible\n\
        - Estado del motor\n\
        - {"#RED_COLOR"}Echar{f0f0f0}\n\
        - Opciones de trabajo (si aplica)\n\
        - Otras opciones\n\
        \n\
        También puedes usar directamente:\n\
        {"#BLUE_COLOR"}/motor{f0f0f0} • {"#BLUE_COLOR"}/luces{f0f0f0} • {"#BLUE_COLOR"}/seguro{f0f0f0}\n\
        \n\
        El combustible se reposta en {"#RED_COLOR"}gasolineras{f0f0f0}.\n\
        Si usas un bidón, debes {"#WARNING_COLOR"}estar fuera del vehículo{f0f0f0}\n\
        y hacerlo desde el {"#SECONDARY_COLOR"}Inventario{f0f0f0}.\n\
        \n\
        {"#PRIMARY_COLOR"}VEHÍCULOS PERSONALES\n\
        {f0f0f0}Se compran en {"#YELLOW_COLOR"}concesionarios{f0f0f0} con dinero de tu {"#GREEN_COLOR"}cuenta bancaria{f0f0f0}.\n\
        Puedes localizarlos con el GPS.\n\
        \n\
        El maletero se gestiona desde el {"#SECONDARY_COLOR"}Inventario{f0f0f0}.\n\
        Si el vehículo es destruido, reaparecerá en su último estacionamiento.\n\
        Las modificaciones se realizan en el {"#YELLOW_COLOR"}mecánico{f0f0f0}.\n\
        \n\
        Para venderlo, acude a {"#PRIMARY_COLOR"}\"Gobierno San Andreas\"{f0f0f0}.\n\
        Recibirás {"#WARNING_COLOR"}25%{f0f0f0} del valor ({"#WARNING_COLOR"}50% VIP{f0f0f0}).\n\
        \n\
        {"#ERROR_COLOR"}Importante:{f0f0f0} Se vende con todo lo que haya en el maletero.\n\
        ",
        "Cerrar", ""
    );
    return 1;
}
/* BANDAS */
DialogCreate:PlayerHelpCrews(playerid) {
    Dialog_Open(playerid, Dialog:ReturnToMainHelpMenu, DIALOG_STYLE_MSGBOX, "Ayuda · {"#PRIMARY_COLOR"}Bandas",
        "\
        {f0f0f0}Utiliza {"#BLUE_COLOR"}/banda{f0f0f0} para crear una banda o administrar la tuya.\n\
        Al entrar a una banda podrás conquistar territorios por todo {"#PRIMARY_COLOR"}San Andreas{f0f0f0}.\n\
        \n\
        Visita {"#SECONDARY_COLOR"}"SERVER_WEBSITE"{f0f0f0} para más información.\n\
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
        {f0f0f0}Puedes comprar VIP por 30 días con el comando {"#BLUE_COLOR"}/vip{f0f0f0}.\n\
        Se adquiere con {"#YELLOW_COLOR"}"SERVER_COIN"{f0f0f0}; para saber cómo obtener esta moneda,\n\
        visita el servidor de discord oficial.\n\
        \n\
        {"#PRIMARY_COLOR"}VENTAJAS\n\
        {f0f0f0}- Hasta {"#GREEN_COLOR"}"#MAX_SU_WORKS"{f0f0f0} trabajos.\n\
        - Hasta {"#GREEN_COLOR"}"#MAX_SU_VEHICLES"{f0f0f0} vehículos.\n\
        - Hasta {"#GREEN_COLOR"}"#MAX_SU_TOYS"{f0f0f0} accesorios.\n\
        - Hasta {"#GREEN_COLOR"}"#MAX_SU_PROPERTIES"{f0f0f0} propiedades.\n\
        - No pagas {"#WARNING_COLOR"}seguro{f0f0f0} de propiedades ni vehículos en el Payday.\n\
        - No pierdes {"#BLUE_COLOR"}chaleco{f0f0f0} al morir.\n\
        - Al vender una propiedad o vehículo al banco recibes el {"#GREEN_COLOR"}50%{f0f0f0}.\n\
        - Recuperación {"#GREEN_COLOR"}doble de rápida{f0f0f0} en el hospital.\n\
        - Al recuperarte, tu vida vuelve al {"#GREEN_COLOR"}100%{f0f0f0}.\n\
        - Reducción de condena al {"#GREEN_COLOR"}50%{f0f0f0}.\n\
        - Cuando un médico te cure, recuperarás {"#GREEN_COLOR"}50% de vida{f0f0f0}.\n\
        \n\
        Visita {"#PRIMARY_COLOR"}"SERVER_WEBSITE"{f0f0f0} para más información.\n\
        \n\
        ",
        "Cerrar", ""
    );
    return 1;
}
