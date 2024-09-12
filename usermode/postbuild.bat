del /q ..\x64\build\kernelmode
del /q ..\x64\build\*.ini
del /q ..\x64\build\*.inf

echo -- CLEANUP COMPLETED --


echo -- COPYING DEPENDENCIES --

del ..\x64\build\Lib\
del ..\x64\build\kernelapi\

copy ..\..\cpython\PCBuild\amd64\*.dll ..\x64\build\
copy ..\..\cpython\PCBuild\amd64\*.pyd ..\x64\build\
xcopy ..\..\cpython\Lib\ ..\x64\build\Lib\ /E /Y
xcopy ..\PythonExtentions\kernelapi\ ..\x64\build\kernelapi\ /E /Y
del ..\x64\build\kernelapi\kernelapi.py
