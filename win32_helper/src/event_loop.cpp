#include "event_loop.h"

WindowsEventLoop::WindowsEventLoop(HWND hwnd) : hwnd_(hwnd) {}

WindowsEventLoop::WindowsEventLoop() : hwnd_(nullptr) {}

WindowsEventLoop::~WindowsEventLoop()
{
    Stop();
}

void WindowsEventLoop::RegisterHotKey(int id, UINT modifiers, UINT vk, EventCallback callback)
{
    if (hotkeys_.find(id) != hotkeys_.end())
        throw std::runtime_error("Hotkey already registered");
    if (!::RegisterHotKey(hwnd_, id, modifiers, vk))
        throw std::runtime_error("RegisterHotKey failed");
    hotkeys_.emplace(id, std::move(callback));
}


void WindowsEventLoop::Run()
{
    MSG msg;
    while (running_) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!PeekMessage(&msg, hwnd_, 0, 0, PM_REMOVE))
            continue;
        if (msg.message == WM_QUIT) {
            running_ = false;
            continue;
        }

        switch (msg.message) {
            case WM_HOTKEY: {
                auto it = hotkeys_.find(msg.wParam);
                if (it != hotkeys_.end()) {
                    it->second();
                }
                break;
            }
            default:
                break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::for_each(hotkeys_.begin(), hotkeys_.end(), [&](auto &pair) {
        if (!::UnregisterHotKey(hwnd_, (int) pair.first)) {
            std::wcout << std::format(L"UnregisterHotKey failed, error code: {}", ::GetLastError()) << std::endl;
        }
    });
    hotkeys_.clear();
}

void WindowsEventLoop::Stop()
{
    running_ = false;
}


