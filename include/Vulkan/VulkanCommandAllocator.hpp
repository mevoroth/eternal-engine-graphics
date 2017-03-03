#ifndef _VULKAN_COMMAND_ALLOCATOR_HPP_
#define _VULKAN_COMMAND_ALLOCATOR_HPP_

struct VkCommandPool_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanCommandAllocator
		{
		public:
			VulkanCommandAllocator();

			//inline VkCommandPool_T* GetVulkanCommandPool() { return _CommandPool; }

		private:
			//VkCommandPool_T* _CommandPool = nullptr;
		};
	}
}

#endif
