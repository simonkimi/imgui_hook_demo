#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx9.h>
#include <detours/detours.h>
#include "hook_dx9_impl.h"
#include "utils/dx_utils.h"

IDirect3DDevice9 *ImguiD39Impl::d3d_device_ = nullptr;
FuncEndScene ImguiD39Impl::vfun_end_scene_ = nullptr;
FuncReset ImguiD39Impl::vfun_reset_ = nullptr;


BOOL GetDx9VTable(HWND hwnd, void **v_table, int size)
{
    Microsoft::WRL::ComPtr<IDirect3DDevice9> device;
    Microsoft::WRL::ComPtr<IDirect3D9> d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.Windowed = (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_POPUP) == 0;

    HRESULT hresult = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, device.GetAddressOf());
    if (FAILED(hresult)) {
        DxTrace(hresult);
        d3dpp.Windowed = !d3dpp.Windowed;
        hresult = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, device.GetAddressOf());
    }

    if (FAILED(hresult)) {
        DxTrace(hresult, true);
        return FALSE;
    }

    memcpy(v_table, *(void ***) device.Get(), size);
    return TRUE;
}

void ImguiD39Impl::StartHook()
{
    SPDLOG_INFO("ImguiD39Impl::StartHook()");
    if (is_initialized_) {
        return;
    }
    void *v_table[119];
    if (!GetDx9VTable(hwnd_, v_table, sizeof(v_table))) {
        SPDLOG_ERROR("GetDx9VTable failed");
        return;
    }
    vfun_end_scene_ = (FuncEndScene) v_table[42];
    vfun_reset_ = (FuncReset) v_table[16];
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) vfun_end_scene_, HookEndScene);
    DetourAttach(&(PVOID &) vfun_reset_, HookReset);
    DetourTransactionCommit();
}

void ImguiD39Impl::InitImgui()
{
    SPDLOG_INFO("ImguiD39Impl::InitImgui()");
    D3DDEVICE_CREATION_PARAMETERS d3d_creation_parameters;
    d3d_device_->GetCreationParameters(&d3d_creation_parameters);
    HookWndProc();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.IniFilename = nullptr;
    io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    ImGui_ImplWin32_Init(hwnd_);
    ImGui_ImplDX9_Init(d3d_device_);

    is_initialized_ = true;
}

void ImguiD39Impl::EndHook()
{
    SPDLOG_INFO("ImguiD39Impl::EndHook()");
    if (is_initialized_) {
        UnhookWndProc();
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        is_initialized_ = false;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) vfun_end_scene_, HookEndScene);
    DetourDetach(&(PVOID &) vfun_reset_, HookReset);
    DetourTransactionCommit();
}

HRESULT ImguiD39Impl::HookEndScene(LPDIRECT3DDEVICE9 pDevice)
{
    if (d3d_device_ == nullptr)
        d3d_device_ = pDevice;
    if (!is_initialized_)
        InitImgui();

    ImGui_ImplDX9_NewFrame();
    DrawFrame();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    return vfun_end_scene_(pDevice);
}

HRESULT ImguiD39Impl::HookReset(D3DPRESENT_PARAMETERS *pPresentationParameters)
{
    UnhookWndProc();
    if (is_initialized_) {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        d3d_device_ = nullptr;
        is_initialized_ = false;
    }
    return vfun_reset_(pPresentationParameters);
}
