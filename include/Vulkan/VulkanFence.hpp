#ifndef _VULKAN_FENCE_HPP_
#define _VULKAN_FENCE_HPP_

#include "Graphics/Fence.hpp"
#include <cstdint>
#include <vector>

struct VkFence_T;
struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		
		class Device;
		class CommandQueue;
		class CommandList;
		class SwapChain;

		class VulkanFence : public Fence
		{
		public:
			VulkanFence(_In_ Device& DeviceObj, _In_ uint32_t SimultaneousResourcesCount);
			~VulkanFence();

			VkFence_T* GetFence(/*_In_ uint32_t FenceIndex*/);

			virtual void Signal(_In_ SwapChain& SwapChainObj, _In_ CommandQueue& CommandQueueObj, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) override;
			virtual void Wait(_In_ Device& DeviceObj) override;
			virtual void Reset(_In_ Device& DeviceObj) override;

		private:
			vector<VkFence_T*>	_Fences;
			Device&				_Device;
			uint32_t			_FenceIndex = 0u;
		};
	}
}

#endif
