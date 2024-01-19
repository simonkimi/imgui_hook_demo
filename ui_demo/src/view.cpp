#include "../include/view.h"


void MainView::DisplaySubWindow() {
    if (is_display_demo_window_) ImGui::ShowDemoWindow(&is_display_demo_window_);
}

bool MainView::Render() {
    if (!ImGui::Begin("测试程序", &open_, flags)) {
        ImGui::End();
        return open_;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("程序")) {
            if (ImGui::MenuItem("退出")) {
                open_ = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("工具")) {
            ImGui::MenuItem("显示 DemoUi", nullptr, &is_display_demo_window_);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::Button("打开Dll")) {
        auto new_path = OpenFileDialog(hwnd_);
        if (new_path != nullptr) {
            dll_path_ = std::move(new_path);
        }
    }

    ImGui::Text("Dll路径: %s", dll_path_ != nullptr ? dll_path_->c_str() : "请选择一个路径");


    DisplaySubWindow();
    ImGui::End();
    return open_;
}

MainView::MainView(HWND__ *hwnd) : hwnd_(hwnd) {
}
