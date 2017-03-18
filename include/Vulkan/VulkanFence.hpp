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

			VkFence_T* GetFence();

			virtual void Signal(_In_ CommandQueue& CommandQueueObj) override;
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
