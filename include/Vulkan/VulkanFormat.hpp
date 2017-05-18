#ifndef _VULKAN_FORMAT_HPP_
#define _VULKAN_FORMAT_HPP_

#include "Macros/Macros.hpp"
#include <cstdint>
#include <vulkan/vulkan.h>
#include "Graphics/Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const VkFormat VULKAN_FORMATS[] =
		{
			VK_FORMAT_B8G8R8A8_UNORM,
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			VK_FORMAT_R32_SFLOAT,
			VK_FORMAT_R32G32B32_SFLOAT, // FIX THIS
			VK_FORMAT_R32G32_SFLOAT,
			VK_FORMAT_D32_SFLOAT
		};

		const uint32_t VULKAN_FORMAT_SIZES[] =
		{
			4,
			4,
			16,
			4,
			12,
			8,
			4
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_FORMATS) == FORMAT_COUNT, "Vulkan Formats declaration not complete");
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_FORMAT_SIZES) == FORMAT_COUNT, "Vulkan Format Sizes declaration not complete");
	}
}

#endif
