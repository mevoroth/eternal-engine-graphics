#include "Vulkan/VulkanRenderTarget.hpp"

#include <cstdint>
#include <vector>
#include "Macros/Macros.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanRenderPass.hpp"

using namespace std;
using namespace Eternal::Graphics;

VulkanRenderTarget::VulkanRenderTarget(_In_ Device& DeviceObj, _In_ RenderPass& RenderPassObj, _In_ View* RenderTargetViews[], _In_ uint32_t RenderTargetViewsCount, _In_ uint32_t Width, _In_ uint32_t Height)
{
	vector<VkImageView> AttachmentViews;
	AttachmentViews.resize(RenderTargetViewsCount);
	for (uint32_t RenderTargetViewIndex = 0; RenderTargetViewIndex < RenderTargetViewsCount; ++RenderTargetViewIndex)
	{
		AttachmentViews[RenderTargetViewIndex] = static_cast<VulkanView*>(RenderTargetViews[RenderTargetViewIndex])->GetImageView();
	}

	VkFramebufferCreateInfo FrameBufferInfo;
	FrameBufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	FrameBufferInfo.pNext			= nullptr;
	FrameBufferInfo.flags			= 0;
	FrameBufferInfo.renderPass		= static_cast<VulkanRenderPass&>(RenderPassObj).GetVulkanRenderPass();
	FrameBufferInfo.attachmentCount	= AttachmentViews.size();
	FrameBufferInfo.pAttachments	= AttachmentViews.data();
	FrameBufferInfo.width			= Width;
	FrameBufferInfo.height			= Height;
	FrameBufferInfo.layers			= 1;

	VkResult Result = vkCreateFramebuffer(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &FrameBufferInfo, nullptr, &_FrameBuffer);
	ETERNAL_ASSERT(!Result);
}

//RenderTarget& VulkanRenderTarget::GetAsRenderTarget()
//{
//	return *this;
//}
