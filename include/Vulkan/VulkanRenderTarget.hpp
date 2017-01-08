#ifndef _VULKAN_RENDER_TARGET_HPP_
#define _VULKAN_RENDER_TARGET_HPP_

#include <vulkan/vulkan.h>

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;

		class VulkanRenderTarget
		{
		public:
			//VulkanRenderTarget(_In_ VulkanDevice& Device);
			VulkanRenderTarget(_In_ VkSurfaceKHR& RenderTargetHandle);

			VkSurfaceKHR& GetRenderTarget() { return _RenderTarget; }

		private:
			VkSurfaceKHR _RenderTarget;
		};
	}
}

#endif
