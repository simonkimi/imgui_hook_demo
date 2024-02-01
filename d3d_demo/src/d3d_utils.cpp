#include "d3d_utils.h"
#include <sstream>

void DxTrace(HRESULT hresult)
{
    if (!FAILED(hresult))
        return;

    _com_error error(hresult);
    LPCTSTR msg_template = _T("D3D error, \n Message: %s\n Description: %s\n Source: %s\n");
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
