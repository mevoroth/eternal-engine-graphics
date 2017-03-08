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
		class VulkanDevice;
		class VulkanView;

		class VulkanRenderPass
		{
		public:
			VulkanRenderPass(_In_ VulkanDevice& DeviceObj, _In_ const vector<VulkanView*>& RenderTargets, _In_ const vector<VulkanRenderPass*>& SubPasses);
			VulkanRenderPass(_In_ VulkanDevice& DeviceObj, _In_ const vector<VulkanView*>& RenderTargets);
			~VulkanRenderPass();

			void Initialize();

			VkRenderPass_T* GetRenderPass() { return _RenderPass; }

		private:
			void _BuildSubPass(_In_ VulkanRenderPass* SubPass, _Out_ VkSubpassDescription& SubPassDescription);

			VulkanDevice& _Device;
			const vector<VulkanView*>& _RenderTargets;
			vector<VulkanRenderPass*> _SubPasses;
			VkRenderPass_T* _RenderPass = nullptr;
		};
	}
}

#endif
