#pragma once

#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Window;
		class GraphicsContext;
		class Resource;
		class View;

		class SwapChain
		{
		public:
			SwapChain();
			virtual ~SwapChain();

			virtual void Acquire(GraphicsContext& Context) = 0;
			virtual void Present(GraphicsContext& Context) = 0;

			std::vector<Resource*>& GetBackBuffers() { return _BackBuffers; }
			std::vector<View*>& GetBackBufferRenderTargetViews() { return _BackBufferRenderTargetViews; }

		protected:
			std::vector<Resource*>	_BackBuffers;
			std::vector<View*>		_BackBufferRenderTargetViews;
		};
	}
}
