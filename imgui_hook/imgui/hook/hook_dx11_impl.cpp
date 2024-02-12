#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "process_helper.h"
#include "hook_dx11_impl.h"
#include "detours/detours.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IDXGISwapChain *ImguiD311Impl::d3d_swap_chain_ = nullptr;
ID3D11Device *ImguiD311Impl::d3d_device_ = nullptr;
ID3D11DeviceContext *ImguiD311Impl::d3d_device_context_ = nullptr;
FuncPresent ImguiD311Impl::vfun_present_ = nullptr;
FuncResizeBuffers ImguiD311Impl::vfun_resize_buffers_ = nullptr;
FuncSetFullscreenState ImguiD311Impl::vfun_set_fullscreen_state_ = nullptr;


void ImguiD311Impl::InitImgui()
{
    SPDLOG_INFO("ImguiD311Impl::InitImgui()");
    HookWndProc();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    ImGui_ImplWin32_Init(hwnd_);

    d3d_swap_chain_->GetDevice(__uuidof(ID3D11Device), (void **) &d3d_device_);
    d3d_device_->GetImmediateContext(&d3d_device_context_);

    ImGui_ImplDX11_Init(d3d_device_, d3d_device_context_);
    is_initialized_ = true;
}

HRESULT ImguiD311Impl::HookPresent(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags)
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
    hwnd_ = win32::GetProcessWindow();
    if (!hwnd_) {
        SPDLOG_ERROR("Failed to get process window");
        return;
    }

    void *d3d11_swap_chain[40];
    GetDx11VTable(hwnd_, d3d11_swap_chain, sizeof(d3d11_swap_chain));
    vfun_present_ = (FuncPresent) d3d11_swap_chain[8];
    vfun_resize_buffers_ = (FuncResizeBuffers) d3d11_swap_chain[13];
    vfun_set_fullscreen_state_ = (FuncSetFullscreenState) d3d11_swap_chain[10];

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) vfun_present_, HookPresent);
    DetourAttach(&(PVOID &) vfun_resize_buffers_, HookResizeBuffers);
    DetourAttach(&(PVOID &) vfun_set_fullscreen_state_, HookSetFullscreenState);
    DetourTransactionCommit();
}


HRESULT ImguiD311Impl::HookResizeBuffers(IDXGISwapChain *p_this, UINT buffer_count, UINT width, UINT height,
                                         DXGI_FORMAT new_format, UINT swap_chain_flags)
{
    SPDLOG_INFO("D3d11Hook::HookResizeBuffers()");
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


HRESULT ImguiD311Impl::HookSetFullscreenState(IDXGISwapChain *p_this, BOOL full_screen, IDXGIOutput *p_target)
{
    SPDLOG_INFO("D3d11Hook::HookSetFullscreenState()");
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
    DetourDetach(&(PVOID &) vfun_present_, HookPresent);
    DetourDetach(&(PVOID &) vfun_resize_buffers_, HookResizeBuffers);
    DetourDetach(&(PVOID &) vfun_set_fullscreen_state_, HookSetFullscreenState);
    DetourTransactionCommit();
}



void GetDx11VTable(HWND hwnd, void **v_table, int size)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = (GetWindowLongPtr(sd.OutputWindow, GWL_STYLE) & WS_POPUP) == 0;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    Microsoft::WRL::ComPtr<ID3D11Device> d3d_device;
    Microsoft::WRL::ComPtr<IDXGISwapChain> d3d_swap_chain;

    HRESULT hresult = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            d3d_swap_chain.GetAddressOf(),
            d3d_device.GetAddressOf(),
            nullptr,
            nullptr
    );
    if (hresult == DXGI_ERROR_UNSUPPORTED) {
        hresult = D3D11CreateDeviceAndSwapChain(
                nullptr,
                D3D_DRIVER_TYPE_WARP,
                nullptr,
                0,
                nullptr,
                0,
                D3D11_SDK_VERSION,
                &sd,
                d3d_swap_chain.GetAddressOf(),
                d3d_device.GetAddressOf(),
                nullptr,
                nullptr
        );
    }
    HR(hresult)
    memcpy(v_table, *(void ***) (d3d_swap_chain.Get()), size);
}
