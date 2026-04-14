@echo off
setlocal

:: 1. Move to the directory where the script is located
cd /d "%~dp0"

:: 2. Define paths relative to the project root
set "SOURCE_FILE=main.c"
set "OUTPUT_DIR=obj\Release"
set "OUTPUT_FILE=%OUTPUT_DIR%\main.o"

:: 3. Create the directory if it doesn't exist
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

:: 4. Run the compiler
echo Building %OUTPUT_FILE%...
gcc -c "%SOURCE_FILE%" -o "%OUTPUT_FILE%" -O2

if %errorlevel% neq 0 (
    echo [ERROR] Build failed. Check if %SOURCE_FILE% exists in %CD%
    exit /b %errorlevel%
)

echo [SUCCESS] File built: %OUTPUT_FILE%