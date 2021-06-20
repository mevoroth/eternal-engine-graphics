#include "Vulkan_deprecated/VulkanRenderPass.hpp"

#include "Graphics_deprecated/Format.hpp"
#include "Graphics_deprecated/Viewport.hpp"
#include "NextGenGraphics/Context.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan_deprecated/VulkanDevice.hpp"
#include "Vulkan_deprecated/VulkanView.hpp"
#include "Vulkan_deprecated/VulkanFormat.hpp"

using namespace Eternal::Graphics;

VulkanRenderPass::VulkanRenderPass(_In_ GraphicsContext& Context, _In_ const RenderPassCreateInformation& CreateInformation)
	: RenderPass(CreateInformation)
	, _Device(Context.GetDevice())
{
	using namespace Vulkan;

	vk::Device& Device = static_cast<VulkanDevice&>(Context.GetDevice()).GetVulkanDevice();

	vector<vk::AttachmentDescription> VulkanAttachments;
	vector<vk::AttachmentReference> VulkanAttachmentReferences;
	vector<vk::ImageView> AttachmentViews;
	VulkanAttachments.resize(CreateInformation.RenderTargets.size());
	VulkanAttachmentReferences.resize(CreateInformation.RenderTargets.size());
	AttachmentViews.resize(CreateInformation.RenderTargets.size());
	
	for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < GetRenderTargets().size(); ++RenderTargetIndex)
	{
		VulkanAttachments[RenderTargetIndex] = vk::AttachmentDescription(
			vk::AttachmentDescriptionFlagBits(),
			VULKAN_FORMATS[int(static_cast<VulkanView*>(GetRenderTargets()[RenderTargetIndex])->GetFormat())],
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eDontCare
		).setFinalLayout(
			vk::ImageLayout::eColorAttachmentOptimal
		);

		VulkanAttachmentReferences[RenderTargetIndex] = vk::AttachmentReference(RenderTargetIndex, vk::ImageLayout::eColorAttachmentOptimal);

		AttachmentViews[RenderTargetIndex] = static_cast<VulkanView*>(GetRenderTargets()[RenderTargetIndex])->GetImageView();
	}

	vk::AttachmentReference DepthStencilAttachmentReference;
	if (CreateInformation.DepthStencilRenderTarget)
	{
		DepthStencilAttachmentReference = vk::AttachmentReference(0, vk::ImageLayout::eDepthStencilAttachmentOptimal);
	}

	vk::SubpassDescription VulkanSubPass(
		vk::SubpassDescriptionFlagBits(),
		vk::PipelineBindPoint::eGraphics,
		0, nullptr,
		uint32_t(VulkanAttachmentReferences.size()),
		VulkanAttachmentReferences.data(),
		nullptr,
		CreateInformation.DepthStencilRenderTarget ? &DepthStencilAttachmentReference : nullptr,
		0, nullptr
	);

	vk::RenderPassCreateInfo RenderPassInfo(
		vk::RenderPassCreateFlagBits(),
		uint32_t(VulkanAttachments.size()),
		VulkanAttachments.data(),
		1, &VulkanSubPass,
		0, nullptr
	);

	VerifySuccess(Device.createRenderPass(&RenderPassInfo, nullptr, &_RenderPass));

	vk::FramebufferCreateInfo FrameBufferInfo(
		vk::FramebufferCreateFlagBits(),
		_RenderPass,
		uint32_t(AttachmentViews.size()),
		AttachmentViews.data(),
		CreateInformation.Viewport.GetWidth(),
		CreateInformation.Viewport.GetHeight(),
		1
	);
	
	VerifySuccess(Device.createFramebuffer(&FrameBufferInfo, nullptr, &_FrameBuffer));
}

VulkanRenderPass::~VulkanRenderPass()
{
	static_cast<VulkanDevice&>(_Device).GetVulkanDevice().destroyRenderPass(_RenderPass, nullptr);
}
