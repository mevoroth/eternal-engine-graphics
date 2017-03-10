#ifndef _FENCE_HPP_
#define _FENCE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class SwapChain;
		class CommandQueue;
		class CommandList;

		class Fence
		{
		public:
			virtual void Signal(_In_ SwapChain& SwapChainObj, _In_ CommandQueue& CommandQueueObj, _In_ CommandList CommandLists[], _In_ uint32_t CommandListsCount) = 0;
			virtual void Wait(_In_ Device& DeviceObj) = 0;
			virtual void Reset(_In_ Device& DeviceObj) = 0;
		};
	}
}

#endif
