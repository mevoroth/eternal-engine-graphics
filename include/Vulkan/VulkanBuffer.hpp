#ifndef _VULKAN_BUFFER_HPP_
#define _VULKAN_BUFFER_HPP_

struct VkBuffer_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;

		class VulkanBuffer
		{
		public:
			VulkanBuffer(_In_ VulkanDevice& Device);

			void* Map(_In_ VulkanDevice& Device);
			void Unmap();

		private:
			VkBuffer_T* _Buffer = nullptr;
		};
	}
}

#endif
