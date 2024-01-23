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

    ~HandleManager();

    [[nodiscard]] HANDLE get() const;

    [[nodiscard]] bool IsInvalid() const;

private:
    HANDLE handle_;
};