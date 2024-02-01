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

    static FuncPresent vfun_present_;
    static FuncResizeBuffers vfun_resize_buffers_;
    static FuncSetFullscreenState vfun_set_fullscreen_state_;

    static void StartHook()
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

    static HRESULT HookPresent(IDXGISwapChain *pThis, UINT sync_interval, UINT flag)
    {
        if (!d3d_swap_chain_)
            d3d_swap_chain_ = pThis;

        return vfun_present_(pThis, sync_interval, flag);
    }

private:
    static long window_width_;
    static long window_height_;
};


