#pragma once

#include "Graphics/RenderPass.hpp"
#include <vulkan/vulkan.hpp>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class Device;
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
