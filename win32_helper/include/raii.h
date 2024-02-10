#pragma once

#include "pch.h"

class VirtualAllocRaii {
private:
    HANDLE hProcess_;
    LPVOID pMemory_;
    SIZE_T size_;

public:
    VirtualAllocRaii(HANDLE hProcess, SIZE_T size);

    ~VirtualAllocRaii();

    VirtualAllocRaii(const VirtualAllocRaii &) = delete;

    VirtualAllocRaii &operator=(const VirtualAllocRaii &) = delete;

    VirtualAllocRaii(VirtualAllocRaii &&other) noexcept;
    
    VirtualAllocRaii &operator=(VirtualAllocRaii &&other) noexcept;

    [[nodiscard]] bool IsInvalid();

    [[nodiscard]] LPVOID Get();
};


class HandleRaii {
public:
    explicit HandleRaii(HANDLE handle);

    HandleRaii(const HandleRaii &) = delete;

    HandleRaii &operator=(const HandleRaii &) = delete;

    HandleRaii(HandleRaii &&other) noexcept = default;

    HandleRaii &operator=(HandleRaii &&other) noexcept = default;

    ~HandleRaii();

    [[nodiscard]] HANDLE Get() const;

    [[nodiscard]] bool IsInvalid() const;

private:
    HANDLE handle_;
};