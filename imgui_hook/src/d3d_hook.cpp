#include "d3d_hook.h"
#include "imgui_drawer.h"
#include "imgui_impl/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT D3d11Hook::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImguiDrawer::is_display_ && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
        return true;
    }
    if (ImguiDrawer::is_initialized_) {
        ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    }

    if (msg == WM_CLOSE) {
        EndHook();
        TerminateProcess(GetCurrentProcess(), 0);
    }

    if (ImGui::GetIO().WantCaptureMouse) {
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
            return true;
        return false;
    }

    return CallWindowProc(old_wnd_proc_, hWnd, msg, wParam, lParam);
}

void D3d11Hook::StartHook()
{
    h_wnd_ = win32::GetProcessWindow();
    if (!h_wnd_) {
        std::cerr << "Failed to get process window" << std::endl;
        return;
    }

    const auto &[w, h] = win32::GetWindowSize(h_wnd_);
    window_width_ = w;
    window_height_ = h;

    void *d3d11_swap_chain[40];
    GetDx11VTable(h_wnd_, d3d11_swap_chain, sizeof(d3d11_swap_chain));
    vfun_present_ = (FuncPresent) d3d11_swap_chain[8];
    vfun_resize_buffers_ = (FuncResizeBuffers) d3d11_swap_chain[13];
    vfun_set_fullscreen_state_ = (FuncSetFullscreenState) d3d11_swap_chain[10];

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourTransactionCommit();
}

void D3d11Hook::EndHook()
{

}

void D3d11Hook::HookWndProc()
{
    old_wnd_proc_ = (WNDPROC) SetWindowLongPtr(h_wnd_, GWLP_WNDPROC, (LONG_PTR) WndProc);
}

void D3d11Hook::UnhookWndProc()
{
    SetWindowLongPtr(h_wnd_, GWLP_WNDPROC, (LONG_PTR) old_wnd_proc_);
}
