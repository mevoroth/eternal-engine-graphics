#ifndef _VULKAN_RENDER_PASS_HPP_
#define _VULKAN_RENDER_PASS_HPP_

#include <vector>
#include "Graphics/RenderPass.hpp"

struct VkRenderPass_T;
struct VkFramebuffer_T;
struct VkSubpassDescription;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class Device;
		class View;
		class BlendState;
		class Viewport;

		class VulkanRenderPass : public RenderPass
		{
		public:
			VulkanRenderPass(_In_ Device& DeviceObj, _In_ const Viewport& ViewportObj, _In_ const vector<View*>& RenderTargets, _In_ const vector<BlendState*>& BlendStates, _In_ View* DepthStencil = nullptr, _In_ const LogicBlend& LogicBlendObj = LogicBlend());
			~VulkanRenderPass();

			VkRenderPass_T* GetVulkanRenderPass() { return _RenderPass; }
			VkFramebuffer_T* GetFrameBuffer() { return _FrameBuffer; }

		private:
			Device&				_Device;
			VkRenderPass_T*		_RenderPass = nullptr;
			VkFramebuffer_T*	_FrameBuffer = nullptr;
		};
	}
}

#endif
