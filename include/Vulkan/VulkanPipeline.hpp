#ifndef _VULKAN_PIPELINE_HPP_
#define _VULKAN_PIPELINE_HPP_

#include "Graphics/Pipeline.hpp"

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

		class VulkanPipeline : public Pipeline
		{
		public:
			VulkanPipeline(_In_ Device& DeviceObj, _In_ RootSignature& RootSignatureObj, _In_ VulkanRenderPass& RenderPassObj, _In_ Shader& VS, _In_ Shader& PS, _In_ Viewport& ViewportObj);

			VkPipeline_T* GetVulkanPipeline() { return _Pipeline; }

		private:
			VkPipeline_T* _Pipeline = nullptr;
		};
	}
}

#endif
