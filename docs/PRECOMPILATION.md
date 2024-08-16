# Project assembling and compilation 

## Brief information 

Currently the project is running on MSBuild, and I have no plans to change build system. Target OS is win64. 

## Compilation requirements 

Some libraries are already included in project, such as following:
- ImGui (dear ImGui version, link in README)

Some additional dependencies are required for project building:
To build kernelmode you'll need:
- WinSDK, Windows Driver Kit (WDK)
To build usermode you'll need:
- WinSDK, DirectX SDK as part of WinSDK
- Python3.11+ installed 