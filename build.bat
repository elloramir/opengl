@echo off

if not exist bin mkdir bin

set CFLAGS=-Wall -Iengine
set LFLAGS=-lglfw3 -luser32 -lgdi32 -lkernel32 -lglew32 -lopengl32

g++ engine/*.cpp game/*.cpp %CFLAGS% %LFLAGS% -o bin/game

if %ERRORLEVEL% equ 0 "bin/game"