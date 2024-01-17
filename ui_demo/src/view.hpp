#pragma once

#include "imgui.h"

class MainView {
private:
    bool exit_ = false;
    bool is_display_demo_window_ = false;
    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;

private:
    void DisplaySubWindow() {
        if (is_display_demo_window_) ImGui::ShowDemoWindow(&is_display_demo_window_);
    }

public:
    bool Render(){
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

        DisplaySubWindow();
        ImGui::End();
        return !exit_;
    }
};