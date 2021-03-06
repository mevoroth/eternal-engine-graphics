#include "Vulkan/VulkanView.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanFormat.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::Vulkan;

		static constexpr vk::ImageViewType VULKAN_IMAGE_VIEW_TYPES_RENDER_TARGET[] =
		{
			vk::ImageViewType(~0),
			vk::ImageViewType(~0),
			vk::ImageViewType::e1D,
			vk::ImageViewType::e1DArray,
			vk::ImageViewType::e2D,
			vk::ImageViewType::e2DArray,
			vk::ImageViewType::e3D
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_IMAGE_VIEW_TYPES_RENDER_TARGET) == static_cast<int32_t>(ViewRenderTargetType::VIEW_RENDER_TARGET_COUNT), "Mismatch between abstraction and vulkan image view types");

		vk::ImageViewType ConvertViewRenderTargetTypeToVulkanImageViewType(_In_ const ViewRenderTargetType& InViewRenderTargetType)
		{
			return VULKAN_IMAGE_VIEW_TYPES_RENDER_TARGET[static_cast<int32_t>(InViewRenderTargetType)];
		}

		//VIEW_SHADER_RESOURCE_UNKNOWN = 0, // StructuredBuffer
		//	VIEW_SHADER_RESOURCE_BUFFER,
		//	VIEW_SHADER_RESOURCE_TEXTURE_1D,
		//	VIEW_SHADER_RESOURCE_TEXTURE_1D_ARRAY,
		//	VIEW_SHADER_RESOURCE_TEXTURE_2D,
		//	VIEW_SHADER_RESOURCE_TEXTURE_2D_ARRAY,
		//	VIEW_SHADER_RESOURCE_TEXTURE_3D,
		//	VIEW_SHADER_RESOURCE_TEXTURE_CUBE,
		//	VIEW_SHADER_RESOURCE_TEXTURE_CUBE_ARRAY,
		static constexpr vk::ImageViewType VULKAN_IMAGE_VIEW_TYPES_SHADER_RESOURCE[] =
		{
			vk::ImageViewType(~0),
			vk::ImageViewType(~0),
			vk::ImageViewType::e1D,
			vk::ImageViewType::e1DArray,
			vk::ImageViewType::e2D,
			vk::ImageViewType::e2DArray,
			vk::ImageViewType::e3D,
			vk::ImageViewType::eCube,
			vk::ImageViewType::eCubeArray
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_IMAGE_VIEW_TYPES_SHADER_RESOURCE) == static_cast<int32_t>(ViewShaderResourceType::VIEW_SHADER_RESOURCE_COUNT), "Mismatch between abstraction and vulkan image view types");

		vk::ImageViewType ConvertViewShaderResourceTypeToVulkanImageViewType(_In_ const ViewShaderResourceType& InViewShaderResourceViewType)
		{
			return VULKAN_IMAGE_VIEW_TYPES_SHADER_RESOURCE[static_cast<int32_t>(InViewShaderResourceViewType)];
		}

		VulkanView::VulkanView(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(InViewCreateInformation.Context.GetDevice()).GetVulkanDevice();
			VulkanResource& VkResource = *static_cast<VulkanResource*>(InViewCreateInformation.GraphicsResource);

			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);

			_SubresourceRange = vk::ImageSubresourceRange(
				vk::ImageAspectFlagBits::eColor
			);
			_SubresourceRange.levelCount = 1;
			_SubresourceRange.layerCount = 1;

			switch (InViewCreateInformation.ResourceViewRenderTargetType)
			{
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture1D.MipSlice;
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_1D_ARRAY:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.MipSlice;
				_SubresourceRange.baseArrayLayer	= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.FirstArraySlice;
				_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.RenderTargetViewTexture1DArray.ArraySize;
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture2D.MipSlice;
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D_ARRAY:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.MipSlice;
				_SubresourceRange.baseArrayLayer	= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.FirstArraySlice;
				_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.RenderTargetViewTexture2DArray.ArraySize;
			} break;
			case ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_3D:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.RenderTargetViewTexture3D.MipSlice;
			} break;
			}

			vk::ImageViewCreateInfo CreateInfo;
			CreateInfo.flags			= vk::ImageViewCreateFlagBits();
			CreateInfo.image			= VkResource.GetVulkanImage();
			CreateInfo.viewType			= ConvertViewRenderTargetTypeToVulkanImageViewType(InViewCreateInformation.ResourceViewRenderTargetType);
			CreateInfo.format			= ConvertFormatToVulkanFormat(InViewCreateInformation.GraphicsFormat).Format;
			CreateInfo.components		= vk::ComponentMapping();
			CreateInfo.subresourceRange	= _SubresourceRange;
			
			VerifySuccess(
				VkDevice.createImageView(&CreateInfo, nullptr, &_VulkanViewMetaData.ImageView)
			);
		}

		VulkanView::VulkanView(_In_ const ConstantBufferViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(InViewCreateInformation.Context.GetDevice()).GetVulkanDevice();
			VulkanResource& VkResource = static_cast<VulkanResource&>(GetResource());

			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);

			// For buffer
			//vk::BufferViewCreateInfo ConstanBufferViewCreateInfo(
			//	vk::BufferViewCreateFlags(),
			//	VkResource.GetVulkanBuffer(),
			//	VULKAN_FORMATS[static_cast<int32_t>(InViewCreateInformation.GraphicsFormat)].Format,
			//	InViewCreateInformation.MetaData.ConstantBufferView.BufferOffset,
			//	InViewCreateInformation.MetaData.ConstantBufferView.BufferSize
			//);

			//VerifySuccess(
			//	VkDevice.createBufferView(
			//		&ConstanBufferViewCreateInfo,
			//		nullptr,
			//		&_VulkanViewMetaData.BufferView
			//	)
			//);
		}
		
		VulkanView::VulkanView(_In_ const ShaderResourceViewCreateInformation& InViewCreateInformation)
			: View(InViewCreateInformation)
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(InViewCreateInformation.Context.GetDevice()).GetVulkanDevice();
			VulkanResource& VkResource = static_cast<VulkanResource&>(GetResource());

			_SubresourceRange = vk::ImageSubresourceRange(
				vk::ImageAspectFlagBits::eColor
			);
			_SubresourceRange.layerCount = 1;

			switch (InViewCreateInformation.ResourceViewShaderResourceType)
			{
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_BUFFER:
			{
				ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
				vk::BufferViewCreateInfo BufferViewCreateInfo(
					vk::BufferViewCreateFlags(),
					VkResource.GetVulkanBuffer(),
					ConvertFormatToVulkanFormat(InViewCreateInformation.GraphicsFormat).Format,
					InViewCreateInformation.MetaData.ShaderResourceViewBuffer.FirstElement,
					InViewCreateInformation.MetaData.ShaderResourceViewBuffer.NumElements * InViewCreateInformation.MetaData.ShaderResourceViewBuffer.StructureByteStride
				);
				VerifySuccess(
					VkDevice.createBufferView(&BufferViewCreateInfo, nullptr, &_VulkanViewMetaData.BufferView)
				);
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_1D:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.ShaderResourceViewTexture1D.MostDetailedMip;
				_SubresourceRange.levelCount		= InViewCreateInformation.MetaData.ShaderResourceViewTexture1D.MipLevels;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_1D_ARRAY:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.ShaderResourceViewTexture1DArray.MostDetailedMip;
				_SubresourceRange.levelCount		= InViewCreateInformation.MetaData.ShaderResourceViewTexture1DArray.MipLevels;
				_SubresourceRange.baseArrayLayer	= InViewCreateInformation.MetaData.ShaderResourceViewTexture1DArray.FirstArraySlice;
				_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.ShaderResourceViewTexture1DArray.ArraySize;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_2D:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.ShaderResourceViewTexture2D.MostDetailedMip;
				_SubresourceRange.levelCount		= InViewCreateInformation.MetaData.ShaderResourceViewTexture2D.MipLevels;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_2D_ARRAY:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.ShaderResourceViewTexture2DArray.MostDetailedMip;
				_SubresourceRange.levelCount		= InViewCreateInformation.MetaData.ShaderResourceViewTexture2DArray.MipLevels;
				_SubresourceRange.baseArrayLayer	= InViewCreateInformation.MetaData.ShaderResourceViewTexture2DArray.FirstArraySlice;
				_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.ShaderResourceViewTexture2DArray.ArraySize;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_3D:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.ShaderResourceViewTexture3D.MostDetailedMip;
				_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.ShaderResourceViewTexture3D.MipLevels;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_CUBE:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.ShaderResourceViewTextureCube.MostDetailedMip;
				_SubresourceRange.levelCount		= InViewCreateInformation.MetaData.ShaderResourceViewTextureCube.MipLevels;
				_SubresourceRange.baseArrayLayer	= 0;
				_SubresourceRange.layerCount		= 6;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_CUBE_ARRAY:
			{
				_SubresourceRange.baseMipLevel		= InViewCreateInformation.MetaData.ShaderResourceViewTextureCubeArray.MostDetailedMip;
				_SubresourceRange.levelCount		= InViewCreateInformation.MetaData.ShaderResourceViewTextureCubeArray.MipLevels;
				_SubresourceRange.baseArrayLayer	= InViewCreateInformation.MetaData.ShaderResourceViewTextureCubeArray.First2DArrayFace;
				_SubresourceRange.layerCount		= InViewCreateInformation.MetaData.ShaderResourceViewTextureCubeArray.NumCubes * 6;
			} break;
			case ViewShaderResourceType::VIEW_SHADER_RESOURCE_UNKNOWN:
			default:
				ETERNAL_BREAK();
				break;
			}

			vk::ImageViewCreateInfo CreateInfo;
			CreateInfo.flags			= vk::ImageViewCreateFlagBits();
			CreateInfo.image			= VkResource.GetVulkanImage();
			CreateInfo.viewType			= ConvertViewShaderResourceTypeToVulkanImageViewType(InViewCreateInformation.ResourceViewShaderResourceType);
			CreateInfo.format			= ConvertFormatToVulkanFormat(InViewCreateInformation.GraphicsFormat).Format;
			CreateInfo.components		= vk::ComponentMapping();
			CreateInfo.subresourceRange	= _SubresourceRange;

			VerifySuccess(
				VkDevice.createImageView(&CreateInfo, nullptr, &_VulkanViewMetaData.ImageView)
			);
		}

		VulkanView::~VulkanView()
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetViewCreateInformation().Context.GetDevice()).GetVulkanDevice();

			switch (GetResourceType())
			{
			case ResourceType::RESOURCE_TYPE_BUFFER:
			{
				if (GetViewCreateInformation().ResourceViewType != ViewType::VIEW_CONSTANT_BUFFER)
					VkDevice.destroyBufferView(_VulkanViewMetaData.BufferView);
			} break;
			case ResourceType::RESOURCE_TYPE_TEXTURE:
			{
				VkDevice.destroyImageView(_VulkanViewMetaData.ImageView);
			} break;

			default:
				ETERNAL_BREAK(); // Invalid resource type
			}
		}

		const vk::ImageView& VulkanView::GetVulkanImageView() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _VulkanViewMetaData.ImageView;
		}

		const vk::BufferView& VulkanView::GetVulkanBufferView() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _VulkanViewMetaData.BufferView;
		}

		vk::ImageView& VulkanView::GetVulkanImageView()
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _VulkanViewMetaData.ImageView;
		}

		vk::BufferView& VulkanView::GetVulkanBufferView()
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _VulkanViewMetaData.BufferView;
		}
	}
}
