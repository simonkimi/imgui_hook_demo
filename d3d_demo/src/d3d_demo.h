#pragma once

#include "pch.h"


class D3dDemo {
private:
    ID3D11Device *device_;
    ID3D11DeviceContext *device_context_;
    IDXGISwapChain *swap_chain_;
    ID3D11RenderTargetView *render_target_view_;

public:
    bool Init(HWND hwnd, float width, float height);

    void Render();

    void Release();
};