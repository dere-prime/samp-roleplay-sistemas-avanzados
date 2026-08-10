# Prime Roleplay

Prime Roleplay es un gamemode de SA-MP / GTA:SA orientado a rol en español, con foco en economía, trabajos, vehículos, misiones y un sistema de interacción más elaborado que el típico modo básico.

Este proyecto comenzó como una idea de servidor roleplay bastante ambiciosa, y con el tiempo fue creciendo en forma de un gamemode bastante completo para un entorno privado. Sin embargo, también es importante ser honesto: el proyecto no está terminado, le falta mucho por pulir, organizar y mejorar.

## Estado del proyecto

Este repositorio representa una base funcional y muy extensa de un gamemode de roleplay, pero sigue siendo un proyecto en desarrollo. Hay partes que están bastante bien estructuradas y otras que todavía necesitan refactorización, limpieza y ajustes.

Algunas áreas que todavía requieren trabajo:
- organización y limpieza del código
- mejora de rendimiento y estabilidad
- balanceo de sistemas económicos y de progresión
- refinamiento de algunas mecánicas antiguas
- integración y mejora de misiones y eventos
- revisión de partes heredadas de etapas anteriores del desarrollo

## Qué incluye actualmente

### Sistema de jugador
- registro y login de cuenta
- persistencia de datos del personaje
- inventario y objetos del jugador
- salud, heridas y estados del personaje
- sistema de reputación y tiempo de juego

### Interacción y rol
- menús de interacción con vehículos, objetos y entorno
- sistema de animaciones y acciones del jugador
- lógica de uso de objetos y eventos del mundo
- herramientas de interacción para actividades del servidor

### Vehículos
- gestión de vehículos estáticos y dinámicos
- sistema de combustible, motor, puertas y estado del vehículo
- paneles y speedo para distintos tipos de transporte
- soporte para vehículos especiales, embarcaciones y trabajos

### Trabajos y economía
- sistema de trabajos
- integración con fábricas, misiones y actividades del mundo
- lógica económica de compra, uso y progresión
- componentes de negocio y entorno social

### Misiones y cinematics
- implementación de misiones del gamemode
- sistema de cinematicas y pasos de misión
- estructura preparada para agregar nuevas misiones y eventos

### Administración y seguridad
- sistema de permisos y administración
- módulos de debug y modo desarrollador
- detección de comportamiento sospechoso y anti-cheat
- registros y herramientas de soporte del servidor

## Estructura del proyecto

- gamemodes/: gamemode principal
- gamemodes/core/: módulos del sistema (jugador, vehículos, trabajos, mundo, misiones, administración, etc.)
- filterscripts/: scripts auxiliares
- plugins/: plugins y dependencias externas
- pawncc/: includes, compilador y utilidades de Pawn
- scriptfiles/: datos y archivos generados por el servidor
- Misc/: recursos, SQL y utilidades externas

## Requisitos

- servidor SA-MP / GTA:SA compatible con Pawn
- compilador Pawn válido para este proyecto
- base de datos MySQL configurada
- plugins e includes presentes en las carpetas correspondientes

## Desarrollo

Para compilar el gamemode puedes usar los scripts de compilación incluidos en la carpeta pawncc o el build task del workspace.

## Nota importante

Este proyecto pasó por distintas etapas de aprendizaje. Hubo momentos en los que aún no dominaba bien ciertas técnicas de programación, por eso algunas partes del código pueden verse más simples, repetitivas o poco optimizadas de lo que sería ideal. A lo largo del tiempo se fue mejorando, pero el proyecto sigue teniendo muchas mejoras pendientes.

Es decir, esta base es valiosa, pero no representa una versión final ni pulida del gamemode. Sigue siendo una obra en evolución.

## Contacto

Autor: dere.prime
Discord: dere.prime
