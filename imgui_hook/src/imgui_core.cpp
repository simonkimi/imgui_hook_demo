#include <imgui_impl/imgui_impl_win32.h>
#include "imgui_core.h"

bool ImguiCore::is_display_ = true;
bool ImguiCore::is_initialized_ = false;
ImVec2 ImguiCore::window_size_ = {0, 0};
ImVec2 ImguiCore::window_pos_ = {0, 0};
bool ImguiCore::need_set_pos_ = false;


void ImguiCore::DrawFrame()
{
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (is_display_) {
        ImGui::Begin("Hello, world!", &is_display_);

        ImGui::SetWindowSize({500, 300}, ImGuiCond_Once);
        if (window_pos_.x != 0.0f && window_pos_.y != 0.0f &&
            window_size_.x != 0.0f && window_size_.y != 0.0f && need_set_pos_) {
            ImGui::SetWindowPos(window_pos_, ImGuiCond_Once);
            ImGui::SetWindowSize(window_size_, ImGuiCond_Once);
            need_set_pos_ = false;
        }

        if (!need_set_pos_) {
            window_pos_ = ImGui::GetWindowPos();
            window_size_ = ImGui::GetWindowSize();
        }

        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();
}

void ImguiCore::OnWindowResize()
{
    need_set_pos_ = true;
}
