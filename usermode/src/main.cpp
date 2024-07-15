#include <windows.h>
#include <dwmapi.h>
#include <d3d9.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "gui/GUIController.h"
#include "core/cheat/ThreadController.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

bool menuShow = true;
bool safeExit = false;
bool driverLoaded = false;
HWND _HWND = NULL;
uint32_t ScreenWidth = 0;
uint32_t ScreenHeight = 0;
uint32_t ScreenLeft = 0;
uint32_t ScreenRight = 0;
uint32_t ScreenTop = 0;
uint32_t ScreenBottom = 0;
uint32_t argc;

wchar_t** argv;
HANDLE iqvw64e_device_handle;

DWORD getParentProcess()
{
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    DWORD ppid = 0, pid = GetCurrentProcessId();

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    __try {
        if (hSnapshot == INVALID_HANDLE_VALUE) __leave;

        ZeroMemory(&pe32, sizeof(pe32));
        pe32.dwSize = sizeof(pe32);
        if (!Process32First(hSnapshot, &pe32)) __leave;

        do {
            if (pe32.th32ProcessID == pid) {
                ppid = pe32.th32ParentProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));

    }
    __finally {
        if (hSnapshot != INVALID_HANDLE_VALUE) CloseHandle(hSnapshot);
    }
    return ppid;
}

std::wstring OpenFileDialog()
{
    OPENFILENAME ofn;
    wchar_t szFile[260] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"System Files\0*.sys\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        return std::wstring(ofn.lpstrFile);
    }
    return L"";
}
namespace OverlayWindow
{
    WNDCLASSEX WindowClass;
    HWND Hwnd;
    LPCWSTR Name = L"Overlay";
}

namespace DirectX9Interface
{
    IDirect3D9Ex* Direct3D9 = NULL;
    IDirect3DDevice9Ex* pDevice = NULL;
    D3DPRESENT_PARAMETERS pParams = { 0 };
    MARGINS Margin = { -1 };
    MSG Message = { 0 };
}

void InputHandler() {
    for (int i = 0; i < 5; i++) ImGui::GetIO().MouseDown[i] = false;
    int button = -1;
    if (GetAsyncKeyState(VK_LBUTTON)) button = 0;
    if (button != -1) ImGui::GetIO().MouseDown[button] = true;
}

void Render(GUIController& controller)
{
    if (GetAsyncKeyState(VK_INSERT) & 1)
        menuShow = !menuShow;

    if (GetAsyncKeyState(VK_END) & 1)
        safeExit = true;

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::GetIO().MouseDrawCursor = menuShow;

    controller.Render();

    ImGui::EndFrame();

    DirectX9Interface::pDevice->SetRenderState(D3DRS_ZENABLE, false);
    DirectX9Interface::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    DirectX9Interface::pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

    DirectX9Interface::pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    if (DirectX9Interface::pDevice->BeginScene() >= 0) {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        DirectX9Interface::pDevice->EndScene();
    }

    HRESULT result = DirectX9Interface::pDevice->Present(NULL, NULL, NULL, NULL);
    if (result == D3DERR_DEVICELOST && DirectX9Interface::pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
        ImGui_ImplDX9_InvalidateDeviceObjects();
        DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
        ImGui_ImplDX9_CreateDeviceObjects();
    }
}

void MainLoop(GUIController& controller) {
    static RECT OldRect;
    ZeroMemory(&DirectX9Interface::Message, sizeof(MSG));

    while (DirectX9Interface::Message.message != WM_QUIT && !safeExit) {
        if (PeekMessage(&DirectX9Interface::Message, OverlayWindow::Hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&DirectX9Interface::Message);
            DispatchMessage(&DirectX9Interface::Message);
        }
        HWND ForegroundWindow = GetForegroundWindow();
        if (ForegroundWindow == _HWND) {
            HWND TempProcessHwnd = GetWindow(ForegroundWindow, GW_HWNDPREV);
            SetWindowPos(OverlayWindow::Hwnd, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }

        RECT TempRect;
        POINT TempPoint;
        ZeroMemory(&TempRect, sizeof(RECT));
        ZeroMemory(&TempPoint, sizeof(POINT));

        GetClientRect(_HWND, &TempRect);
        ClientToScreen(_HWND, &TempPoint);

        TempRect.left = TempPoint.x;
        TempRect.top = TempPoint.y;
        ImGuiIO& io = ImGui::GetIO();

        POINT TempPoint2;
        GetCursorPos(&TempPoint2);
        io.MousePos.x = TempPoint2.x - TempPoint.x;
        io.MousePos.y = TempPoint2.y - TempPoint.y;

        if (GetAsyncKeyState(0x1)) {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0].x = io.MousePos.x;
            io.MouseClickedPos[0].y = io.MousePos.y;
        }
        else {
            io.MouseDown[0] = false;
        }

        if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom) {
            OldRect = TempRect;
            ScreenWidth = TempRect.right;
            ScreenHeight = TempRect.bottom;
            DirectX9Interface::pParams.BackBufferWidth = ScreenWidth;
            DirectX9Interface::pParams.BackBufferHeight = ScreenHeight;
            SetWindowPos(OverlayWindow::Hwnd, (HWND)0, TempPoint.x, TempPoint.y, ScreenWidth, ScreenHeight, SWP_NOREDRAW);
            DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
        }
        Render(controller);
    }
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    if (DirectX9Interface::pDevice != NULL) {
        DirectX9Interface::pDevice->EndScene();
        DirectX9Interface::pDevice->Release();
    }
    if (DirectX9Interface::Direct3D9 != NULL) {
        DirectX9Interface::Direct3D9->Release();
    }
    DestroyWindow(OverlayWindow::Hwnd);
    UnregisterClass(OverlayWindow::WindowClass.lpszClassName, OverlayWindow::WindowClass.hInstance);
}

bool DirectXInit() {
    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::Direct3D9))) {
        return false;
    }

    D3DPRESENT_PARAMETERS Params = { 0 };
    Params.Windowed = TRUE;
    Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    Params.hDeviceWindow = OverlayWindow::Hwnd;
    Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    Params.BackBufferFormat = D3DFMT_A8R8G8B8;
    Params.BackBufferWidth = ScreenWidth;
    Params.BackBufferHeight = ScreenHeight;
    Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    Params.EnableAutoDepthStencil = TRUE;
    Params.AutoDepthStencilFormat = D3DFMT_D16;
    Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    if (FAILED(DirectX9Interface::Direct3D9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayWindow::Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &DirectX9Interface::pDevice))) {
        DirectX9Interface::Direct3D9->Release();
        return false;
    }

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplWin32_Init(OverlayWindow::Hwnd);
    ImGui_ImplDX9_Init(DirectX9Interface::pDevice);
    DirectX9Interface::Direct3D9->Release();
    return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
        return true;

    switch (Message) {
    case WM_DESTROY:
        if (DirectX9Interface::pDevice != NULL) {
            DirectX9Interface::pDevice->EndScene();
            DirectX9Interface::pDevice->Release();
        }
        if (DirectX9Interface::Direct3D9 != NULL) {
            DirectX9Interface::Direct3D9->Release();
        }
        PostQuitMessage(0);
        exit(4);
        break;
    case WM_SIZE:
        if (DirectX9Interface::pDevice != NULL && wParam != SIZE_MINIMIZED) {
            ImGui_ImplDX9_InvalidateDeviceObjects();
            DirectX9Interface::pParams.BackBufferWidth = LOWORD(lParam);
            DirectX9Interface::pParams.BackBufferHeight = HIWORD(lParam);
            HRESULT hr = DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
            if (hr == D3DERR_INVALIDCALL)
                IM_ASSERT(0);
            ImGui_ImplDX9_CreateDeviceObjects();
        }
        break;
    default:
        return DefWindowProc(hWnd, Message, wParam, lParam);
        break;
    }
    return 0;
}

void SetupWindow() {
    OverlayWindow::WindowClass = {
        sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, OverlayWindow::Name, LoadIcon(nullptr, IDI_APPLICATION)
    };

    RegisterClassEx(&OverlayWindow::WindowClass);
    if (_HWND) {
        static RECT TempRect = { 0 };
        static POINT TempPoint;
        GetClientRect(_HWND, &TempRect);
        ClientToScreen(_HWND, &TempPoint);
        TempRect.left = TempPoint.x;
        TempRect.top = TempPoint.y;
        ScreenWidth = TempRect.right;
        ScreenHeight = TempRect.bottom;
    }

    OverlayWindow::Hwnd = CreateWindowEx(NULL, OverlayWindow::Name, OverlayWindow::Name, WS_POPUP | WS_VISIBLE, ScreenLeft, ScreenTop, ScreenWidth, ScreenHeight, NULL, NULL, 0, NULL);
    DwmExtendFrameIntoClientArea(OverlayWindow::Hwnd, &DirectX9Interface::Margin);
    SetWindowLong(OverlayWindow::Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
    ShowWindow(OverlayWindow::Hwnd, SW_SHOW);
    UpdateWindow(OverlayWindow::Hwnd);
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    _HWND = FindWindow(NULL, L"Counter-Strike 2");

    if (_HWND == NULL)
    {
        MessageBox(NULL, L"Counter-Strike 2 is not started. Please start the game before running the cheat.", L"Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    bool WindowFocus = false;
    while (!WindowFocus)
    {
        HWND hwnd_active = GetForegroundWindow();
        if (_HWND == hwnd_active)
        {
            _HWND = GetForegroundWindow();

            RECT TempRect;
            GetWindowRect(_HWND, &TempRect);
            ScreenWidth = TempRect.right - TempRect.left;
            ScreenHeight = TempRect.bottom - TempRect.top;
            ScreenLeft = TempRect.left;
            ScreenRight = TempRect.right;
            ScreenTop = TempRect.top;
            ScreenBottom = TempRect.bottom;

            WindowFocus = true;
        }
    }

    SetupWindow();
    if (!DirectXInit())
    {
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    GUIController controller = GUIController(io);

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ThreadMgr thManager;
    thManager.Start();

    MainLoop(controller);

    return 0;
}
