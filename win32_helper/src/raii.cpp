#include "raii.h"

VirtualAllocRaii::VirtualAllocRaii(HANDLE hProcess, SIZE_T size) : hProcess_(hProcess), size_(size), pMemory_(nullptr)
{
    pMemory_ = ::VirtualAllocEx(hProcess, nullptr, size, MEM_COMMIT, PAGE_READWRITE);
}

VirtualAllocRaii::~VirtualAllocRaii()
{
    if (pMemory_ != nullptr) {
        ::VirtualFreeEx(hProcess_, pMemory_, 0, MEM_RELEASE);
    }
}

VirtualAllocRaii::VirtualAllocRaii(VirtualAllocRaii &&other) noexcept
        :
        hProcess_(other.hProcess_),
        pMemory_(other.pMemory_),
        size_(other.size_)
{
    other.pMemory_ = nullptr;
}

VirtualAllocRaii &VirtualAllocRaii::operator=(VirtualAllocRaii &&other) noexcept
{
    if (this != &other) {
        pMemory_ = other.pMemory_;
        hProcess_ = other.hProcess_;
        size_ = other.size_;
        other.pMemory_ = nullptr;
    }
    return *this;
}

bool VirtualAllocRaii::IsInvalid()
{
    return pMemory_ == nullptr;
}

LPVOID VirtualAllocRaii::Get()
{
    return pMemory_;
}

HandleRaii::HandleRaii(HANDLE handle) : handle_(handle) {}

HandleRaii::~HandleRaii()
{
    if (!IsInvalid()) {
        ::CloseHandle(handle_);
    }
}

HANDLE HandleRaii::Get() const
{
    return handle_;

}

bool HandleRaii::IsInvalid() const
{
    return handle_ == INVALID_HANDLE_VALUE || handle_ == nullptr;
}
