#ifndef _VULKAN_FENCE_HPP_
#define _VULKAN_FENCE_HPP_

struct VkFence_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;

		class VulkanFence
		{
		public:
			VulkanFence(_In_ VulkanDevice& Device);

		private:
			VkFence_T* _Fence = nullptr;
		};
	}
}

#endif
