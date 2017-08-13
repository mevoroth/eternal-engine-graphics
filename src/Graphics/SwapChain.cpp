#include "Graphics/SwapChain.hpp"

#include "Window/Window.hpp"

using namespace Eternal::Graphics;

SwapChain::SwapChain(_In_ Window& WindowObj)
	: _BackBufferViewport(0, 0, WindowObj.GetWidth(), WindowObj.GetHeight())
{
}
