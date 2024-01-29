#pragma once

#include "pch.h"
#include <functional>
#include <queue>
#include <mutex>


using EventCallback = std::function<void()>;

class WindowsEventLoop {
public:
    /// 为窗口注册热键
    /// \param hwnd 
    explicit WindowsEventLoop(HWND hwnd);

    /// 为线程注册热键
    explicit WindowsEventLoop();

    ~WindowsEventLoop();

    /// 注册热键
    /// \param id 热键Id, 需要确保唯一
    /// \param modifiers 
    /// \param vk 
    /// \param callback 热键按下后的回调函数
    void RegisterHotKey(int id, UINT modifiers, UINT vk, EventCallback callback);


    /// 启动事件循环
    void Run();

    /// 停止事件循环
    void Stop();

private:
    std::atomic<bool> running_ = true;
    HWND hwnd_;
    std::unordered_map<DWORD, EventCallback> hotkeys_{};
    std::mutex mutex_;
};