#pragma once

#include "Graphics/Pipeline.hpp"
#include "Vulkan/VulkanHeader.hpp"

struct VkPipeline_T;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class VulkanPipeline : public Pipeline
		{
		public:
			VulkanPipeline(
				_In_ Device& InDevice,
				_In_ const PipelineCreateInformation& InPipelineCreateInformation
			);

			const vk::Pipeline& GetVulkanPipeline() const { return _Pipeline; }

		private:
			vk::Pipeline _Pipeline;
		};
	}
}
