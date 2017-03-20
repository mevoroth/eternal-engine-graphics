#include "Vulkan/VulkanRenderPass.hpp"

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Graphics/Format.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanFormat.hpp"

using namespace Eternal::Graphics;

VulkanRenderPass::VulkanRenderPass(_In_ Device& DeviceObj, _In_ const vector<View*>& RenderTargets, _In_ const vector<VulkanRenderPass*>& SubPasses)
	: _Device(DeviceObj)
	, _RenderTargets(RenderTargets)
	, _SubPasses(SubPasses)
{
}

VulkanRenderPass::VulkanRenderPass(_In_ Device& DeviceObj, _In_ const vector<View*>& RenderTargets)
	: _Device(DeviceObj)
	, _RenderTargets(RenderTargets)
{
	_SubPasses.push_back(this);
}

VulkanRenderPass::~VulkanRenderPass()
{
	vkDestroyRenderPass(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), _RenderPass, nullptr);
}

void VulkanRenderPass::_BuildSubPass(_In_ VulkanRenderPass* SubPass, _Out_ VkSubpassDescription& SubPassDescription)
{
	vector<VkAttachmentReference> VulkanAttachementReferences;
	VulkanAttachementReferences.resize(SubPass->_RenderTargets.size());
	for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < SubPass->_RenderTargets.size(); ++RenderTargetIndex)
	{
		VulkanAttachementReferences[RenderTargetIndex].attachment	= RenderTargetIndex;
		VulkanAttachementReferences[RenderTargetIndex].layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}

	SubPassDescription.flags					= 0;
	SubPassDescription.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
	SubPassDescription.inputAttachmentCount		= 0;
	SubPassDescription.pInputAttachments		= nullptr;
	SubPassDescription.colorAttachmentCount		= VulkanAttachementReferences.size();
	SubPassDescription.pColorAttachments		= VulkanAttachementReferences.data();
	SubPassDescription.pResolveAttachments		= nullptr;
	SubPassDescription.pDepthStencilAttachment	= nullptr;
	SubPassDescription.preserveAttachmentCount	= 0;
	SubPassDescription.pPreserveAttachments		= nullptr;
}

void VulkanRenderPass::Initialize()
{
	vector<VkAttachmentDescription> VulkanAttachements;
	VulkanAttachements.resize(_RenderTargets.size());
	for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < _RenderTargets.size(); ++RenderTargetIndex)
	{
		VkAttachmentDescription& CurrentAttachment = VulkanAttachements[RenderTargetIndex];
		CurrentAttachment.flags				= 0;
		CurrentAttachment.format			= VULKAN_FORMATS[static_cast<VulkanView*>(_RenderTargets[RenderTargetIndex])->GetFormat()];
		CurrentAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
		CurrentAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
		CurrentAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
		CurrentAttachment.stencilLoadOp		= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		CurrentAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
		CurrentAttachment.initialLayout		= VK_IMAGE_LAYOUT_UNDEFINED;
		CurrentAttachment.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}

	vector< vector<VkAttachmentReference> > VulkanAttachmentReferences;
	vector<VkSubpassDescription> VulkanSubPasses;
	VulkanSubPasses.resize(_SubPasses.size());
	VulkanAttachmentReferences.resize(_SubPasses.size());

	for (uint32_t SubPassIndex = 0; SubPassIndex < _SubPasses.size(); ++SubPassIndex)
	{
		VulkanRenderPass*& SubPass					= _SubPasses[SubPassIndex];
		VkSubpassDescription& SubPassDescription	= VulkanSubPasses[SubPassIndex];

		VulkanAttachmentReferences[SubPassIndex].resize(SubPass->_RenderTargets.size());
		for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < SubPass->_RenderTargets.size(); ++RenderTargetIndex)
		{
			VulkanAttachmentReferences[SubPassIndex][RenderTargetIndex].attachment	= RenderTargetIndex;
			VulkanAttachmentReferences[SubPassIndex][RenderTargetIndex].layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		SubPassDescription.flags					= 0;
		SubPassDescription.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
		SubPassDescription.inputAttachmentCount		= 0;
		SubPassDescription.pInputAttachments		= nullptr;
		SubPassDescription.colorAttachmentCount		= VulkanAttachmentReferences[SubPassIndex].size();
		SubPassDescription.pColorAttachments		= VulkanAttachmentReferences[SubPassIndex].data();
		SubPassDescription.pResolveAttachments		= nullptr;
		SubPassDescription.pDepthStencilAttachment	= nullptr;
		SubPassDescription.preserveAttachmentCount	= 0;
		SubPassDescription.pPreserveAttachments		= nullptr;
	}

	VkRenderPassCreateInfo RenderPassInfo;
	RenderPassInfo.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassInfo.pNext			= nullptr;
	RenderPassInfo.flags			= 0;
	RenderPassInfo.attachmentCount	= VulkanAttachements.size();
	RenderPassInfo.pAttachments		= VulkanAttachements.data();
	RenderPassInfo.subpassCount		= VulkanSubPasses.size();
	RenderPassInfo.pSubpasses		= VulkanSubPasses.data();
	RenderPassInfo.dependencyCount	= 0;
	RenderPassInfo.pDependencies	= nullptr;

	VkResult Result = vkCreateRenderPass(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), &RenderPassInfo, nullptr, &_RenderPass);
	ETERNAL_ASSERT(!Result);
}
