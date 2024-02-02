#pragma once


class ImguiDrawer {
public:
    static bool is_display_;
    static bool is_initialized_;
    static bool need_set_pos_;

    static HRESULT APIENTRY Present(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags);

private:
    static ImVec2 window_size_;
    static ImVec2 window_pos_;

    static void InitImgui(IDXGISwapChain *swap_chain);
};


