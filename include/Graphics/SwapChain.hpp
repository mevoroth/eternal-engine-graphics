#pragma once

#include "Graphics/Viewport.hpp"
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class OutputDevice;
		class GraphicsContext;
		class Resource;
		class View;

		class SwapChain
		{
		public:
			virtual ~SwapChain();

			virtual void Acquire(GraphicsContext& InContext) = 0;
			virtual void Present(GraphicsContext& InContext) = 0;

			vector<Resource*>& GetBackBuffers() { return _BackBuffers; }
			vector<View*>& GetBackBufferRenderTargetViews() { return _BackBufferRenderTargetViews; }

		protected:
			SwapChain();

			vector<Resource*>	_BackBuffers;
			vector<View*>		_BackBufferRenderTargetViews;
		};
	}
}
