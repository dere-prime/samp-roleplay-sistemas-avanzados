/* 

    Fetch Last Build
    by: Dere.Prime
    date: 2024-06-15

*/
#include <YSI_Coding\y_hooks>

new g_last_build_version = 0;

stock FetchLastBuildVersion() {
    new File:BUILD = fopen("SERVER/build.txt", io_read), build_str[128];
    fread(BUILD, build_str);
    sscanf(build_str, "d", g_last_build_version);
    fclose(BUILD);
    return 1;
}

/* Onbtener la build de el servidor al encender */
hook OnScriptInit() {
    FetchLastBuildVersion();
}