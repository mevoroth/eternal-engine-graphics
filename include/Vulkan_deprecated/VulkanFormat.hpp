#ifndef _VULKAN_FORMAT_HPP_
#define _VULKAN_FORMAT_HPP_

#include <vulkan/vulkan.hpp>
#include "Graphics/Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const vk::Format VULKAN_FORMATS[] =
		{
			vk::Format::eB8G8R8A8Unorm,
			vk::Format::eR8G8B8A8Unorm,
			vk::Format::eR32G32B32A32Sfloat,
			vk::Format::eR32Sfloat,
			vk::Format::eR32G32B32Sfloat, // FIX THIS
			vk::Format::eR32G32Sfloat,
			vk::Format::eD32Sfloat
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

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_FORMATS) == static_cast<int32_t>(Format::FORMAT_COUNT), "Vulkan Formats declaration not complete");
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_FORMAT_SIZES) == static_cast<int32_t>(Format::FORMAT_COUNT), "Vulkan Format Sizes declaration not complete");
	}
}

#endif
