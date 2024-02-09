#pragma once

#include "pch.h"

class ImguiCore {
protected:
    static bool is_display_;
    static bool is_initialized_;
    static ImVec2 window_size_;
    static ImVec2 window_pos_;
    static bool need_set_pos_;

public:
    static void DrawFrame();

    static void OnWindowResize();
};
