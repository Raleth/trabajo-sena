@echo off
echo Building Budget Manager...

REM Create directories
if not exist "build" mkdir build
if not exist "bin" mkdir bin

REM Set MSYS2 MinGW64 path
set PATH=C:\msys64\mingw64\bin;%PATH%

REM Compile
echo Compiling main.c...
gcc -Iinclude -Wall -Wextra -O2 -c src/main.c -o build/main.o
gcc -Iinclude -Wall -Wextra -O2 -c idiomas/idiomas.c -o build/idiomas.o
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

REM Link
echo Linking...
gcc build/main.o build/idiomas.o -o bin/BudgetManager.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lshell32
if %ERRORLEVEL% NEQ 0 (
    echo Linking failed!
    pause
    exit /b 1
)

REM Copy required DLLs
echo Copying DLLs...
copy /Y C:\msys64\mingw64\bin\libraylib.dll bin\ >nul 2>&1
copy /Y C:\msys64\mingw64\bin\libgcc_s_seh-1.dll bin\ >nul 2>&1
copy /Y C:\msys64\mingw64\bin\libwinpthread-1.dll bin\ >nul 2>&1

echo Build successful!
echo.
echo Running application...
start bin\BudgetManager.exe
