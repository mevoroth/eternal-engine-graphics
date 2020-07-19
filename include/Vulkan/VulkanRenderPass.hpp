#ifndef _VULKAN_RENDER_PASS_HPP_
#define _VULKAN_RENDER_PASS_HPP_

#include <vulkan/vulkan.hpp>
#include "Graphics/RenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class Device;
		class View;
		class BlendState;
		class Viewport;
		class GraphicsContext;

		class VulkanRenderPass : public RenderPass
		{
		public:
			VulkanRenderPass(_In_ GraphicsContext& Context, _In_ const RenderPassCreateInformation& CreateInformation);
			~VulkanRenderPass();

			vk::RenderPass& GetVulkanRenderPass() { return _RenderPass; }
			vk::Framebuffer& GetFrameBuffer() { return _FrameBuffer; }

		private:
			Device&			_Device;
			vk::RenderPass	_RenderPass;
			vk::Framebuffer	_FrameBuffer;
		};
	}
}

#endif
