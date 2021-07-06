#include "Graphics/SwapChain.hpp"

#include "Window/Window.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/View.hpp"

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
				delete _BackBufferRenderTargetViews[BackBufferIndex];
				_BackBufferRenderTargetViews[BackBufferIndex] = nullptr;

				delete _BackBuffers[BackBufferIndex];
				_BackBuffers[BackBufferIndex] = nullptr;
			}
		}
	}
}
