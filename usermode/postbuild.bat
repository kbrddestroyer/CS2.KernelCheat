del /q ..\x64\build\kernelmode
del /q ..\x64\build\*.ini
del /q ..\x64\build\*.inf

echo -- CLEANUP COMPLETED --

cd ..\PythonExtentions\
build.bat
