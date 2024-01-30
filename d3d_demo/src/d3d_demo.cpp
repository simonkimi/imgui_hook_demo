#include "d3d_demo.h"

bool D3dDemo::Init(HWND hWnd, float width, float height)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = (UINT) width;
    sd.BufferDesc.Height = (UINT) height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    HRESULT hresult;

    const D3D_FEATURE_LEVEL featureLevelArray[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0,};

    hresult = D3D11CreateDeviceAndSwapChain(nullptr,
                                            D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevelArray,
                                            2, D3D11_SDK_VERSION,
                                            &sd, &d3d_swap_chain_, &d3d_device_,
                                            nullptr, &d3d_device_context_);
    if (FAILED(hresult)) {
        std::wcerr << L"D3D11CreateDeviceAndSwapChain failed" << std::endl;
        return false;
    }

    ID3D11Texture2D *pBackBuffer;
    hresult = d3d_swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &pBackBuffer);
    if (FAILED(hresult)) {
        std::wcerr << L"GetBuffer failed" << std::endl;
        return false;
    }

    hresult = d3d_device_->CreateRenderTargetView(pBackBuffer, nullptr, &d3d_render_target_view_);

    pBackBuffer->Release();
    if (FAILED(hresult)) {
        std::wcerr << L"CreateRenderTargetView failed" << std::endl;
        return false;
    }

    d3d_device_context_->OMSetRenderTargets(1, &d3d_render_target_view_, nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT) width;
    vp.Height = (FLOAT) height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    d3d_device_context_->RSSetViewports(1, &vp);
    return true;
}

void D3dDemo::Render()
{
    float ClearColor[4] = {1.0f, 0.125f, 0.6f, 1.0f}; // RGBA
    d3d_device_context_->ClearRenderTargetView(d3d_render_target_view_, ClearColor);
    d3d_swap_chain_->Present(0, 0);
}

void D3dDemo::Release()
{
    d3d_render_target_view_->Release();
    d3d_swap_chain_->Release();
    d3d_device_context_->Release();
    d3d_device_->Release();
}
