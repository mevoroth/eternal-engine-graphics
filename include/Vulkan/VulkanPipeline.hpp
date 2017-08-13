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
		class InputLayout;
		class RenderPass;
		class Shader;
		class DepthTest;
		class StencilTest;

		class VulkanPipeline : public Pipeline
		{
		public:
			VulkanPipeline(
				_In_ Device& DeviceObj,
				_In_ RootSignature& RootSignatureObj,
				_In_ InputLayout& InputLayoutObj,
				_In_ RenderPass& RenderPassObj,
				_In_ Shader& VS,
				_In_ Shader& PS,
				_In_ const DepthTest& DepthTestObj,
				_In_ const StencilTest& StencilTestObj,
				_In_ const Viewport& ViewportObj
			);

			VkPipeline_T* GetVulkanPipeline() { return _Pipeline; }

		private:
			VkPipeline_T* _Pipeline = nullptr;
		};
	}
}

#endif
