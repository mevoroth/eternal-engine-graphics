#ifndef _VULKAN_RENDER_TARGET_HPP_
#define _VULKAN_RENDER_TARGET_HPP_

#include "Vulkan/VulkanResource.hpp"

struct VkSurfaceKHR_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;

		class VulkanRenderTarget : public VulkanResource
		{
		public:
			//VulkanRenderTarget(_In_ VulkanDevice& Device);
			VulkanRenderTarget(_In_ VkSurfaceKHR_T*& RenderTargetHandle);

			VkSurfaceKHR_T*& GetRenderTarget() { return _RenderTarget; }

		private:
			VkSurfaceKHR_T* _RenderTarget;
		};
	}
}

#endif
