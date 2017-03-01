#ifndef _VULKAN_COMMAND_LIST_HPP_
#define _VULKAN_COMMAND_LIST_HPP_

struct VkCommandPool_T;
struct VkCommandBuffer_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;
		class VulkanCommandList
		{
		public:
			VulkanCommandList(_In_ VulkanDevice& Device);
			~VulkanCommandList();

		private:
			VulkanDevice& _Device;

			VkCommandPool_T* _CommandPool = nullptr;
			VkCommandBuffer_T* _CommandBuffer = nullptr;
		};
	}
}

#endif
