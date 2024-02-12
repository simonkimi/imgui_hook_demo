#include "dx_utils.h"
#include "sstream"

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

void DxTrace(HRESULT hresult, bool use_msgbox)
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