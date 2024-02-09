#include <imgui_impl/imgui_impl_win32.h>
#include <imgui_impl/imgui_impl_dx11.h>
#include <process_helper.h>
#include <utils/d3d_utils.h>
#include "imgui_d311_impl.h"
#include "detours/detours.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IDXGISwapChain *ImguiD311Impl::d3d_swap_chain_ = nullptr;
ID3D11Device *ImguiD311Impl::d3d_device_ = nullptr;
ID3D11DeviceContext *ImguiD311Impl::d3d_device_context_ = nullptr;
HWND ImguiD311Impl::h_wnd_ = nullptr;
WNDPROC ImguiD311Impl::old_wnd_proc_ = nullptr;
FuncPresent ImguiD311Impl::vfun_present_ = nullptr;
FuncResizeBuffers ImguiD311Impl::vfun_resize_buffers_ = nullptr;
FuncSetFullscreenState ImguiD311Impl::vfun_set_fullscreen_state_ = nullptr;


void ImguiD311Impl::HookWndProc()
{
    old_wnd_proc_ = (WNDPROC) SetWindowLongPtr(h_wnd_, GWLP_WNDPROC, (LONG_PTR) WndProc);
}


void ImguiD311Impl::InitImgui()
{
    SPDLOG_INFO("ImguiDrawer::InitImgui()");
    HookWndProc();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    ImGui_ImplWin32_Init(h_wnd_);

    d3d_swap_chain_->GetDevice(__uuidof(ID3D11Device), (void **) &d3d_device_);
    d3d_device_->GetImmediateContext(&d3d_device_context_);

    ImGui_ImplDX11_Init(d3d_device_, d3d_device_context_);
    is_initialized_ = true;
}

HRESULT ImguiD311Impl::HookedPresent(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags)
{
    if (d3d_swap_chain_ == nullptr)
        d3d_swap_chain_ = swap_chain;
    if (!is_initialized_)
        InitImgui();

    ImGui_ImplDX11_NewFrame();

    DrawFrame();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return vfun_present_(swap_chain, sync_interval, flags);
}

void ImguiD311Impl::StartHook()
{
    SPDLOG_INFO("ImguiD311Impl::StartHook()");
    h_wnd_ = win32::GetProcessWindow();
    if (!h_wnd_) {
        SPDLOG_ERROR("Failed to get process window");
        return;
    }

    void *d3d11_swap_chain[40];
    GetDx11VTable(h_wnd_, d3d11_swap_chain, sizeof(d3d11_swap_chain));
    vfun_present_ = (FuncPresent) d3d11_swap_chain[8];
    vfun_resize_buffers_ = (FuncResizeBuffers) d3d11_swap_chain[13];
    vfun_set_fullscreen_state_ = (FuncSetFullscreenState) d3d11_swap_chain[10];

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) vfun_present_, HookedPresent);
    DetourAttach(&(PVOID &) vfun_resize_buffers_, HookedResizeBuffers);
    DetourAttach(&(PVOID &) vfun_set_fullscreen_state_, HookedSetFullscreenState);
    DetourTransactionCommit();
}

BOOL ImguiD311Impl::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (is_display_ && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
        return true;
    }
    if (is_initialized_) {
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

HRESULT ImguiD311Impl::HookedResizeBuffers(IDXGISwapChain *p_this, UINT buffer_count, UINT width, UINT height,
                                           DXGI_FORMAT new_format, UINT swap_chain_flags)
{
    SPDLOG_INFO("D3d11Hook::HookedResizeBuffers()");
    OnWindowResize();
    UnhookWndProc();
    if (is_initialized_) {
        is_initialized_ = false;
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    RELEASE_COM(d3d_device_)
    RELEASE_COM(d3d_device_context_)
    d3d_swap_chain_ = nullptr;

    return vfun_resize_buffers_(p_this, buffer_count, width, height, new_format, swap_chain_flags);
}

void ImguiD311Impl::UnhookWndProc()
{
    SetWindowLongPtr(h_wnd_, GWLP_WNDPROC, (LONG_PTR) old_wnd_proc_);
}

HRESULT ImguiD311Impl::HookedSetFullscreenState(IDXGISwapChain *p_this, BOOL full_screen, IDXGIOutput *p_target)
{
    SPDLOG_INFO("D3d11Hook::HookedSetFullscreenState()");
    OnWindowResize();
    UnhookWndProc();
    if (is_initialized_) {
        is_initialized_ = false;
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    RELEASE_COM(d3d_device_)
    RELEASE_COM(d3d_device_context_)
    d3d_swap_chain_ = nullptr;
    return vfun_set_fullscreen_state_(p_this, full_screen, p_target);
}

void ImguiD311Impl::EndHook()
{
    SPDLOG_INFO("D3d11Hook::EndHook()");
    if (is_initialized_) {
        UnhookWndProc();
        is_initialized_ = false;
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        RELEASE_COM(d3d_device_)
        RELEASE_COM(d3d_device_context_)
        d3d_swap_chain_ = nullptr;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) vfun_present_, HookedPresent);
    DetourDetach(&(PVOID &) vfun_resize_buffers_, HookedResizeBuffers);
    DetourDetach(&(PVOID &) vfun_set_fullscreen_state_, HookedSetFullscreenState);
    DetourTransactionCommit();
}


