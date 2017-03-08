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
			VulkanFence(_In_ VulkanDevice& DeviceObj);
			~VulkanFence();

			VkFence_T* GetFence() { return _Fence; }

		private:
			VulkanDevice& _Device;
			VkFence_T* _Fence = nullptr;
		};
	}
}

#endif
