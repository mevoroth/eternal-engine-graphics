#include "Vulkan/VulkanView.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanFormat.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const vk::ImageViewType VULKAN_IMAGE_VIEW_TYPES[] =
		{
			vk::ImageViewType(~0),
			vk::ImageViewType(~0),
			vk::ImageViewType::e1D,
			vk::ImageViewType::e1DArray,
			vk::ImageViewType::e2D,
			vk::ImageViewType::e2DArray,
			vk::ImageViewType::e3D
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_IMAGE_VIEW_TYPES) == static_cast<int32_t>(ViewRenderTargetType::VIEW_RENDER_TARGET_COUNT), "Mis-match between vulkan image view types and view render target types");

		VulkanView::VulkanView(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(InViewCreateInformation.Context.GetDevice()).GetVulkanDevice();
			VulkanResource& VkResource = static_cast<VulkanResource&>(InViewCreateInformation.GraphicsResource);

			switch (static_cast<VulkanResource&>(InViewCreateInformation.GraphicsResource).GetVulkanResourceType())
			{
			case VulkanResourceType::BUFFER:
				ETERNAL_BREAK();

			case VulkanResourceType::IMAGE:
				_SubresourceRange = vk::ImageSubresourceRange(
					vk::ImageAspectFlagBits::eColor
				);
				_SubresourceRange.levelCount = 1;
				_SubresourceRange.layerCount = 1;

				switch (InViewCreateInformation.ResourceViewRenderTargetType)
				{
				case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D:
					_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture1D.MipSlice;
					break;
				case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D_ARRAY:
					_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.MipSlice;
					_SubresourceRange.baseArrayLayer	= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.FirstArraySlice;
					_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.ArraySize;
					break;
				case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D:
					_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture2D.MipSlice;
					break;
				case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY:
					_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.MipSlice;
					_SubresourceRange.baseArrayLayer	= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.FirstArraySlice;
					_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.ArraySize;
					break;
				case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_3D:
					_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture3D.MipSlice;
					_SubresourceRange.baseArrayLayer	= InViewCreateInformation.MetaData.RenderTargetViewTexture3D.FirstWSlice;
					_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.RenderTargetViewTexture3D.WSize + 1;
					break;
				}

				vk::ImageViewCreateInfo CreateInfo;
				CreateInfo.flags			= vk::ImageViewCreateFlagBits();
				CreateInfo.image			= VkResource.GetVulkanImage();
				CreateInfo.viewType			= VULKAN_IMAGE_VIEW_TYPES[static_cast<int32_t>(InViewCreateInformation.ResourceViewRenderTargetType)];
				CreateInfo.format			= VULKAN_FORMATS[static_cast<int32_t>(InViewCreateInformation.GraphicsFormat)].Format;
				CreateInfo.components		= vk::ComponentMapping();
				CreateInfo.subresourceRange	= _SubresourceRange;
				
				Vulkan::VerifySuccess(
					VkDevice.createImageView(&CreateInfo, nullptr, &_VulkanViewMetaData.ImageView)
				);
				break;
			}
		}

		VulkanView::~VulkanView()
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetViewCreateInformation().Context.GetDevice()).GetVulkanDevice();

			switch (GetVulkanResourceType())
			{
			case VulkanResourceType::BUFFER:
				VkDevice.destroyBufferView(_VulkanViewMetaData.BufferView);
				break;

			case VulkanResourceType::IMAGE:
				VkDevice.destroyImageView(_VulkanViewMetaData.ImageView);
			}
		}

		VulkanResourceType VulkanView::GetVulkanResourceType()
		{
			return static_cast<VulkanResource&>(GetViewCreateInformation().GraphicsResource).GetVulkanResourceType();
		}

		vk::ImageView& VulkanView::GetVulkanImageView()
		{
			ETERNAL_ASSERT(GetVulkanResourceType() == VulkanResourceType::IMAGE);
			return _VulkanViewMetaData.ImageView;
		}

		vk::BufferView& VulkanView::GetVulkanBufferView()
		{
			ETERNAL_ASSERT(GetVulkanResourceType() == VulkanResourceType::BUFFER);
			return _VulkanViewMetaData.BufferView;
		}
	}
}
