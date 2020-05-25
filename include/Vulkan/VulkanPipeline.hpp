#ifndef _VULKAN_PIPELINE_HPP_
#define _VULKAN_PIPELINE_HPP_

#include <vulkan/vulkan.hpp>
#include "Graphics/Pipeline.hpp"

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

#endif
