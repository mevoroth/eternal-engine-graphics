#include "Graphics/SwapChain.hpp"

#include "Window/Window.hpp"

namespace Eternal
{
	namespace Graphics
	{
		SwapChain::SwapChain(_In_ Window& WindowObj)
			: _BackBufferViewport(0, 0, WindowObj.GetWidth(), WindowObj.GetHeight())
		{
		}

		SwapChain::~SwapChain()
		{
			for (uint32_t BackBufferIndex = 0; BackBufferIndex < _BackBuffers.size(); ++BackBufferIndex)
			{
				delete _BackBuffers[BackBufferIndex];
				_BackBuffers[BackBufferIndex] = nullptr;
			}
		}
	}
}
