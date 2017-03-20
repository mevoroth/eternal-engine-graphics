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
			VulkanRenderPass(_In_ Device& DeviceObj, _In_ const vector<View*>& RenderTargets, _In_ const vector<VulkanRenderPass*>& SubPasses);
			VulkanRenderPass(_In_ Device& DeviceObj, _In_ const vector<View*>& RenderTargets);
			~VulkanRenderPass();

			void Initialize();

			VkRenderPass_T* GetRenderPass() { return _RenderPass; }

		private:
			void _BuildSubPass(_In_ VulkanRenderPass* SubPass, _Out_ VkSubpassDescription& SubPassDescription);

			Device& _Device;
			const vector<View*>& _RenderTargets;
			vector<VulkanRenderPass*> _SubPasses;
			VkRenderPass_T* _RenderPass = nullptr;
		};
	}
}

#endif
