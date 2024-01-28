export module raii;

#include "Windows.h"


export class VirtualAllocRaii {
private:
    HANDLE hProcess_;
    LPVOID pMemory_;
    SIZE_T size_;

public:
    VirtualAllocRaii(HANDLE hProcess, SIZE_T size) : hProcess_(hProcess), size_(size), pMemory_(nullptr)
    {
        pMemory_ = ::VirtualAllocEx(hProcess, nullptr, size, MEM_COMMIT, PAGE_READWRITE);
    }

    ~VirtualAllocRaii()
    {
        if (pMemory_ != nullptr) {
            ::VirtualFreeEx(hProcess_, pMemory_, 0, MEM_RELEASE);
        }
    }

    VirtualAllocRaii(const VirtualAllocRaii &) = delete;

    VirtualAllocRaii &operator=(const VirtualAllocRaii &) = delete;


    VirtualAllocRaii(VirtualAllocRaii &&other) noexcept
            :
            hProcess_(other.hProcess_),
            pMemory_(other.pMemory_),
            size_(other.size_)
    {
        other.pMemory_ = nullptr;
    }


    VirtualAllocRaii &operator=(VirtualAllocRaii &&other) noexcept
    {
        if (this != &other) {
            pMemory_ = other.pMemory_;
            hProcess_ = other.hProcess_;
            size_ = other.size_;
            other.pMemory_ = nullptr;
        }
        return *this;
    }

    [[nodiscard]] bool IsInvalid()
    {
        return pMemory_ == nullptr;
    }

    [[nodiscard]] LPVOID Get()
    {
        return pMemory_;
    }
};


export class HandleRaii {
public:
    explicit HandleRaii(HANDLE handle) : handle_(handle) {}

    HandleRaii(const HandleRaii &) = delete;

    HandleRaii &operator=(const HandleRaii &) = delete;

    HandleRaii(HandleRaii &&other) noexcept = default;

    HandleRaii &operator=(HandleRaii &&other) noexcept = default;

    ~HandleRaii()
    {
        if (!IsInvalid()) {
            ::CloseHandle(handle_);
        }
    }

    [[nodiscard]] HANDLE Get() const
    {
        return handle_;

    }

    [[nodiscard]] bool IsInvalid() const
    {
        return handle_ == INVALID_HANDLE_VALUE || handle_ == nullptr;
    }

private:
    HANDLE handle_;
};