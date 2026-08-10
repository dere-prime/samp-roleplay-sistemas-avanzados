# Prime Roleplay

Prime Roleplay es un gamemode de SA-MP/Grand Theft Auto: San Andreas orientado a rol en español, con un enfoque en progresión de personaje, economía, trabajo, misiones y sistema de interacción social.

## Características principales

### Sistema de personajes
- Registro y login de jugadores.
- Persistencia de datos de cuenta y estado del personaje.
- Sistema de inventario y objetos equipables.
- Estado de salud, heridas, barras de vida y efectos de daño.
- Sistema de reputación y tiempo de juego.

### Interacción y rol
- Menús de interacción con NPCs, vehículos y objetos.
- Sistema de cuerpos/objetos del personaje.
- Integración con animaciones y acciones del entorno.
- Menú de interacción para vehículos y trabajos.

### Vehículos
- Gestión de vehículos estáticos y dinámicos.
- Sistema de combustible, motor, puertas y estado general.
- Indicadores de velocidad y paneles para distintos tipos de vehículos.
- Reparación de embarcaciones y mecanismos de apoyo para vehículos especiales.

### Trabajos y economía
- Sistema de trabajos y vehículos de trabajo.
- Integración con fábricas, misiones y actividades del mundo.
- Economia basada en objetos, compras, inventario y progresión.
- Soporte para sistemas de negocio y actividades del entorno.

### Misiones y cinematics
- Implementación de misiones del modo de juego.
- Sistema de cinematicas y pasos de misión.
- Modo especial para misiones con control de flujo y eventos.
- Estructura modular para agregar nuevas misiones.

### Seguridad y administración
- Integración de anti-cheat y detección de comportamiento sospechoso.
- Sistema de permisos y administración.
- Mecanismos de debug y modo desarrollador.
- Registro de eventos y logs del servidor.

## Estructura del proyecto

- gamemodes/: código principal del gamemode.
- gamemodes/core/: módulos del sistema (jugador, vehículos, trabajos, mundo, misiones, etc.).
- filterscripts/: scripts auxiliares.
- plugins/: plugins y configuraciones externas.
- pawncc/: includes, compilador y utilidades Pawn.
- scriptfiles/: archivos generados por el servidor y datos temporales.
- Misc/: recursos, SQL y utilidades externas.

## Requisitos

- Servidor SA-MP/GTA:SA con soporte para Pawn.
- Compilador Pawn compatible con el proyecto.
- MySQL configurado para la base de datos.
- Plugins y includes incluidos en la carpeta pawncc y plugins.

## Desarrollo

Para compilar el gamemode, puedes usar los scripts de compilación incluidos en la carpeta pawncc o el task de build del workspace.

## Nota

Este repositorio contiene una base funcional de un proyecto de Roleplay bastante completo. Algunas partes están enfocadas a un servidor privado y pueden requerir ajustes para adaptarlas a otro entorno.

## Contacto

Autor: dere.prime
Discord: dere.prime
