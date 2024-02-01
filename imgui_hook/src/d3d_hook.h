#pragma once

#include "pch.h"
#include "utils/d3d_utils.h"
#include "process_helper.h"

using FuncPresent = HRESULT(APIENTRY *)(IDXGISwapChain *pThis, UINT SyncInterval, UINT Flag);
using FuncResizeBuffers = HRESULT(APIENTRY *)(IDXGISwapChain *pThis, UINT BufferCount, UINT Width, UINT Height,
                                              DXGI_FORMAT NewFormat, UINT SwapChainFlags);
using FuncSetFullscreenState = HRESULT(APIENTRY *)(IDXGISwapChain *pThis, BOOL FullScreen, IDXGIOutput *pTarget);

class D3d11Hook {
public:
    static IDXGISwapChain *d3d_swap_chain_;
    static ID3D11Device *d3d_device_;
    static ID3D11DeviceContext *d3d_device_context_;
    static HMODULE h_dll_module_;
    static HWND h_wnd_;
    static WNDPROC old_wnd_proc_;

    static FuncPresent vfun_present_;
    static FuncResizeBuffers vfun_resize_buffers_;
    static FuncSetFullscreenState vfun_set_fullscreen_state_;

    static void StartHook();
    static void EndHook();
    
    static void HookWndProc();
    static void UnhookWndProc();


private:
    static long window_width_;
    static long window_height_;

    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};


