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
<<<<<<< Updated upstream
			//VulkanRenderTarget(_In_ VulkanDevice& Device);
			VulkanRenderTarget(_In_ VkSurfaceKHR_T*& RenderTargetHandle);

			VkSurfaceKHR_T*& GetRenderTarget() { return _RenderTarget; }

		private:
<<<<<<< HEAD
			VkSurfaceKHR _RenderTarget;
=======
			VulkanRenderTarget();

		private:
>>>>>>> Stashed changes
=======
			VkSurfaceKHR_T* _RenderTarget;
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce
		};
	}
}

#endif
