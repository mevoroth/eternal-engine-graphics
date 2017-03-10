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

	VkResult Result = vkCreateDescriptorSetLayout(Device.GetVulkanDevice(), &DescriptorSetLayoutInfo, nullptr, &_DescriptorSetLayout);
	ETERNAL_ASSERT(!Result);

	VkPipelineLayoutCreateInfo PipelineLayoutInfo;
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.pNext = nullptr;
	PipelineLayoutInfo.flags = 0;
	PipelineLayoutInfo.pushConstantRangeCount = 0;
	PipelineLayoutInfo.pPushConstantRanges = nullptr;
	PipelineLayoutInfo.setLayoutCount = 1;
	PipelineLayoutInfo.pSetLayouts = &_DescriptorSetLayout;

	Result = vkCreatePipelineLayout(Device.GetVulkanDevice(), &PipelineLayoutInfo, nullptr, &_PipelineLayout);
	ETERNAL_ASSERT(!Result);

	VkAttachmentDescription Attachment;
	Attachment.format = VK_FORMAT_B8G8R8A8_UNORM;
	Attachment.flags = 0;
	Attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	Attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	Attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	Attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	Attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	Attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	Attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference ColorReference;
	ColorReference.attachment = 0;
	ColorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription SubPass;
	SubPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	SubPass.flags = 0;
	SubPass.inputAttachmentCount = 0;
	SubPass.pInputAttachments = nullptr;
	SubPass.colorAttachmentCount = 1;
	SubPass.pColorAttachments = &ColorReference;
	SubPass.pResolveAttachments = nullptr;
	SubPass.pDepthStencilAttachment = nullptr;
	SubPass.preserveAttachmentCount = 0;
	SubPass.pPreserveAttachments = nullptr;

	VkRenderPassCreateInfo RenderPassInfo;
	RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassInfo.pNext = nullptr;
	RenderPassInfo.flags = 0;
	RenderPassInfo.attachmentCount = 1;
	RenderPassInfo.pAttachments = &Attachment;
	RenderPassInfo.subpassCount = 1;
	RenderPassInfo.pSubpasses = &SubPass;
	RenderPassInfo.dependencyCount = 0;
	RenderPassInfo.pDependencies = nullptr;

	Result = vkCreateRenderPass(Device.GetVulkanDevice(), &RenderPassInfo, nullptr, &_RenderPass);
	ETERNAL_ASSERT(!Result);
}

VkPipelineLayout_T* VulkanPipeline::GetPipelineLayout()
{
	return _PipelineLayout;
}

VkRenderPass_T* VulkanPipeline::GetRenderPass()
{
	return _RenderPass;
}
