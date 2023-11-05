#pragma once

#if ETERNAL_ENABLE_VULKAN

#include "Graphics/RenderPass.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class VulkanDevice;
		class GraphicsContext;

		class VulkanRenderPass final : public RenderPass
		{
		public:
			VulkanRenderPass(_In_ GraphicsContext& Context, _In_ const RenderPassCreateInformation& CreateInformation);
			~VulkanRenderPass();

			const vk::RenderPass& GetVulkanRenderPass() const { return _RenderPass; }
			const vk::Framebuffer& GetFrameBuffer() const { return _FrameBuffer; }

		private:
			VulkanDevice&	_Device;
			vk::RenderPass	_RenderPass;
			vk::Framebuffer	_FrameBuffer;
		};
	}
}

#endif
