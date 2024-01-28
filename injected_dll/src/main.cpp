#include "main_view.h"
#include "imgui_controller.h"
#include <iostream>
#include <thread>

#define HOTKEY_DISPLAY_WINDOWS 1

void UILopper();

std::atomic<bool> is_run(true);
std::atomic<bool> is_display_windows(true);
std::thread ui_looper;
std::thread message_looper;


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
        std::wcout << L"Dll被挂载" << std::endl;
        if (!RegisterHotKey(nullptr, HOTKEY_DISPLAY_WINDOWS, MOD_CONTROL | MOD_ALT, VK_F10)) {
            std::wcout << std::format(L"注册热键失败, 错误码:{}", GetLastError()) << std::endl;
            return FALSE;
        }
        ui_looper = std::thread(UILopper);
        return TRUE;
    }

    if (fdwReason == DLL_PROCESS_DETACH) {
        std::wcout << L"Dll被卸载" << std::endl;
        UnregisterHotKey(nullptr, HOTKEY_DISPLAY_WINDOWS);
        is_run = false;
        ui_looper.join();
    }

    return TRUE;
}


void UILopper()
{
    MainView view;
    ImguiController *controller;

    while (is_run) {
        if (is_display_windows) {
            if (controller == nullptr) {
                controller = new ImguiController();
            }
            if (!controller->Loop([&view, &controller] { return view.Render(controller->GetHwnd()); })) {
                is_display_windows = false;
            }
        } else {
            if (controller != nullptr) {
                delete controller;
                controller = nullptr;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    delete controller;
}