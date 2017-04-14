#ifndef _VULKAN_RENDER_TARGET_HPP_
#define _VULKAN_RENDER_TARGET_HPP_

#include <cstdint>
#include "Vulkan/VulkanResource.hpp"
#include "NextGenGraphics/RenderTarget.hpp"
#include "Graphics/View.hpp"

struct VkFramebuffer_T;
struct VkImage_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class VulkanRenderPass;
		class VulkanView;

		class VulkanRenderTarget
			: public View
			, public RenderTarget
		{
		public:
			VulkanRenderTarget(_In_ Device& DeviceObj, _In_ VulkanRenderPass& RenderPassObj, _In_ VulkanView* RenderTargetViews[], _In_ uint32_t RenderTargetViewsCount, _In_ uint32_t Width, _In_ uint32_t Height);

			virtual RenderTarget& GetAsRenderTarget() override;
			VkFramebuffer_T*& GetFrameBuffer() { return _FrameBuffer; }

		private:
			VkFramebuffer_T* _FrameBuffer = nullptr;
		};
	}
}

#endif
