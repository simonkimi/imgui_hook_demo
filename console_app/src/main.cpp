#include "process_helper.h"
#include <d3d9.h>
#include <wrl/client.h>
#include <comdef.h>
#include <sstream>
#include <memory>

std::unique_ptr<TCHAR[]> GetHRResult(HRESULT hresult)
{
    _com_error error(hresult);
    size_t len = 2048;
    auto buffer = std::make_unique<TCHAR[]>(len);
    _sntprintf_s(buffer.get(),
                 len,
                 _TRUNCATE,
                 _T("D3D error,Message: \n%s\nDescription: \n%s\nSource: \n%s\n"),
                 error.ErrorMessage(),
                 static_cast<LPCTSTR>(error.Description()),
                 static_cast<LPCTSTR>(error.Source())
    );
    return buffer;
}


void DxTrace(HRESULT hresult, bool use_msgbox = true)
{
    if (!FAILED(hresult))
        return;
    
    auto err_msg = GetHRResult(hresult);
    OutputDebugStringW(err_msg.get());
    if (!use_msgbox)
        return;
    int id = MessageBox(nullptr, err_msg.get(), _T("D3D error"), MB_ICONERROR | MB_ABORTRETRYIGNORE);
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

BOOL GetDx9VTable(HWND hwnd, void **v_table, int size)
{
    Microsoft::WRL::ComPtr<IDirect3DDevice9> device;
    Microsoft::WRL::ComPtr<IDirect3D9> d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.Windowed = (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_POPUP) == 0;

    HRESULT hresult = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, device.GetAddressOf());
    if (FAILED(hresult)) {
        DxTrace(hresult);
        d3dpp.Windowed = !d3dpp.Windowed;
        hresult = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, device.GetAddressOf());
    }

    if (FAILED(hresult)) {
        DxTrace(hresult, true);
        return FALSE;
    }

    memcpy(v_table, *(void ***) device.Get(), size);
    return TRUE;
}


int main(int argc, char *argv[])
{
    HWND hwnd = GetConsoleWindow();
    void *v_table[119];
    if (!GetDx9VTable(hwnd, v_table, sizeof(v_table))) {
        return 1;
    }
    return 0;
}