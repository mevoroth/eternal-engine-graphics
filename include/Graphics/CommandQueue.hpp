#ifndef _COMMAND_QUEUE_HPP_
#define _COMMAND_QUEUE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandAllocator;
		class CommandList;
		class Fence;
		class SwapChain;

		class CommandQueue
		{
		public:
			virtual ~CommandQueue() {}
			virtual void Submit(_In_ uint32_t FrameIndex, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount, _In_ Fence& FenceObj, _In_ SwapChain& SwapChainObj) = 0;
		};
	}
}

#endif
