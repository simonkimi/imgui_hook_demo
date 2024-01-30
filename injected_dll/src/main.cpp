#include <pch.h>
#include "event_loop.h"

HANDLE hotkey_thread;
std::atomic_bool cancel_token = false;
std::atomic_bool is_exit = false;
HANDLE exit_event;

void LoopThread()
{
    is_exit = false;
    std::list<std::tuple<UINT, UINT, EventCallback>> hotkeys;
    hotkeys.emplace_back(MOD_ALT | MOD_CONTROL, 'A', []() {
        MessageBox(nullptr, L"热键A被触发!", L"A", MB_OK);
    });
    hotkeys.emplace_back(MOD_ALT | MOD_CONTROL, 'B', []() {
        MessageBox(nullptr, L"热键B被触发!", L"B", MB_OK);
    });
    HotkeyEventLoop(nullptr, cancel_token, std::move(hotkeys));
    SetEvent(exit_event);
}


BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD fdw_reason, LPVOID lpv_reserved)
{
    switch (fdw_reason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(h_instance);
            exit_event = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            hotkey_thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) LoopThread, nullptr, 0, nullptr);
            MessageBox(nullptr, L"DLL注入成功!", L"Dll注入", MB_OK);
            break;
        case DLL_PROCESS_DETACH:
            MessageBox(nullptr, L"DLL卸载成功", L"Dll注入", MB_OK);
            cancel_token = true;
            WaitForSingleObject(exit_event, INFINITE);
            ::CloseHandle(hotkey_thread);
            ::CloseHandle(exit_event);
            break;
        default:
            break;
    }
    return TRUE;
}



