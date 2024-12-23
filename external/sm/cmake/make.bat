%echo off

cd /d %~dp0

if exist _build rd /s /q _build

mkdir _build
chdir _build

rem cmake ../ -G "Visual Studio 17 2022" -A x64 -T host=x64 -DCMAKE_INSTALL_PREFIX:PATH=.\install

cmake ../ -G "Visual Studio 17 2022" -A x64 -T host=x64 -DCMAKE_INSTALL_PREFIX:PATH=.\install

EXIT /B