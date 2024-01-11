module;

#include "imgui.h"

export module View;


class UIState {
public:
    bool is_display_demo_window = false;
    bool is_looping = true;
};

static UIState ui_state;


export bool create_view() {
    ImGui::Begin("Test");
    ImGui::Checkbox("是否显示DemoUI", &ui_state.is_display_demo_window);

    if (ImGui::Button("退出")) {
        ui_state.is_looping = false;
    }

    ImGui::End();

    if (ui_state.is_display_demo_window) {
        ImGui::ShowDemoWindow();
    }

    return ui_state.is_looping;
}
