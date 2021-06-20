#ifndef _VULKAN_FENCE_HPP_
#define _VULKAN_FENCE_HPP_

#include "Graphics_deprecated/Fence.hpp"

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
			VulkanFence(_In_ Device& DeviceObj);
			~VulkanFence();

			VkFence_T* GetVulkanFence();

			virtual void Signal(_In_ CommandQueue& CommandQueueObj) override;
			virtual void Wait(_In_ Device& DeviceObj) override;
			virtual void Reset(_In_ Device& DeviceObj) override;

		private:
			Device&		_Device;
			VkFence_T*	_Fence	= nullptr;
		};
	}
}

#endif
