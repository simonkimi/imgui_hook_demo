#pragma once

#include <tchar.h>
#include "imgui.h"
#include "Windows.h"
#include "../../win32_helper/include//dialog.h"

class MainView {
private:
    bool open_ = true;
    bool is_display_demo_window_ = false;
    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
    HWND__ *hwnd_;
    
    std::string dll_path_;
    
    void DisplaySubWindow();

public:
    explicit MainView(HWND__ *hwnd);

    bool Render();
};