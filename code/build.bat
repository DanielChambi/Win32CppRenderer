@echo off
mkdir ..\build
pushd ..\build
cl -FAsc -Zi ..\code\win32_renderer.cpp user32.lib gdi32.lib
popd