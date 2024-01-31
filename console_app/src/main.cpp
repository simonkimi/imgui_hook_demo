#include "process_helper.h"


int main(int argc, char *argv[])
{
    DWORD pid = win32::FindProcessById(L"qt_windows.exe");
    std::cout << std::format("PID: {}", pid) << std::endl;

    auto modules = win32::GetProcessModules(pid);
    for (auto &[hMod, path]: modules) {
        std::wcout << std::format(L"hMod: {}, path: {}", (void*)hMod, path) << std::endl;
    }
}