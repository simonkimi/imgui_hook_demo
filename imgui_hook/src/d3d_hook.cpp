#include <imgui_impl/imgui_impl_dx11.h>
#include "d3d_hook.h"
#include "imgui_drawer.h"
#include "imgui_impl/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IDXGISwapChain *D3d11Hook::d3d_swap_chain_ = nullptr;
ID3D11Device *D3d11Hook::d3d_device_ = nullptr;
ID3D11DeviceContext *D3d11Hook::d3d_device_context_ = nullptr;
HMODULE D3d11Hook::h_dll_module_ = nullptr;
HWND D3d11Hook::h_wnd_ = nullptr;
WNDPROC D3d11Hook::old_wnd_proc_ = nullptr;
FuncPresent D3d11Hook::vfun_present_ = nullptr;
FuncResizeBuffers D3d11Hook::vfun_resize_buffers_ = nullptr;
FuncSetFullscreenState D3d11Hook::vfun_set_fullscreen_state_ = nullptr;
long D3d11Hook::window_width_ = 0;
long D3d11Hook::window_height_ = 0;


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
    DetourAttach(&(PVOID &) vfun_present_, ImguiDrawer::Present);
    DetourAttach(&(PVOID &) vfun_resize_buffers_, HookedResizeBuffers);
    DetourAttach(&(PVOID &) vfun_set_fullscreen_state_, HookedSetFullscreenState);
    DetourTransactionCommit();
}

void D3d11Hook::EndHook()
{
    if (ImguiDrawer::is_initialized_) {
        UnhookWndProc();
        ImguiDrawer::is_initialized_ = false;
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        RELEASE_COM(d3d_device_)
        RELEASE_COM(d3d_device_context_)
        d3d_swap_chain_ = nullptr;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) vfun_present_, ImguiDrawer::Present);
    DetourDetach(&(PVOID &) vfun_resize_buffers_, HookedResizeBuffers);
    DetourDetach(&(PVOID &) vfun_set_fullscreen_state_, HookedSetFullscreenState);
    DetourTransactionCommit();
}

void D3d11Hook::HookWndProc()
{
    old_wnd_proc_ = (WNDPROC) SetWindowLongPtr(h_wnd_, GWLP_WNDPROC, (LONG_PTR) WndProc);
}

void D3d11Hook::UnhookWndProc()
{
    SetWindowLongPtr(h_wnd_, GWLP_WNDPROC, (LONG_PTR) old_wnd_proc_);
}

void D3d11Hook::GetDeviceAndContext(IDXGISwapChain *swap_chain)
{
    swap_chain->GetDevice(__uuidof(ID3D11Device), (void **) &d3d_device_);
    d3d_device_->GetImmediateContext(&d3d_device_context_);
}

HRESULT D3d11Hook::HookedResizeBuffers(IDXGISwapChain *p_this, UINT buffer_count, UINT width, UINT height,
                                       DXGI_FORMAT new_format, UINT swap_chain_flags)
{
    ImguiDrawer::need_set_pos_ = true;
    UnhookWndProc();
    if (ImguiDrawer::is_initialized_) {
        ImguiDrawer::is_initialized_ = false;
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    RELEASE_COM(d3d_device_)
    RELEASE_COM(d3d_device_context_)
    d3d_swap_chain_ = nullptr;

    return vfun_resize_buffers_(p_this, buffer_count, width, height, new_format, swap_chain_flags);
}

HRESULT D3d11Hook::HookedSetFullscreenState(IDXGISwapChain *p_this, BOOL full_screen, IDXGIOutput *p_target)
{
    ImguiDrawer::need_set_pos_ = true;
    UnhookWndProc();
    if (ImguiDrawer::is_initialized_) {
        ImguiDrawer::is_initialized_ = false;
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    RELEASE_COM(d3d_device_)
    RELEASE_COM(d3d_device_context_)
    d3d_swap_chain_ = nullptr;
    return vfun_set_fullscreen_state_(p_this, full_screen, p_target);
}
