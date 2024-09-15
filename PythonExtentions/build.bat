echo -- COPYING PYTHON DEPENDENCIES --

echo Environment compiled, installing external dependencies

mkdir ..\x64\build\Lib

..\..\cpython\PCBuild\amd64\python.exe -m ensurepip
..\..\cpython\PCBuild\amd64\python.exe -m pip install -r requirements.txt

pause
del ..\x64\build\Lib\
del ..\x64\build\kernelapi\

echo COPYING DLLS...
copy ..\..\cpython\PCBuild\amd64\*.dll ..\x64\build\

echo COPYING PYD...
copy ..\..\cpython\PCBuild\amd64\*.pyd ..\x64\build\

echo Creating python environment
xcopy ..\..\cpython\Lib\ ..\x64\build\Lib\ /E /Y
xcopy kernelapi\ ..\x64\build\kernelapi\ /E /Y
del ..\x64\build\kernelapi\kernelapi.py
pause