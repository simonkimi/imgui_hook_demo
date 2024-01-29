#include <pch.h>
#include "event_loop.h"

WindowsEventLoop event_loop;
std::thread event_loop_thread;

void LoopThread();

BOOL WINAPI DllMain(HINSTANCE h_instance, DWORD fdw_reason, LPVOID lpv_reserved)
{
    switch (fdw_reason) {
        case DLL_PROCESS_ATTACH:
            event_loop_thread = std::thread(LoopThread);
            MessageBox(nullptr, L"DLL_PROCESS_ATTACH", L"Dll注入", MB_OK);
            break;
        case DLL_PROCESS_DETACH:
            MessageBox(nullptr, L"DLL_PROCESS_DETACH", L"Dll注入", MB_OK);
            event_loop.Stop();
            event_loop_thread.join();
            break;
        case DLL_THREAD_ATTACH:
            MessageBox(nullptr, L"DLL_THREAD_ATTACH!", L"Dll注入", MB_OK);
            break;
        case DLL_THREAD_DETACH:
            MessageBox(nullptr, L"DLL_THREAD_DETACH!", L"Dll注入", MB_OK);
            break;
        default:
            break;
    }
    return TRUE;
}


void LoopThread()
{
    event_loop.RegisterHotKey(TEST_HOT_KEY, MOD_ALT | MOD_CONTROL, 'A', []() {
        MessageBox(nullptr, L"热键被触发!", L"特检", MB_OK);
    });
    event_loop.Run();
}
