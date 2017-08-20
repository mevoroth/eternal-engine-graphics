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
		class Context;

		class CommandQueue
		{
		public:
			virtual ~CommandQueue() {}
			virtual void Submit(_In_ Context& GfxContext, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) = 0;
		};
	}
}

#endif
