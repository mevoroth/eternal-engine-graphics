#ifndef _VULKAN_STATE_HPP_
#define _VULKAN_STATE_HPP_

struct VkPipeline_T;

namespace Eternal
{
	namespace Graphics
	{
		class Viewport;
		class Device;
		class RootSignature;
		class VulkanRenderPass;
		class Shader;

		class VulkanState
		{
		public:
			VulkanState(_In_ Device& DeviceObj, _In_ RootSignature& RootSignatureObj, _In_ VulkanRenderPass& RenderPassObj, _In_ Shader& VS, _In_ Shader& PS, _In_ Viewport& ViewportObj);

			VkPipeline_T* GetPipeline() { return _Pipeline; }

		private:
			VkPipeline_T* _Pipeline = nullptr;
		};
	}
}

#endif
