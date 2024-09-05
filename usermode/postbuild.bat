del /q ..\x64\build\kernelmode
del /q ..\x64\build\*.ini
del /q ..\x64\build\*.inf

echo -- CLEANUP COMPLETED --


echo -- COPYING DEPENDENCIES --

copy ..\..\cpython\PCBuild\amd64\*.dll ..\x64\build\
xcopy ..\..\cpython\Lib\ ..\x64\build\ /E /Y