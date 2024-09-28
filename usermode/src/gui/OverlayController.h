#pragma once
/*
*   Overlay classes are used for overlay window control,
* message dispatch etc.
* 
* Look on commit messages
* wip/usermode/gui-debug
*/

#include <Windows.h>
#include <dwmapi.h>
#include <d3d9.h>

#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

/**
* Dataclass for OverlayClass data
* refactor of those horrible global namespaces on main
* 
* @param WindowClass - Window class structure, dataholder
* @param Hwnd - Base window for overlay
* @param Name - Window class name
*/
struct OverlayWindow
{
    WNDCLASSEX WindowClass;
    HWND Hwnd;
    const LPCWSTR Name = L"Overlay";
};

/**
* Dataclass for DirectX data
* refactor of those horrible global namespaces on main
* 
* @param Direct3D9 - pointer on Direct3D
* @param IDirect3DDevice9Ex - pointer on Direct3D device
* @param pParams
* @param Margin
* @param Message - usage should be deprecated
*/
struct DirectX9Interface
{
    IDirect3D9Ex* Direct3D9 = NULL;
    IDirect3DDevice9Ex* pDevice = NULL;
    D3DPRESENT_PARAMETERS pParams = { 0 };
    MARGINS Margin = { -1 };
    MSG Message = { 0 };
};

/**
* Singletone service class to share some data between WinAPI functions
* @func 
*/
class OverlayDataholder
{
private:
    static OverlayDataholder* instance;
public:
    OverlayDataholder()
    {
        if (!instance)
            instance = this;
    }

    ~OverlayDataholder()
    {
        instance = nullptr;  // Backref cleanup
    }

    static OverlayDataholder* Instance() { return instance; }
};

class OverlayController
{

};

