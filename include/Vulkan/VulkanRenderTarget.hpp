#ifndef _VULKAN_RENDER_TARGET_HPP_
#define _VULKAN_RENDER_TARGET_HPP_

#include <cstdint>
#include "Vulkan/VulkanResource.hpp"
#include "NextGenGraphics/RenderTarget.hpp"

struct VkFramebuffer_T;
struct VkImage_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanRenderPass;
		class VulkanView;

		class VulkanRenderTarget
			: public VulkanResource
			, public RenderTarget
		{
		public:
			VulkanRenderTarget(_In_ VulkanDevice& DeviceObj, _In_ VulkanRenderPass& RenderPassObj, _In_ VkImage_T*& VulkanImage, _In_ VulkanView& View, _In_ uint32_t Width, _In_ uint32_t Height);

			VkFramebuffer_T*& GetFrameBuffer() { return _FrameBuffer; }

		private:
			VkFramebuffer_T* _FrameBuffer = nullptr;
		};
	}
}

#endif
