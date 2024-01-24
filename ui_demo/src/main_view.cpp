#include "main_view.h"
#include "string_helper.h"


void MainView::DisplaySubWindow() {
    if (is_display_demo_window_) ImGui::ShowDemoWindow(&is_display_demo_window_);
}

bool MainView::Render() {
    if (!ImGui::Begin("测试程序", &open_, flags)) {
        ImGui::End();
        return open_;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("工具")) {
            ImGui::MenuItem("显示 DemoUi", nullptr, &is_display_demo_window_);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::Button("打开Dll")) {
        auto new_path = win32::OpenFileDialog(hwnd_, L"DLL文件\0*.dll\0All Files\0*.*\0");
        if (new_path.has_value()) {
            dll_path_ = win32::TCharToCChar(new_path.value());
        }
    }

    ImGui::Text("Dll路径: %s", dll_path_ != nullptr ? dll_path_.get() : "请选择一个路径");

    DisplaySubWindow();
    ImGui::End();
    return open_;
}

MainView::MainView(HWND hwnd) : hwnd_(hwnd) {
}
