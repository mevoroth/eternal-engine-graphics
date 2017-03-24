#include "Vulkan/VulkanRenderPass.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Graphics/Format.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanFormat.hpp"

using namespace Eternal::Graphics;

VulkanRenderPass::VulkanRenderPass(_In_ Device& DeviceObj, _In_ const vector<View*>& RenderTargets, _In_ View* DepthStencil)
	: _Device(DeviceObj)
	, _RenderTargets(RenderTargets)
{
	vector<VkAttachmentDescription> VulkanAttachments;
	vector<VkAttachmentReference> VulkanAttachmentReferences;
	VulkanAttachments.resize(RenderTargets.size());
	VulkanAttachmentReferences.resize(RenderTargets.size());
	
	uint32_t RenderTargetIndex = 0;
	for (; RenderTargetIndex < _RenderTargets.size(); ++RenderTargetIndex)
	{
		VkAttachmentDescription& CurrentAttachment = VulkanAttachments[RenderTargetIndex];
		CurrentAttachment.flags				= 0;
		CurrentAttachment.format			= VULKAN_FORMATS[static_cast<VulkanView*>(_RenderTargets[RenderTargetIndex])->GetFormat()];
		CurrentAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
		CurrentAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
		CurrentAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
		CurrentAttachment.stencilLoadOp		= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		CurrentAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
		CurrentAttachment.initialLayout		= VK_IMAGE_LAYOUT_UNDEFINED;
		CurrentAttachment.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // FIX ME

		VulkanAttachmentReferences[RenderTargetIndex].attachment	= RenderTargetIndex;
		VulkanAttachmentReferences[RenderTargetIndex].layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	VkAttachmentReference DepthStencilAttachmentReference;
	if (DepthStencil)
	{
		DepthStencilAttachmentReference.attachment	= RenderTargetIndex;
		DepthStencilAttachmentReference.layout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	VkSubpassDescription VulkanSubPass;
	VulkanSubPass.flags						= 0;
	VulkanSubPass.pipelineBindPoint			= VK_PIPELINE_BIND_POINT_GRAPHICS;
	VulkanSubPass.inputAttachmentCount		= 0;
	VulkanSubPass.pInputAttachments			= nullptr;
	VulkanSubPass.colorAttachmentCount		= VulkanAttachmentReferences.size();
	VulkanSubPass.pColorAttachments			= VulkanAttachmentReferences.data();
	VulkanSubPass.pResolveAttachments		= nullptr;
	VulkanSubPass.pDepthStencilAttachment	= DepthStencil ? &DepthStencilAttachmentReference : nullptr;
	VulkanSubPass.preserveAttachmentCount	= 0;
	VulkanSubPass.pPreserveAttachments		= nullptr;

	VkRenderPassCreateInfo RenderPassInfo;
	RenderPassInfo.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassInfo.pNext			= nullptr;
	RenderPassInfo.flags			= 0;
	RenderPassInfo.attachmentCount	= VulkanAttachments.size();
	RenderPassInfo.pAttachments		= VulkanAttachments.data();
	RenderPassInfo.subpassCount		= 1;
	RenderPassInfo.pSubpasses		= &VulkanSubPass;
	RenderPassInfo.dependencyCount	= 0;
	RenderPassInfo.pDependencies	= nullptr;

	VkResult Result = vkCreateRenderPass(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &RenderPassInfo, nullptr, &_RenderPass);
	ETERNAL_ASSERT(!Result);
}

VulkanRenderPass::~VulkanRenderPass()
{
	vkDestroyRenderPass(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), _RenderPass, nullptr);
}
