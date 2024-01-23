#include "../include/handle_manager.h"

HandleManager::HandleManager(HANDLE handle) : handle_(handle, &HandleManager::CloseHandle) {}

HANDLE HandleManager::get() const { 
    return handle_.get(); 
}

bool HandleManager::IsInvalid() const { 
    return handle_.get() == INVALID_HANDLE_VALUE || get() == nullptr; 
}

void HandleManager::CloseHandle(HANDLE handle) {
    if (handle != INVALID_HANDLE_VALUE && handle != nullptr) {
        ::CloseHandle(handle);
    }
}