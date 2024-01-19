
#include "../include/dialog.h"
#include <iostream>


bool OpenFileDialog(HWND__ *hwnd, TCHAR *filePath, DWORD size) {
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = size / sizeof(TCHAR);
    ofn.lpstrFilter = _T("DLL文件\0*.dll\0All Files\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    return GetOpenFileName(&ofn);
}
