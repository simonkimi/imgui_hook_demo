#pragma once

#include <memory>
#include "imgui.h"
#include "Windows.h"
#include "view_base.h"
#include "win32_dialog.h"

class MainView : public IView {
private:
    bool open_ = true;
    bool is_display_demo_window_ = false;
    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
    std::unique_ptr<char[]> dll_path_;

    void DisplaySubWindow();

public:
    explicit MainView() = default;

    bool Render(HWND hwnd) override;
};
