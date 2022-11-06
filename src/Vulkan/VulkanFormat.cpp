#include "Vulkan/VulkanFormat.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static const VulkanFormat VULKAN_FORMATS[] =
		{
			VulkanFormat(vk::Format::eUndefined,				0),
			VulkanFormat(vk::Format::eB8G8R8A8Unorm,			4),
			VulkanFormat(vk::Format::eR8G8B8A8Unorm,			4),
			VulkanFormat(vk::Format::eR8G8B8A8Snorm,			4),
			VulkanFormat(vk::Format::eB10G11R11UfloatPack32,	4),
			VulkanFormat(vk::Format::eR32G32B32A32Sfloat,		16),
			VulkanFormat(vk::Format::eR32G32B32Sfloat,			12),
			VulkanFormat(vk::Format::eR32Sfloat,				4),
			VulkanFormat(vk::Format::eR32Uint,					4),
			VulkanFormat(vk::Format::eD32Sfloat,				4),
			VulkanFormat(vk::Format::eD32SfloatS8Uint,			16), // FIX THIS
			VulkanFormat(vk::Format::eD32SfloatS8Uint,			16), // FIX THIS
			VulkanFormat(vk::Format::eR32G32Sfloat,				8),
			VulkanFormat(vk::Format::eR16Uint,					2),
			VulkanFormat(vk::Format::eR8Unorm,					1)
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_FORMATS) == static_cast<int32_t>(Format::FORMAT_COUNT), "Vulkan Formats declaration not complete");

		const VulkanFormat& ConvertFormatToVulkanFormat(_In_ const Format& InFormat)
		{
			return VULKAN_FORMATS[static_cast<int32_t>(InFormat)];
		}

		bool IsVulkanDepthStencilFormat(const Format& InFormat)
		{
			switch (InFormat)
			{
			case Format::FORMAT_D32:
			case Format::FORMAT_DS328:
			case Format::FORMAT_RG328:
				return true;
			}
			return false;
		}
	}
}
