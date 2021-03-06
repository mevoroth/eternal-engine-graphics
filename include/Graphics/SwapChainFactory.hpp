#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class SwapChain;
		class Window;
		class CommandQueue;
		class GraphicsContext;

		SwapChain* CreateSwapChain(_In_ GraphicsContext& Context);
	}
}
