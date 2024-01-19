#include "../include/view.h"
#include "../../win32_helper/include/string_helper.h"


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
        TCHAR new_path[MAX_PATH] = {0};
        auto has_new = OpenFileDialog(hwnd_, new_path, sizeof(new_path));
        if (has_new) {
            std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
            TCharToCChar(new_path, MAX_PATH, buffer.get());
            dll_path_ = std::move(buffer);
        }
    }

    ImGui::Text("Dll路径: %s", dll_path_ != nullptr ? dll_path_.get() : "请选择一个路径");

    DisplaySubWindow();
    ImGui::End();
    return open_;
}

MainView::MainView(HWND hwnd) : hwnd_(hwnd) {
}
