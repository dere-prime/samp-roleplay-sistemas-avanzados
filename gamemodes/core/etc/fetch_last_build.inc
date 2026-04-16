/* 

    Fetch Last Build
    by: Dere.Prime
    date: 2024-06-15

*/
#include <YSI_Coding\y_hooks>

new g_last_build_version = 0;

#if DEVELOPER_MODE == 1
    static const SERVER_MODE = 1;
#else
    static const SERVER_MODE = 0;
#endif

stock FetchLastBuildVersion() {
    new File:BUILD = fopen("SERVER/build.txt", io_read), build_str[128];
    fread(BUILD, build_str);
    sscanf(build_str, "d", g_last_build_version);
    fclose(BUILD);
    return 1;
}

/*
    Estados de el servidor:
    - NORMAL: El servidor se ha iniciado correctamente y está listo para recibir jugadores.
    - CRITICAL_ERROR: El servidor ha encontrado un error crítico durante el inicio, lo que impide su funcionamiento normal. En este estado, el servidor no aceptará conexiones de jugadores y mostrará un mensaje de error detallado en la consola.
    
    Al llamarse estos estados se puede hacer un llamado remoto para avisar al desarollador desde otra plataforma estos errores.
*/
forward SendServerLoadedRequest();
stock SendServerLoadedRequest() {
    printf("#########################################################################");
    printf("##                                                                     ##");
    printf("##  PPPPPPPPPPP  RRRRRRRRRR   IIIIIIIIIII MMMM      MMMMMM  EEEEEEEEE  ##");
    printf("##  PPPPPPPPPPPP RRRRRRRRRRR     IIIII    MMMMM    MMMMMMM  EEEEEEEEE  ##");
    printf("##  PPPP     PPP RRRP     RRR    IIIII    MMMMMM  MMMMMMMM  EEEE       ##");
    printf("##  PPPP     PPP RRRP     RRR    IIIII    MMMMMMMMMMMMMMMM  EEEE       ##");
    printf("##  PPPPPPPPPPPP RRRRRRRRRRR     IIIII    MMMM MMMMMM MMMM  EEEEEEEE   ##");
    printf("##  PPPPPPPPPPT  RRRRRRRRRR      IIIII    MMMM  MMMM  MMMM  EEEEEEEE   ##");
    printf("##  PPPP         RRRP   RRR      IIIII    MMMM        MMMM  EEEE       ##");
    printf("##  PPPP         RRRP    RRR     IIIII    MMMM        MMMM  EEEE       ##");
    printf("##  PPPP         RRRP     RRR IIIIIIIIIII MMMM        MMMM  EEEEEEEE   ##");
    printf("##                                                                     ##");
    printf("#########################################################################");
    printf(":: ROLEPLAY :: BUILD %d :: MODE %s ::", g_last_build_version, SERVER_MODE ? "DEVELOPER" : "NORMAL");
    return 1;
}
forward CriticalError();
stock CriticalError() {
    printf("####################################################################################################");
    printf("##                                                                                                ##");
    printf("##  EEEEEEEEEEEE  RRRRRRRRRRRRRRR    RRRRRRRRRRRRR        OOOOOOOOOOOOO     RRRRRRRRRRRRR         ##");
    printf("##  EEEEEEEEEEEE  RRRRRRRRRRRRRRRR   RRRRRRRRRRRRRR      OOOOOOOOOOOOOOOO   RRRRRRRRRRRRRR        ##");
    printf("##  EEEEEEEEEEEE  RRRRRRRRRRRRRRRR   RRRRRRRRRRRRRR     OOOOOOOOOOOOOOOOOO  RRRRRRRRRRRRRR        ##");
    printf("##  EEEEEE        RRRRRR      RRRR   RRRR      RRRRR    OOOOO        OOOOO  RRRR      RRRRR       ##");
    printf("##  EEEEEE        RRRRRR      RRRR   RRRR      RRRRR    OOOOO        OOOOO  RRRR      RRRRR       ##");
    printf("##  EEEEEEEEEEEE  RRRRRRRRRRRRRRRR   RRRRRRRRRRRRRRR    OOOOO        OOOOO  RRRRRRRRRRRRRRR       ##");
    printf("##  EEEEEEEEEEEE  RRRRRRRRRRRRRRR    RRRRRRRRRRRRRR     OOOOO        OOOOO  RRRRRRRRRRRRRR        ##");
    printf("##  EEEEEEEEEEEE  RRRRRRRRRRRR       RRRRRRRRRRRRR      OOOOO        OOOOO  RRRRRRRRRRRRR         ##");
    printf("##  EEEEEE        RRRRRR   RRRR      RRRRR      RRRR    OOOOO        OOOOO  RRRRR      RRRR       ##");
    printf("##  EEEEEE        RRRRRR    RRRR     RRRRR       RRRR   OOOOOOOOOOOOOOOOOO  RRRRR       RRRR      ##");
    printf("##  EEEEEEEEEEEE  RRRRRR     RRRR    RRRRR        RRRR   OOOOOOOOOOOOOOOO   RRRRR        RRRR     ##");
    printf("##  EEEEEEEEEEEE  RRRRRR      RRRR   RRRRR         RRRR   OOOOOOOOOOOOOO    RRRRR         RRRR    ##");
    printf("##                                                                                                ##");
    printf("####################################################################################################");
    return 1;
}

/* Onbtener la build de el servidor al encender */
hook OnScriptInit() {
    FetchLastBuildVersion();
}