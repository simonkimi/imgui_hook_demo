#pragma once


class ImguiDrawer {
public:
    static bool is_display_;
    static bool is_initialized_;

    static HRESULT APIENTRY Present(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags);

private:


    static void InitImgui(IDXGISwapChain *swap_chain);
};


