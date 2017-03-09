#ifndef _VULKAN_FENCE_HPP_
#define _VULKAN_FENCE_HPP_

#include <cstdint>
#include <vector>

struct VkFence_T;
struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		
		class VulkanDevice;
		class VulkanCommandQueue;
		class VulkanCommandList;
		class VulkanSwapChain;

		class VulkanFence
		{
		public:
			VulkanFence(_In_ VulkanDevice& DeviceObj, _In_ uint32_t SimultaneousResourcesCount);
			~VulkanFence();

			VkFence_T* GetFence(/*_In_ uint32_t FenceIndex*/);

			void Signal(_In_ VulkanSwapChain& SwapChainObj, _In_ VulkanCommandQueue& CommandQueueObj, _In_ VulkanCommandList CommandLists[], _In_ uint32_t CommandListsCount);
			void Wait(_In_ VulkanDevice& DeviceObj);
			void Reset(_In_ VulkanDevice& DeviceObj);

		private:
			vector<VkFence_T*> _Fences;
			VulkanDevice& _Device;
			uint32_t _FenceIndex = 0u;
		};
	}
}

#endif
