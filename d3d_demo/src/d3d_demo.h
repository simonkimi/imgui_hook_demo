#pragma once

#include "pch.h"


class D3dDemo {
private:
    ID3D11Device *d3d_device_;
    ID3D11DeviceContext *d3d_device_context_;
    IDXGISwapChain *d3d_swap_chain_;
    ID3D11RenderTargetView *d3d_render_target_view_;

public:
    bool Init(HWND hWnd, float width, float height);

    void Render();

    void Release();
};