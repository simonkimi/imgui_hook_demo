#include "backends/imgui_impl_win32.h"
#include "imgui_core.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool ImguiCore::is_display_ = true;
bool ImguiCore::is_initialized_ = false;
ImVec2 ImguiCore::window_size_ = {0, 0};
ImVec2 ImguiCore::window_pos_ = {0, 0};
bool ImguiCore::need_set_pos_ = false;
WNDPROC ImguiCore::original_wnd_proc_ = nullptr;
HWND ImguiCore::hwnd_ = nullptr;
HMODULE ImguiCore::hmodule_ = nullptr;


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

void ImguiCore::UnhookWndProc()
{
    if (original_wnd_proc_ != nullptr) {
        SetWindowLongPtr(hwnd_, GWLP_WNDPROC, (LONG_PTR) original_wnd_proc_);
        original_wnd_proc_ = nullptr;
    }
}

void ImguiCore::HookWndProc()
{
    if (original_wnd_proc_ == nullptr) {
        original_wnd_proc_ = (WNDPROC) SetWindowLongPtr(hwnd_, GWLP_WNDPROC, (LONG_PTR) WndProc);
    }
}

BOOL ImguiCore::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (is_display_ && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
        return true;
    }
    if (is_initialized_) {
        ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    }

    if (msg == WM_CLOSE) {
        TerminateProcess(GetCurrentProcess(), 0);
    }

    if (ImGui::GetIO().WantCaptureMouse) {
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
            return true;
        return false;
    }

    return CallWindowProc(original_wnd_proc_, hWnd, msg, wParam, lParam);
}


