#include "imgui.h"

export module View;

export class MainView {
private:
    bool exit_ = false;
    bool is_display_demo_window_ = false;

private:
    void display_sub_window() {
        if (is_display_demo_window_) ImGui::ShowDemoWindow(&is_display_demo_window_);
    }

public:
    bool render() {
        if (!ImGui::Begin("Demo", nullptr, ImGuiWindowFlags_MenuBar)) {
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

        display_sub_window();
        ImGui::End();
        return !exit_;
    }


};


