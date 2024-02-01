#include "d3d_demo.h"
#include "d3d_utils.h"

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

    // 创建D3D设备, 设备环境和交换链
    hresult = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevelArray,
            2, D3D11_SDK_VERSION,
            &sd, d3d_swap_chain_.GetAddressOf(), d3d_device_.GetAddressOf(),
            nullptr, d3d_device_context_.GetAddressOf());
    DxTrace(hresult);


    // 获取交换链的后台缓冲区
    ComPtr<ID3D11Texture2D> pBackBuffer;
    hresult = d3d_swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *) &pBackBuffer);
    DxTrace(hresult);
    // 创建渲染目标视图
    hresult = d3d_device_->CreateRenderTargetView(pBackBuffer.Get(), nullptr, d3d_render_target_view_.GetAddressOf());
    DxTrace(hresult);

    // 将视图绑定到输出合并阶段
    d3d_device_context_->OMSetRenderTargets(1, d3d_render_target_view_.GetAddressOf(), nullptr);

    // 设置可视窗口
    D3D11_VIEWPORT vp{0, 0, (FLOAT) width, (FLOAT) height, 0.0f, 1.0f};
    d3d_device_context_->RSSetViewports(1, &vp);
    return true;
}

void D3dDemo::Render()
{
    float ClearColor[4] = {1.0f, 0.125f, 0.6f, 1.0f}; // RGBA
    d3d_device_context_->ClearRenderTargetView(d3d_render_target_view_.Get(), ClearColor);
    HR(d3d_swap_chain_->Present(0, 0))
}

void D3dDemo::Release()
{
}


