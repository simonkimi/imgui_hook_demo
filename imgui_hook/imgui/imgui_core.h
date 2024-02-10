#pragma once

#include "pch.h"

class ImguiCore {
protected:
    static bool is_display_;
    static bool is_initialized_;
    static ImVec2 window_size_;
    static ImVec2 window_pos_;
    static bool need_set_pos_;
    static WNDPROC original_wnd_proc_;
    static HWND hwnd_;
    static HMODULE hmodule_;

protected:
    static void DrawFrame();

    static void OnWindowResize();
    
    static void UnhookWndProc();

    static void HookWndProc();

    static BOOL WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
