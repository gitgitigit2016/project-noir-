@echo off
gcc -O2 -std=c99 ^
    globals.c ^
    utils.c ^
    init.c ^
    spawn.c ^
    update.c ^
    draw.c ^
    main.c ^
    -lraylib -lopengl32 -lgdi32 -lwinmm -lm ^
    -o mystery_defense.exe

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo Build FAILED.
    pause
    exit /b 1
)
echo Build successful! Running...
mystery_defense.exe
