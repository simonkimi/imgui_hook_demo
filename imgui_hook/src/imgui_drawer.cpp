#include "imgui_drawer.h"
#include "imgui_impl/imgui_impl_dx11.h"
#include "imgui_impl/imgui_impl_win32.h"
#include "d3d_hook.h"

bool ImguiDrawer::is_initialized_ = false;

void ImguiDrawer::InitImgui(IDXGISwapChain *swap_chain)
{


}

HRESULT ImguiDrawer::Present(IDXGISwapChain *swap_chain, UINT sync_interval, UINT flags)
{
    return 0;
}
