#include "event_loop.h"


void HotkeyEventLoop(
        HWND hWnd,
        std::atomic<bool> &cancel_token,
        std::vector<std::tuple<UINT, UINT, EventCallback>> hotkeys
)
{
    std::unordered_map<int, EventCallback> hotkey_map;
    int hotkey_id = 0;
    for (auto &hotkey: hotkeys) {
        auto [modifiers, vk, callback] = hotkey;
        if (!::RegisterHotKey(hWnd, hotkey_id, modifiers, vk)) {
            std::wcerr << std::format(L"RegisterHotKey failed, error code: {}", ::GetLastError()) << std::endl;
            cancel_token = true;
            break;
        }
        hotkey_map.emplace(hotkey_id, std::move(callback));
        hotkey_id += 1;
    }

    MSG msg;
    while (!cancel_token) {
        if (!PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
            continue;
        switch (msg.message) {
            case WM_QUIT:
                cancel_token = true;
                continue;
            case WM_HOTKEY: {
                auto it = hotkey_map.find(msg.wParam);
                if (it != hotkey_map.end()) {
                    it->second();
                }
                break;
            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::for_each(hotkey_map.begin(), hotkey_map.end(), [&](auto &pair) {
        if (!::UnregisterHotKey(hWnd, (int) pair.first)) {
            std::wcerr << std::format(L"UnregisterHotKey failed, error code: {}", ::GetLastError()) << std::endl;
        }
    });
}
