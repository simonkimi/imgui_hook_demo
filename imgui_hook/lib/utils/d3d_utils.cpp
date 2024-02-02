#include "d3d_utils.h"
#include <sstream>

void DxTrace(HRESULT hresult)
{
    if (!FAILED(hresult))
        return;

    _com_error error(hresult);
    std::basic_stringstream<TCHAR> tss;
    tss << _T("D3D error, n Message: ")
        << error.ErrorMessage()
        << _T("n Description: ")
        << static_cast<LPCTSTR>(error.Description())
        << _T("n Source: ")
        << static_cast<LPCTSTR>(error.Source())
        << std::endl;
    auto err_msg = tss.str();
    std::wcerr << err_msg << std::endl;
    int id = MessageBox(nullptr, err_msg.c_str(), _T("D3D error"), MB_ICONERROR | MB_ABORTRETRYIGNORE);
    switch (id) {
        case IDRETRY:
            __debugbreak();
            break;
        case IDIGNORE:
            break;
        case IDABORT:
        default:
            exit(1);
    }
}

void GetDx11VTable(HWND hwnd, void **v_table, int size)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = (GetWindowLongPtr(sd.OutputWindow, GWL_STYLE) & WS_POPUP) == 0;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ComPtr<ID3D11Device> d3d_device;
    ComPtr<IDXGISwapChain> d3d_swap_chain;

    HRESULT hresult = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            d3d_swap_chain.GetAddressOf(),
            d3d_device.GetAddressOf(),
            nullptr,
            nullptr
    );
    if (hresult == DXGI_ERROR_UNSUPPORTED) {
        hresult = D3D11CreateDeviceAndSwapChain(
                nullptr,
                D3D_DRIVER_TYPE_WARP,
                nullptr,
                0,
                nullptr,
                0,
                D3D11_SDK_VERSION,
                &sd,
                d3d_swap_chain.GetAddressOf(),
                d3d_device.GetAddressOf(),
                nullptr,
                nullptr
        );
    }
    HR(hresult)
    memcpy(v_table, *(void ***) (d3d_swap_chain.Get()), size);
}
