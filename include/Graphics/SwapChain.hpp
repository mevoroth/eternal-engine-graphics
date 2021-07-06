#pragma once

#include "Graphics/Viewport.hpp"
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class Window;
		class GraphicsContext;
		class Resource;
		class View;

		class SwapChain
		{
		public:
			virtual ~SwapChain();

			virtual void Acquire(GraphicsContext& Context) = 0;
			virtual void Present(GraphicsContext& Context) = 0;

			vector<Resource*>& GetBackBuffers() { return _BackBuffers; }
			vector<View*>& GetBackBufferRenderTargetViews() { return _BackBufferRenderTargetViews; }

		protected:
			SwapChain();

			vector<Resource*>	_BackBuffers;
			vector<View*>		_BackBufferRenderTargetViews;
		};
	}
}
