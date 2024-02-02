#pragma once

#include "pch.h"
#include <spdlog/sinks/base_sink.h>

template<typename Mutex>
class output_debug_string_sink : public spdlog::sinks::base_sink<Mutex>
{
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        OutputDebugStringA(fmt::to_string(formatted).c_str());
    }

    void flush_() override
    {
        // 在这里实现如何flush数据（如果需要）
    }
};