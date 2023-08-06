#pragma once

#include "Graphics/Pipeline.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class VulkanPipeline final : public Pipeline
		{
		public:
			VulkanPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
			);

			VulkanPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
			);

			VulkanPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
			);

			VulkanPipeline(
				_Inout_ GraphicsContext& InOutContext,
				_In_ const RayTracingPipelineCreateInformation& InPipelineCreateInformation
			);

			inline const vk::Pipeline& GetVulkanPipeline() const { return _Pipeline; }
			inline vk::Pipeline& GetVulkanPipeline() { return _Pipeline; }

			VulkanPipeline& VulkanPipeline::operator=(_In_ const VulkanPipeline& InPipeline);

			virtual bool IsPipelineCompiled() const override final;

		private:
			vk::Pipeline _Pipeline;
		};
	}
}
