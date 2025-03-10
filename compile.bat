@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

cls

SET RAYLIB_PATH=C:\raylib\raylib
SET COMPILER_PATH=C:\raylib\w64devkit\bin
SET PATH=%COMPILER_PATH%;%PATH%
SET CC=gcc
SET CFLAGS=-s -static -O2 -Wall -I%RAYLIB_PATH%\src -Iexternal -DPLATFORM_DESKTOP
SET LDFLAGS=-lraylib -lopengl32 -lgdi32 -lwinmm -lcomdlg32
SET NAME_PART=paint19

SET OBJ=
IF NOT EXIST obj mkdir obj

FOR %%f IN (src\*.c) DO (
    SET FILENAME=%%~nf
    %CC% -c %%f -o obj\!FILENAME!.o %CFLAGS%
    SET OBJ=!OBJ! obj\!FILENAME!.o
)
FOR %%f IN (src\game\*.c) DO (
    SET FILENAME=%%~nf
    %CC% -c %%f -o obj\!FILENAME!.o %CFLAGS%
    SET OBJ=!OBJ! obj\!FILENAME!.o
)

%CC% -o %NAME_PART%.exe main.c %OBJ% %CFLAGS% %LDFLAGS%

REM PATH'i temizle
SET PATH=%PATH:C:\raylib\w64devkit\bin;=%

IF EXIST %NAME_PART%.exe (
    echo Info: Compressing executable.
    upx %NAME_PART%.exe
    %NAME_PART%.exe
) ELSE (
    echo Error: Executable not found.
)
