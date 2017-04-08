#ifndef _VULKAN_RENDER_PASS_HPP_
#define _VULKAN_RENDER_PASS_HPP_

#include <vector>

struct VkRenderPass_T;
struct VkSubpassDescription;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class Device;
		class View;

		class VulkanRenderPass
		{
		public:
			VulkanRenderPass(_In_ Device& DeviceObj, _In_ const vector<View*>& RenderTargets, _In_ View* DepthStencil = nullptr);
			~VulkanRenderPass();

			VkRenderPass_T* GetRenderPass() { return _RenderPass; }

		private:
			Device& _Device;
			const vector<View*>& _RenderTargets;
			VkRenderPass_T* _RenderPass = nullptr;
		};
	}
}

#endif
