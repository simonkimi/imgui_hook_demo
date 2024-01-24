#include "process_helper.h"
#include "handle_manager.h"

DWORD win32::FindProcessById(LPCTSTR process_name) {
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HandleManager hSnapshot(::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

    if (hSnapshot.IsInvalid()) {
        return -1;
    }

    if (::Process32First(hSnapshot.get(), &pe32)) {
        do {
            if (lstrcmpi(pe32.szExeFile, process_name) == 0) {
                return pe32.th32ProcessID;
            }
        } while (::Process32Next(hSnapshot.get(), &pe32));
    }
    return -1;
}

bool win32::CreateRemoteThreadInjectDll(DWORD pid, LPCTSTR dll_path) {
    // 打开注入进程

    HandleManager hProcess(::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid));

    if (hProcess.get() == nullptr) {
        return false;
    }

    // 在注入进程中申请内存
    LPVOID dll_addr = ::VirtualAllocEx(hProcess.get(), nullptr, lstrlen(dll_path), MEM_COMMIT, PAGE_READWRITE);
    if (dll_addr == nullptr) {
        return false;
    }

    SIZE_T dll_size = (lstrlen(dll_path) + 1) * sizeof(TCHAR);

    // 注入dll文件名称
    if (!::WriteProcessMemory(hProcess.get(), dll_addr, dll_path, dll_size, nullptr)) {
        return false;
    }

    // 获取LoadLibraryA函数地址
    FARPROC load_lib_addr = ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "LoadLibraryA");
    if (load_lib_addr == nullptr) {
        return false;
    }

    // 在注入进程中创建远程线程
    HandleManager thread_handle(::CreateRemoteThread(
            hProcess.get(),
            nullptr,
            0,
            (LPTHREAD_START_ROUTINE) load_lib_addr,
            dll_addr,
            0,
            nullptr
    ));
    if (thread_handle.get() == nullptr) {
        return false;
    }

    return true;
}

std::list<std::tuple<DWORD, win32::tstring>> win32::GetProcessList() {
    PROCESSENTRY32 pe32;
    std::list<std::tuple<DWORD, tstring>> process_list;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    HandleManager hSnapshot(::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

    if (hSnapshot.IsInvalid()) {
        return process_list;
    }

    if (::Process32First(hSnapshot.get(), &pe32)) {
        do {
            process_list.emplace_back(pe32.th32ProcessID, pe32.szExeFile);
        } while (::Process32Next(hSnapshot.get(), &pe32));
    }

    return process_list;
}