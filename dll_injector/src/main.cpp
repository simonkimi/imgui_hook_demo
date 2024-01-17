#include "main.h"

DWORD FindProcessById(LPCTSTR process_name) {
    PROCESSENTRY32 pe32;
    HANDLE hSnapshot = INVALID_HANDLE_VALUE;
    DWORD pid = 0;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (::Process32First(hSnapshot, &pe32)) {
        do {
            if (lstrcmpi(pe32.szExeFile, process_name) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (::Process32Next(hSnapshot, &pe32));
    }

    ::CloseHandle(hSnapshot);
    return pid;
}

bool CreateRemoteThreadInjectDll(DWORD pid, LPCTSTR dll_path) {
    // 打开注入进程
    HANDLE handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (handle == nullptr) {
        return false;
    }

    // 在注入进程中申请内存
    LPVOID dll_addr = ::VirtualAllocEx(handle, nullptr, lstrlen(dll_path), MEM_COMMIT, PAGE_READWRITE);
    if (dll_addr == nullptr) {
        return false;
    }

    SIZE_T dll_size = lstrlen(dll_path) + sizeof(char);

    // 注入dll文件名称
    if (!::WriteProcessMemory(handle, dll_addr, dll_path, dll_size, nullptr)) {
        return false;
    }

    // 获取LoadLibraryA函数地址
    FARPROC load_lib_addr = ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "LoadLibraryA");
    if (load_lib_addr == nullptr) {
        return false;
    }

    // 在注入进程中创建远程线程
    HANDLE thread_handle = ::CreateRemoteThread(
            handle,
            nullptr,
            0,
            (LPTHREAD_START_ROUTINE) load_lib_addr,
            dll_addr,
            0,
            nullptr
    );
    if (thread_handle == nullptr) {
        return false;
    }

    // 关闭句柄
    ::CloseHandle(handle);
    return true;
}

