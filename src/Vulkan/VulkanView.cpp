#include "Vulkan/VulkanView.hpp"

#include "Graphics/View.hpp"
#include "Graphics/Format.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanFormat.hpp"
#include "Vulkan/VulkanDescriptorHeap.hpp"

using namespace Eternal::Graphics;

const vk::ImageViewType VULKAN_IMAGE_VIEW_TYPES[] =
{
	vk::ImageViewType::e1D,
	vk::ImageViewType::e1DArray,
	vk::ImageViewType::e2D,
	vk::ImageViewType::e2DArray,
	vk::ImageViewType::e3D,
	vk::ImageViewType::eCube,
	vk::ImageViewType::eCubeArray
};

VulkanView::VulkanView(_In_ Device& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj)
	: View(FormatObj)
{
	_ImageView = vk::ImageView();
	//VkImageAspectFlagBits AspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//if (FormatObj == FORMAT_D32)
	//	AspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

	//VkImageViewCreateInfo ImageViewInfo;

	//ImageViewInfo.sType								= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	//ImageViewInfo.pNext								= nullptr;
	//ImageViewInfo.flags								= 0;
	//ImageViewInfo.image								= ResourceObj.GetImage();
	//ImageViewInfo.viewType							= VULKAN_IMAGE_VIEW_TYPES[ViewType];
	//ImageViewInfo.format							= VULKAN_FORMATS[FormatObj];
	//ImageViewInfo.components.r						= VK_COMPONENT_SWIZZLE_R;
	//ImageViewInfo.components.g						= VK_COMPONENT_SWIZZLE_G;
	//ImageViewInfo.components.b						= VK_COMPONENT_SWIZZLE_B;
	//ImageViewInfo.components.a						= VK_COMPONENT_SWIZZLE_A;
	//ImageViewInfo.subresourceRange.aspectMask		= AspectMask;
	//ImageViewInfo.subresourceRange.baseMipLevel		= 0;
	//ImageViewInfo.subresourceRange.levelCount		= 1;
	//ImageViewInfo.subresourceRange.baseArrayLayer	= 0;
	//ImageViewInfo.subresourceRange.layerCount		= 1;

	//VkResult Result = vkCreateImageView(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &ImageViewInfo, nullptr, &_View.ImageView);
	//ETERNAL_ASSERT(!Result);
}

VulkanView::VulkanView(_In_ Device& DeviceObj, _In_ const vk::Image& BackBufferImage, _In_ const TextureView& ViewType, _In_ const Format& FormatObj)
	: View(FormatObj)
{
	using namespace Vulkan;

	vk::ImageViewCreateInfo ImageViewInfo(
		vk::ImageViewCreateFlagBits(),
		BackBufferImage,
		VULKAN_IMAGE_VIEW_TYPES[int(ViewType)],
		VULKAN_FORMATS[int(FormatObj)],
		vk::ComponentMapping(
			vk::ComponentSwizzle::eR,
			vk::ComponentSwizzle::eG,
			vk::ComponentSwizzle::eB,
			vk::ComponentSwizzle::eA
		),
		vk::ImageSubresourceRange(
			vk::ImageAspectFlagBits::eColor,
			0, 1,
			0, 1
		)
	);
	ImageViewInfo.flags = vk::ImageViewCreateFlagBits();

	VerifySuccess(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice().createImageView(&ImageViewInfo, nullptr, &_ImageView));
}

VulkanView::VulkanView(_In_ Device& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const Format& FormatObj, _In_ uint64_t Offset, _In_ uint64_t Size)
	: View(FormatObj)
{
	_BufferView = vk::BufferView();
	//VkBufferViewCreateInfo BufferViewInfo;
	//
	//BufferViewInfo.sType	= VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
	//BufferViewInfo.pNext	= nullptr;
	//BufferViewInfo.flags	= 0;
	//BufferViewInfo.buffer	= ResourceObj.GetBuffer();
	//BufferViewInfo.format	= VULKAN_FORMATS[FormatObj];
	//BufferViewInfo.offset	= Offset;
	//BufferViewInfo.range	= Size;

	//VkResult Result = vkCreateBufferView(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &BufferViewInfo, nullptr, &_View.BufferView);
	//ETERNAL_ASSERT(!Result);
}

VulkanView::~VulkanView()
{
	ETERNAL_ASSERT(_BufferView);
}

//RenderTarget& VulkanView::GetAsRenderTarget()
//{
//	return *((RenderTarget*)nullptr);
//}
