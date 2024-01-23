#pragma once

#include <memory>
#include "imgui.h"
#include "Windows.h"
#include "view_base.h"
#include "../../win32_helper/include/dialog.h"

class MainView : public IView {
private:
    bool open_ = true;
    bool is_display_demo_window_ = false;
    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
    HWND hwnd_;
    std::unique_ptr<char[]> dll_path_;

    void DisplaySubWindow();

public:
    explicit MainView(HWND hwnd);

    bool Render() override;
};
