#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanRenderPass.hpp"

#include "Graphics/Format.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanFormat.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanRenderPass::VulkanRenderPass(_In_ GraphicsContext& InContext, _In_ const RenderPassCreateInformation& InCreateInformation)
			: RenderPass(InCreateInformation)
			, _Device(static_cast<VulkanDevice&>(InContext.GetDevice()))
		{
			using namespace Eternal::Graphics::Vulkan;

			vk::Device& Device = _Device.GetVulkanDevice();

			vector<vk::AttachmentDescription> VulkanAttachments;
			vector<vk::AttachmentReference> VulkanAttachmentReferences;
			vector<vk::ImageView> AttachmentViews;
			VulkanAttachments.resize(InCreateInformation.RenderTargets.size() + (InCreateInformation.DepthStencilRenderTarget ? 1 : 0));
			VulkanAttachmentReferences.resize(InCreateInformation.RenderTargets.size());
			AttachmentViews.resize(InCreateInformation.RenderTargets.size() + (InCreateInformation.DepthStencilRenderTarget ? 1 : 0));
			
			uint32_t RenderTargetIndex = 0;
			for (; RenderTargetIndex < GetRenderTargets().size(); ++RenderTargetIndex)
			{
				const RenderTargetInformation& CurrentRenderTargetInformation	= GetRenderTargets()[RenderTargetIndex];
				View* CurrentRenderTarget										= CurrentRenderTargetInformation.RenderTarget;

				vk::ImageLayout CurrentRenderTargetLayout	= vk::ImageLayout::eColorAttachmentOptimal;

				VulkanAttachments[RenderTargetIndex] = vk::AttachmentDescription(
					vk::AttachmentDescriptionFlagBits(),
					ConvertFormatToVulkanFormat(CurrentRenderTarget->GetViewFormat()).Format,
					vk::SampleCountFlagBits::e1,
					ConvertLoadOperatorToVulkanAttachmentLoadOperator(CurrentRenderTargetInformation.Operator.Load),
					ConvertStoreOperatorToVulkanAttachmentStoreOperator(CurrentRenderTargetInformation.Operator.Store),
					vk::AttachmentLoadOp::eDontCare,
					vk::AttachmentStoreOp::eDontCare,
					CurrentRenderTargetLayout,
					CurrentRenderTargetLayout
				);

				VulkanAttachmentReferences[RenderTargetIndex]	= vk::AttachmentReference(RenderTargetIndex, CurrentRenderTargetLayout);
				AttachmentViews[RenderTargetIndex]				= static_cast<VulkanView*>(CurrentRenderTarget)->GetVulkanImageView();
			}

			vk::AttachmentReference DepthStencilAttachmentReference;
			View* InDepthStencilRenderTarget = InCreateInformation.DepthStencilRenderTarget;
			if (InDepthStencilRenderTarget)
			{
				DepthStencilAttachmentReference			= vk::AttachmentReference(RenderTargetIndex, vk::ImageLayout::eDepthStencilAttachmentOptimal);
				VulkanAttachments[RenderTargetIndex]	= vk::AttachmentDescription(
					vk::AttachmentDescriptionFlagBits(),
					ConvertFormatToVulkanFormat(InDepthStencilRenderTarget->GetViewFormat()).Format,
					vk::SampleCountFlagBits::e1,
					ConvertLoadOperatorToVulkanAttachmentLoadOperator(InCreateInformation.DepthStencilOperator.Load),
					ConvertStoreOperatorToVulkanAttachmentStoreOperator(InCreateInformation.DepthStencilOperator.Store),
					vk::AttachmentLoadOp::eDontCare,
					vk::AttachmentStoreOp::eDontCare,
					vk::ImageLayout::eDepthStencilAttachmentOptimal,
					vk::ImageLayout::eDepthStencilAttachmentOptimal
				);
				AttachmentViews[RenderTargetIndex]		= static_cast<VulkanView*>(InDepthStencilRenderTarget)->GetVulkanImageView();
			}

			vk::SubpassDescription VulkanSubPass(
				vk::SubpassDescriptionFlagBits(),
				vk::PipelineBindPoint::eGraphics,
				0, nullptr,
				uint32_t(VulkanAttachmentReferences.size()),
				VulkanAttachmentReferences.data(),
				nullptr,
				InDepthStencilRenderTarget ? &DepthStencilAttachmentReference : nullptr,
				0, nullptr
			);

			vk::RenderPassCreateInfo RenderPassInfo(
				vk::RenderPassCreateFlagBits(),
				static_cast<uint32_t>(VulkanAttachments.size()),
				VulkanAttachments.data(),
				1, &VulkanSubPass,
				0, nullptr
			);

			VerifySuccess(
				Device.createRenderPass(&RenderPassInfo, nullptr, &_RenderPass)
			);

			vk::FramebufferCreateInfo FrameBufferInfo(
				vk::FramebufferCreateFlagBits(),
				_RenderPass,
				static_cast<uint32_t>(AttachmentViews.size()),
				AttachmentViews.data(),
				InCreateInformation.Viewport.GetWidth(),
				InCreateInformation.Viewport.GetHeight(),
				1
			);
	
			VerifySuccess(
				Device.createFramebuffer(&FrameBufferInfo, nullptr, &_FrameBuffer)
			);
		}

		VulkanRenderPass::~VulkanRenderPass()
		{
			vk::Device& VkDevice = _Device.GetVulkanDevice();
			VkDevice.destroyRenderPass(_RenderPass);
			VkDevice.destroyFramebuffer(_FrameBuffer);
		}
	}
}

#endif
