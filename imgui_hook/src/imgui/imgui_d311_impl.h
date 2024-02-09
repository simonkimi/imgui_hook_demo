#pragma once

#include "pch.h"
#include "imgui_core.h"
#include "d3d11.h"

using FuncPresent = HRESULT(APIENTRY *)(IDXGISwapChain *p_this, UINT sync_interval, UINT flag);
using FuncResizeBuffers = HRESULT(APIENTRY *)(IDXGISwapChain *p_this, UINT BufferCount, UINT width, UINT height,
                                              DXGI_FORMAT new_format, UINT swap_chain_flags);
using FuncSetFullscreenState = HRESULT(APIENTRY *)(IDXGISwapChain *p_this, BOOL full_screen, IDXGIOutput *p_target);


class ImguiD311Impl : public ImguiCore {
private:
    static IDXGISwapChain *d3d_swap_chain_;
    static ID3D11Device *d3d_device_;
    static ID3D11DeviceContext *d3d_device_context_;

    static HWND h_wnd_;
    static WNDPROC old_wnd_proc_;

    static FuncPresent vfun_present_;
    static FuncResizeBuffers vfun_resize_buffers_;
    static FuncSetFullscreenState vfun_set_fullscreen_state_;
public:
    static void StartHook();

    static void EndHook();

private:
    static void InitImgui();

    static HRESULT HookedPresent(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags);

    static HRESULT APIENTRY HookedResizeBuffers(IDXGISwapChain *p_this, UINT buffer_count, UINT width, UINT height,
                                                DXGI_FORMAT new_format, UINT swap_chain_flags);

    static HRESULT APIENTRY HookedSetFullscreenState(IDXGISwapChain *p_this, BOOL full_screen, IDXGIOutput *p_target);
    
    static void UnhookWndProc();

    static void HookWndProc();

    static BOOL WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};