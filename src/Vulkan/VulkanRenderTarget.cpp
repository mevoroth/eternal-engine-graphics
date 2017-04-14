#include "Vulkan/VulkanRenderTarget.hpp"

#include <vector>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanRenderPass.hpp"

using namespace std;
using namespace Eternal::Graphics;

VulkanRenderTarget::VulkanRenderTarget(_In_ Device& DeviceObj, _In_ VulkanRenderPass& RenderPassObj, _In_ VulkanView* RenderTargetViews[], _In_ uint32_t RenderTargetViewsCount, _In_ uint32_t Width, _In_ uint32_t Height)
{
	vector<VkImageView> AttachmentViews;
	AttachmentViews.resize(RenderTargetViewsCount);
	for (unsigned int RenderTargetViewIndex = 0; RenderTargetViewIndex < RenderTargetViewsCount; ++RenderTargetViewIndex)
	{
		AttachmentViews[RenderTargetViewIndex] = RenderTargetViews[RenderTargetViewIndex]->GetImageView();
	}

	VkFramebufferCreateInfo FrameBufferInfo;
	FrameBufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	FrameBufferInfo.pNext			= nullptr;
	FrameBufferInfo.flags			= 0;
	FrameBufferInfo.renderPass		= RenderPassObj.GetRenderPass();
	FrameBufferInfo.attachmentCount	= AttachmentViews.size();
	FrameBufferInfo.pAttachments	= AttachmentViews.data();
	FrameBufferInfo.width			= Width;
	FrameBufferInfo.height			= Height;
	FrameBufferInfo.layers			= 1;

	VkResult Result = vkCreateFramebuffer(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &FrameBufferInfo, nullptr, &_FrameBuffer);
	ETERNAL_ASSERT(!Result);
}

RenderTarget& VulkanRenderTarget::GetAsRenderTarget()
{
	return *this;
}
