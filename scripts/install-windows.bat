@echo off
set PROJECT_ROOT=%~dp0..
set BUILD_DIR=%PROJECT_ROOT%\build

mkdir "%BUILD_DIR%" 2>nul
cd "%BUILD_DIR%" || exit /b

cmake ..
cmake --build . --config Release
cmake --install .