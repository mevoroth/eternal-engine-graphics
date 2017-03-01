#include "Vulkan/VulkanPipeline.hpp"

#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanDevice.hpp"

using namespace Eternal::Graphics;

VulkanPipeline::VulkanPipeline(_In_ VulkanDevice& Device)
{
	VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
	DescriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	DescriptorSetLayoutInfo.pNext = nullptr;
	DescriptorSetLayoutInfo.bindingCount = 0;
	DescriptorSetLayoutInfo.pBindings = nullptr;
	DescriptorSetLayoutInfo.flags = 0;

	VkResult Result = vkCreateDescriptorSetLayout(Device.GetDevice(), &DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout);
	ETERNAL_ASSERT(!Result);

	VkPipelineLayoutCreateInfo PipelineLayoutInfo;
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.pNext = nullptr;
	PipelineLayoutInfo.flags = 0;
	PipelineLayoutInfo.pushConstantRangeCount = 0;
	PipelineLayoutInfo.pPushConstantRanges = nullptr;
	PipelineLayoutInfo.setLayoutCount = 1;
	PipelineLayoutInfo.pSetLayouts = &_DescriptorSetLayout;

	vkCreatePipelineLayout(Device.GetDevice(), &PipelineLayoutInfo, nullptr, &_PipelineLayout);
}
