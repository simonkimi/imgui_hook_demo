#include "handle_manager.h"

HandleManager::HandleManager(HANDLE handle) : handle_(handle) {}

HANDLE HandleManager::get() const {
    return handle_;
}

bool HandleManager::IsInvalid() const {
    return handle_ == INVALID_HANDLE_VALUE || handle_ == nullptr;
}


HandleManager::~HandleManager() {
    if (!IsInvalid()) {
        ::CloseHandle(handle_);
    }
}