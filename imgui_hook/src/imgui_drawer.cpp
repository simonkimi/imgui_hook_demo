#include "imgui_drawer.h"
#include "imgui_impl/imgui_impl_dx11.h"
#include "imgui_impl/imgui_impl_win32.h"
#include "d3d_hook.h"

bool ImguiDrawer::is_initialized_ = false;
bool ImguiDrawer::is_display_ = true;
bool ImguiDrawer::need_set_pos_ = false;
ImVec2 ImguiDrawer::window_size_ = {500, 300};
ImVec2 ImguiDrawer::window_pos_ = {0, 0};

void ImguiDrawer::InitImgui(IDXGISwapChain *swap_chain)
{
    SPDLOG_INFO("ImguiDrawer::InitImgui()");
    D3d11Hook::HookWndProc();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    ImGui_ImplWin32_Init(D3d11Hook::h_wnd_);

    D3d11Hook::GetDeviceAndContext(swap_chain);

    ImGui_ImplDX11_Init(D3d11Hook::d3d_device_, D3d11Hook::d3d_device_context_);
    is_initialized_ = true;
}

HRESULT ImguiDrawer::Present(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags)
{
    if (!D3d11Hook::d3d_swap_chain_)
        D3d11Hook::d3d_swap_chain_ = swap_chain;

    if (!is_initialized_)
        InitImgui(swap_chain);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (is_display_) {
        ImGui::Begin("Hello, world!", &is_display_);

        ImGui::SetWindowSize({500, 300}, ImGuiCond_Once);
        if (window_pos_.x != 0.0f && window_pos_.y != 0.0f && window_size_.x != 0.0f && window_size_.y != 0.0f &&
            need_set_pos_) {
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
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return D3d11Hook::vfun_present_(swap_chain, sync_interval, flags);
}
