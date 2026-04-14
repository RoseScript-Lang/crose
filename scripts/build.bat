@echo off
setlocal

cd /d "%~dp0"

set "SOURCE_FILE=main.c"
set "OUTPUT_DIR=obj\Release"
set "OUTPUT_FILE=%OUTPUT_DIR%\main.o"

if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)


echo Building %OUTPUT_FILE%...
gcc -c "%SOURCE_FILE%" -o "%OUTPUT_FILE%" -O2

if %errorlevel% neq 0 (
    echo [ERROR] Build failed. Check if %SOURCE_FILE% exists in %CD%
    exit /b %errorlevel%
)

echo [SUCCESS] File built: %OUTPUT_FILE%
