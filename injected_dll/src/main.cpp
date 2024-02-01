#include "main.h"
#include "event_loop.h"
#include "hook_demo.h"

HANDLE hotkey_thread;
HANDLE exit_event;
std::atomic_bool cancel_token = false;

void OnProcessAttach();

void OnProcessDetach();

void EventLoopThread();


API BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD fdw_reason, API LPVOID lpv_reserved)
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
    exit_event = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    hotkey_thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) EventLoopThread, nullptr, 0, nullptr);
    StartHook();
    std::cout << "Dll初始化完毕" << std::endl;
}

void OnProcessDetach()
{
    cancel_token = true;
    WaitForSingleObject(exit_event, INFINITE);
    ::CloseHandle(hotkey_thread);
    ::CloseHandle(exit_event);
    EndHook();
    std::cout << "Dll卸载完毕" << std::endl;
    FreeConsole();
}


void EventLoopThread()
{
    std::vector<std::tuple<UINT, UINT, EventCallback>> hotkeys(2);
    hotkeys[0] = std::make_tuple(MOD_ALT | MOD_CONTROL, 'A', [] {
        MessageBox(nullptr, L"热键A被触发!", L"A", MB_OK);
    });
    hotkeys[1] = std::make_tuple(MOD_ALT | MOD_CONTROL, 'B', [] {
        MessageBox(nullptr, L"热键B被触发!", L"B", MB_OK);
    });

    HotkeyEventLoop(nullptr, cancel_token, hotkeys);
    SetEvent(exit_event);
}

