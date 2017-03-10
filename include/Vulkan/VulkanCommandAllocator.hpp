#ifndef _VULKAN_COMMAND_ALLOCATOR_HPP_
#define _VULKAN_COMMAND_ALLOCATOR_HPP_

struct VkCommandPool_T;
struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;

		class VulkanCommandAllocator
		{
		public:
			VulkanCommandAllocator(_In_ VulkanDevice& DeviceObj);
			virtual ~VulkanCommandAllocator();

			inline VkCommandPool_T*& GetVulkanCommandPool() { return _CommandPool; }
			inline VkSemaphore_T*& GetSemaphore() { return _CompletedSemaphore; }

		private:
			VulkanDevice&		_Device;
			VkCommandPool_T*	_CommandPool		= nullptr;
			VkSemaphore_T*		_CompletedSemaphore = nullptr;
		};
	}
}

#endif
