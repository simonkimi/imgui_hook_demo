//
// Created by ms on 2024/1/30.
//

#include "d3d_demo.h"

bool D3dDemo::Init(HWND hwnd, float width, float height)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 640;
    sd.BufferDesc.Height = 480;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    const D3D_FEATURE_LEVEL featureLevelArray[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0,};
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr,
                                             D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevelArray,
                                             2, D3D11_SDK_VERSION,
                                             &sd, &swap_chain_, &device_,
                                             nullptr, &device_context_))) {
        return false;
    }

    ID3D11Texture2D *pBackBuffer;
    if (FAILED(swap_chain_->GetBuffer(
            0, __uuidof(ID3D11Texture2D),
            (LPVOID *) &pBackBuffer))) {
        return false;
    }

    HRESULT hr = device_->CreateRenderTargetView(pBackBuffer, nullptr, &render_target_view_);
    pBackBuffer->Release();

    if (FAILED(hr)) {
        return false;
    }

    device_context_->OMSetRenderTargets(1, &render_target_view_, nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT) width;
    vp.Height = (FLOAT) height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    device_context_->RSSetViewports(1, &vp);
    return true;
}

void D3dDemo::Render()
{
    float ClearColor[4] = {1.0f, 0.125f, 0.6f, 1.0f}; // RGBA
    device_context_->ClearRenderTargetView(render_target_view_, ClearColor);

    swap_chain_->Present(0, 0);
}

void D3dDemo::Release()
{
    render_target_view_->Release();
    swap_chain_->Release();
    device_context_->Release();
    device_->Release();
}
