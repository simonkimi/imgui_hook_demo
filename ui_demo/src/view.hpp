#pragma once


#include <tchar.h>
#include "imgui.h"
#include "windows.h"

class MainView {
private:
    bool exit_ = false;
    bool is_display_demo_window_ = false;
    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
    HWND__ *hwnd_;

    void DisplaySubWindow() {
        if (is_display_demo_window_) ImGui::ShowDemoWindow(&is_display_demo_window_);
    }


public:
    explicit MainView(HWND__ *hwnd) : hwnd_(hwnd) {

    }

    bool Render() {
        if (!ImGui::Begin("Demo", nullptr, flags)) {
            ImGui::End();
            return false;
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("程序")) {
                if (ImGui::MenuItem("退出")) {
                    exit_ = true;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("工具")) {
                ImGui::MenuItem("显示 DemoUi", nullptr, &is_display_demo_window_);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        if (ImGui::Button("弹出对话框")) {
            MessageBoxW(hwnd_, L"测试测试测试测试", L"测试", MB_OK);
        }
        DisplaySubWindow();
        ImGui::End();
        return !exit_;
    }
};