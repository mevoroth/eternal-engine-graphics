#include "Vulkan/VulkanView.hpp"

#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Graphics/View.hpp"
#include "Graphics/Format.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanFormat.hpp"

using namespace Eternal::Graphics;

const VkImageViewType VULKAN_IMAGE_VIEW_TYPES[] =
{
	VK_IMAGE_VIEW_TYPE_1D,
	VK_IMAGE_VIEW_TYPE_1D_ARRAY,
	VK_IMAGE_VIEW_TYPE_2D,
	VK_IMAGE_VIEW_TYPE_2D_ARRAY,
	VK_IMAGE_VIEW_TYPE_3D,
	VK_IMAGE_VIEW_TYPE_CUBE,
	VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
};
//extern const VkFormat VULKAN_FORMATS[];

VulkanView::VulkanView(_In_ VulkanDevice& DeviceObj, _In_ VulkanResource& ResourceObj, _In_ const TextureView& View, _In_ const Format& FormatObj)
	: _Format(FormatObj)
{
	VkImageViewCreateInfo ImageViewInfo;

	ImageViewInfo.sType								= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ImageViewInfo.pNext								= nullptr;
	ImageViewInfo.flags								= 0;
	ImageViewInfo.image								= ResourceObj.GetImage();
	ImageViewInfo.viewType							= VULKAN_IMAGE_VIEW_TYPES[View];
	ImageViewInfo.format							= VULKAN_FORMATS[FormatObj];
	ImageViewInfo.components.r						= VK_COMPONENT_SWIZZLE_R;
	ImageViewInfo.components.g						= VK_COMPONENT_SWIZZLE_G;
	ImageViewInfo.components.b						= VK_COMPONENT_SWIZZLE_B;
	ImageViewInfo.components.a						= VK_COMPONENT_SWIZZLE_A;
	ImageViewInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
	ImageViewInfo.subresourceRange.baseMipLevel		= 0;
	ImageViewInfo.subresourceRange.levelCount		= 1;
	ImageViewInfo.subresourceRange.baseArrayLayer	= 0;
	ImageViewInfo.subresourceRange.layerCount		= 1;

	VkResult Result = vkCreateImageView(DeviceObj.GetDevice(), &ImageViewInfo, nullptr, &_View.ImageView);
	ETERNAL_ASSERT(!Result);
}

VulkanView::VulkanView(_In_ VulkanDevice& DeviceObj, _In_ VkImage_T*& BackBufferImage, _In_ const TextureView& View, _In_ const Format& FormatObj)
	: _Format(FormatObj)
{
	VkImageViewCreateInfo ImageViewInfo;

	ImageViewInfo.sType								= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ImageViewInfo.pNext								= nullptr;
	ImageViewInfo.flags								= 0;
	ImageViewInfo.image								= BackBufferImage;
	ImageViewInfo.viewType							= VULKAN_IMAGE_VIEW_TYPES[View];
	ImageViewInfo.format							= VULKAN_FORMATS[FormatObj];
	ImageViewInfo.components.r						= VK_COMPONENT_SWIZZLE_R;
	ImageViewInfo.components.g						= VK_COMPONENT_SWIZZLE_G;
	ImageViewInfo.components.b						= VK_COMPONENT_SWIZZLE_B;
	ImageViewInfo.components.a						= VK_COMPONENT_SWIZZLE_A;
	ImageViewInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
	ImageViewInfo.subresourceRange.baseMipLevel		= 0;
	ImageViewInfo.subresourceRange.levelCount		= 1;
	ImageViewInfo.subresourceRange.baseArrayLayer	= 0;
	ImageViewInfo.subresourceRange.layerCount		= 1;

	VkResult Result = vkCreateImageView(DeviceObj.GetDevice(), &ImageViewInfo, nullptr, &_View.ImageView);
	ETERNAL_ASSERT(!Result);
}

VulkanView::~VulkanView()
{
	ETERNAL_ASSERT(_View.BufferView);
}
