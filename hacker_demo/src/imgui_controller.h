#pragma once

#include "imgui.h"
#include "../lib/imgui_impl/imgui_impl_win32.h"
#include "../lib/imgui_impl/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <stdexcept>
#include <functional>
#include "dwmapi.h"


class ImguiController {
public:
    ImguiController();

    ~ImguiController();

    bool loop(const std::function<bool()> &render);
    
private:
    WNDCLASSEXW *wc_;
    HWND__ *hwnd_;
    ImVec4 *clear_color_ = new ImVec4(0, 0, 0, 0);
};
