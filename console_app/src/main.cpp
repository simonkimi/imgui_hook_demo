#include "process_helper.h"

#define UNUSED [[maybe_unused]]

int main(UNUSED int argc, UNUSED  char *argv[]) {
    auto process_list = win32::GetProcessList();
    for (const auto &[pid, process]: process_list) {
        std::wcout << std::format(L"pid: {}, process: {}", pid, process) << std::endl;
    }

    auto process_list2 = win32::GetProcessList();
    for (const auto &[pid, process]: process_list2) {
        std::wcout << std::format(L"pid2: {}, process2: {}", pid, process) << std::endl;
    }

    return 0;
}