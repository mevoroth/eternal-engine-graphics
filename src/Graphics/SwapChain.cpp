#include "Graphics/SwapChain.hpp"

#include "Window/Window.hpp"
#include "Graphics/ViewFactory.hpp"
#include "Graphics/ResourceFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		SwapChain::SwapChain()
		{
		}

		SwapChain::~SwapChain()
		{
			for (uint32_t BackBufferIndex = 0; BackBufferIndex < _BackBuffers.size(); ++BackBufferIndex)
			{
				DestroyView(_BackBufferRenderTargetViews[BackBufferIndex]);
				DestroyResource(_BackBuffers[BackBufferIndex]);
			}
		}
	}
}
