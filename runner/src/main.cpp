#include "process_helper.h"
#include "iostream"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    auto process_list = win32::GetProcessList();
    for (const auto &[pid, process_name]: process_list) {
        std::wcout << std::format(L"pid: {}, process_name: {}", pid, process_name) << std::endl;
    }
    return 0;
}