#pragma once

#include "pch.h"


class D3dDemo {
private:
    ComPtr <ID3D11Device> d3d_device_;
    ComPtr <ID3D11DeviceContext> d3d_device_context_;
    ComPtr <IDXGISwapChain> d3d_swap_chain_;
    ComPtr <ID3D11RenderTargetView> d3d_render_target_view_;

public:
    bool Init(HWND hWnd, float width, float height);

    void Render();

    void Release();
};