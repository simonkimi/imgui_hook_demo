#pragma once

#include <memory>
#include "stdafx.h"

class HandleManager {
public:
    explicit HandleManager(HANDLE handle);

    HandleManager(const HandleManager &) = delete;

    HandleManager &operator=(const HandleManager &) = delete;

    HandleManager(HandleManager &&other) noexcept = default;

    HandleManager &operator=(HandleManager &&other) noexcept = default;

    [[nodiscard]] HANDLE get() const;

    [[nodiscard]] bool IsInvalid() const;

private:
    std::unique_ptr<std::remove_pointer<HANDLE>::type, void (*)(HANDLE)> handle_;

    static void CloseHandle(HANDLE handle);
};