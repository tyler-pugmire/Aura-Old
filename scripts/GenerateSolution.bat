@echo off
pushd %~dp0\..\
call tools\premake\premake5.exe vs2022
popd
PAUSE