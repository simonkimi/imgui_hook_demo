#include <pch.h>
#include "d3d_hook.h"


void OnProcessAttach();

void OnProcessDetach();

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD fdw_reason, LPVOID lpv_reserved)
{
    switch (fdw_reason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(h_instance);
            OnProcessAttach();
            break;
        case DLL_PROCESS_DETACH:
            OnProcessDetach();
            break;
        default:
            break;
    }
    return TRUE;
}

void OnProcessAttach()
{
    AllocConsole();
    freopen_s((FILE **) stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE **) stderr, "CONOUT$", "w", stderr);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleTitle(_T("Injected DLL"));
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) D3d11Hook::StartHook, nullptr, 0, nullptr);
}

void OnProcessDetach()
{
    D3d11Hook::EndHook();
    FreeConsole();
}