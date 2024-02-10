#pragma once

#include "pch.h"
#include "imgui_core.h"
#include "d3d11.h"

#ifndef HR
#define HR(x) { HRESULT hr = (x); if (FAILED(hr)) { DxTrace(hr); } }
#endif

using FuncPresent = HRESULT(APIENTRY *)(IDXGISwapChain *p_this, UINT sync_interval, UINT flag);
using FuncResizeBuffers = HRESULT(APIENTRY *)(IDXGISwapChain *p_this, UINT BufferCount, UINT width, UINT height,
                                              DXGI_FORMAT new_format, UINT swap_chain_flags);
using FuncSetFullscreenState = HRESULT(APIENTRY *)(IDXGISwapChain *p_this, BOOL full_screen, IDXGIOutput *p_target);

void GetDx11VTable(HWND hwnd, void **v_table, int size);

class ImguiD311Impl : public ImguiCore {
private:
    static IDXGISwapChain *d3d_swap_chain_;
    static ID3D11Device *d3d_device_;
    static ID3D11DeviceContext *d3d_device_context_;

    static FuncPresent vfun_present_;
    static FuncResizeBuffers vfun_resize_buffers_;
    static FuncSetFullscreenState vfun_set_fullscreen_state_;
public:
    static void StartHook();

    static void EndHook();

private:
    static void InitImgui();

    static HRESULT HookPresent(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags);

    static HRESULT APIENTRY HookResizeBuffers(IDXGISwapChain *p_this, UINT buffer_count, UINT width, UINT height,
                                              DXGI_FORMAT new_format, UINT swap_chain_flags);

    static HRESULT APIENTRY HookSetFullscreenState(IDXGISwapChain *p_this, BOOL full_screen, IDXGIOutput *p_target);
};