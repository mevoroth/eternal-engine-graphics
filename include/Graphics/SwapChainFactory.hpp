#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class SwapChain;
		class GraphicsContext;

		SwapChain* CreateSwapChain(_In_ GraphicsContext& InContext);
	}
}
