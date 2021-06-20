#ifndef _SWAP_CHAIN_FACTORY_HPP_
#define _SWAP_CHAIN_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class SwapChain;
		class Window;
		class CommandQueue;
		class GraphicsContext;

		SwapChain* CreateSwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj, _In_ CommandQueue& CommandQueueObj);
		SwapChain* CreateSwapChain(_In_ GraphicsContext& Context);
	}
}

#endif
