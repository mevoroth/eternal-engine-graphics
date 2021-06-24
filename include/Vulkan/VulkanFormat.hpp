#pragma once

#include <vulkan/vulkan.hpp>
#include "Graphics/Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct VulkanFormat
		{
			VulkanFormat(const vk::Format& InFormat, uint32_t InSize)
				: Format(InFormat)
				, Size(InSize)
			{
			}

			vk::Format	Format;
			uint32_t	Size;
		};

		extern const VulkanFormat VULKAN_FORMATS[];
	}
}
