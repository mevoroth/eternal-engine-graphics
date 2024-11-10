#pragma once

#include "Graphics/SwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullSwapChain final : public SwapChain
		{
		public:

			NullSwapChain(_In_ GraphicsContext& InContext)
				: SwapChain()
			{
				(void)InContext;

				_BackBuffers.resize(GraphicsContext::FrameBufferingCount);
				_BackBufferRenderTargetViews.resize(GraphicsContext::FrameBufferingCount);
			}

			virtual void Acquire(GraphicsContext& InContext) override final { (void)InContext; }
			virtual void Present(GraphicsContext& InContext) override final { (void)InContext; }

		};
	}
}
