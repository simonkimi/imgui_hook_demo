
#include "../include/dialog.h"
#include <iostream>

std::unique_ptr<std::string> OpenFileDialog(HWND__ *hwnd) {
    OPENFILENAME ofn;
    wchar_t szFile[MAX_PATH];
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("DLL 文件\0*.DLL\0All Files\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    if (::GetOpenFileName(&ofn)) {
        char str[MAX_PATH] = {0};
        WideCharToMultiByte(CP_ACP, 0, szFile, -1, str, MAX_PATH, nullptr, nullptr);
        return std::make_unique<std::string>(str);
    }
    return nullptr;
}