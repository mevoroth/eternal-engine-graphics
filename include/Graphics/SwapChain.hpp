#pragma once

#include "Graphics/Viewport.hpp"
#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Window;
		class GraphicsContext;

		class SwapChain
		{
		public:
			SwapChain(_In_ Window& WindowObj);
			virtual ~SwapChain() {}

			virtual void Acquire(GraphicsContext& Context) = 0;
			virtual void Present(GraphicsContext& Context) = 0;

		private:
			Viewport	_BackBufferViewport;
		};
	}
}
