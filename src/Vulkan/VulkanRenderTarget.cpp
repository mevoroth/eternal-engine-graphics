#include "Vulkan/VulkanRenderTarget.hpp"

#include "Macros/Macros.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanRenderPass.hpp"

using namespace Eternal::Graphics;

VulkanRenderTarget::VulkanRenderTarget(_In_ VulkanDevice& DeviceObj, _In_ VulkanRenderPass& RenderPassObj, _In_ VkImage_T*& VulkanImage, _In_ VulkanView& View, _In_ uint32_t Width, _In_ uint32_t Height)
	: VulkanResource(VulkanImage)
{
	VkFramebufferCreateInfo FrameBufferInfo;
	FrameBufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	FrameBufferInfo.pNext			= nullptr;
	FrameBufferInfo.flags			= 0;
	FrameBufferInfo.renderPass		= RenderPassObj.GetRenderPass();
	FrameBufferInfo.attachmentCount	= 1;
	FrameBufferInfo.pAttachments	= &View.GetImageView();
	FrameBufferInfo.width			= Width;
	FrameBufferInfo.height			= Height;
	FrameBufferInfo.layers			= 1;

	VkResult Result = vkCreateFramebuffer(DeviceObj.GetVulkanDevice(), &FrameBufferInfo, nullptr, &_FrameBuffer);
	ETERNAL_ASSERT(!Result);
}
