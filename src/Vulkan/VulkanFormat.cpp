#include "Vulkan/VulkanFormat.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static const VulkanFormat VULKAN_FORMATS[] =
		{
			VulkanFormat(vk::Format::eUndefined,			0),
			VulkanFormat(vk::Format::eB8G8R8A8Unorm,		4),
			VulkanFormat(vk::Format::eR8G8B8A8Unorm,		4),
			VulkanFormat(vk::Format::eR32G32B32A32Sfloat,	16),
			VulkanFormat(vk::Format::eR32Sfloat,			4),
			VulkanFormat(vk::Format::eR32G32B32Sfloat,		12), // FIX THIS
			VulkanFormat(vk::Format::eR32G32Sfloat,			8),
			VulkanFormat(vk::Format::eD32Sfloat,			4),
			VulkanFormat(vk::Format::eR16Uint,				2)
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_FORMATS) == static_cast<int32_t>(Format::FORMAT_COUNT), "Vulkan Formats declaration not complete");

		const VulkanFormat& ConvertFormatToVulkanFormat(_In_ const Format& InFormat)
		{
			return VULKAN_FORMATS[static_cast<int32_t>(InFormat)];
		}
	}
}
