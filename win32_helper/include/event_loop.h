#pragma once

#include "pch.h"
#include <functional>
#include <queue>
#include <mutex>

using EventCallback = std::function<void()>;

void HotkeyEventLoop(
        HWND hWnd,
        std::atomic<bool> &cancel_token,
        std::vector<std::tuple<UINT, UINT, EventCallback>>& hotkeys
);