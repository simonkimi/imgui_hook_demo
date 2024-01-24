#include "iostream"
#include "process_helper.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    auto process_list = Win32Helper::GetProcessList();
    for (auto &process: process_list) {
        DWORD pid;
        tstring process_name;
        std::tie(pid, process_name) = process;
        tcout << std::format(_T("pid: {}, process_name: {}"), pid, process_name) << std::endl;
    }
    return 0;
}