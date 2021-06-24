#pragma once

#include "Graphics_deprecated/Pipeline.hpp"
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
			VulkanPipeline(Device& InDevice, const PipelineCreateInformation& CreateInformation);

			vk::Pipeline& GetVulkanPipeline() { return _Pipeline; }

		private:
			vk::Pipeline _Pipeline;
		};
	}
}
