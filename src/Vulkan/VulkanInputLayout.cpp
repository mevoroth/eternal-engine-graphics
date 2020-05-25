#include "Vulkan/VulkanInputLayout.hpp"

#include <cstdint>
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Graphics/InputLayout.hpp"
#include "Vulkan/VulkanFormat.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const VulkanInputLayout VulkanEmptyInputLayout(nullptr, 0);

		VulkanInputLayout::VulkanInputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t DataTypeCount)
		{
			if (!DataTypeCount) // No input layout
				return;

			_VertexInputAttributeDescriptions.resize(DataTypeCount);

			uint32_t AttributeOffset = 0u;
			for (uint32_t DataTypeIndex = 0; DataTypeIndex < DataTypeCount; ++DataTypeIndex)
			{
				_VertexInputAttributeDescriptions[DataTypeIndex] = vk::VertexInputAttributeDescription(
					0, DataTypeIndex,
					VULKAN_FORMATS[VERTEX_FORMATS[DataType[DataTypeIndex]]],
					AttributeOffset
				);

				AttributeOffset += VULKAN_FORMAT_SIZES[VERTEX_FORMATS[DataType[DataTypeIndex]]];
			}

			_VertexInputBindingDescriptions.resize(1);
			_VertexInputBindingDescriptions[0] = vk::VertexInputBindingDescription(
				0, AttributeOffset,
				vk::VertexInputRate::eVertex
			);

			LogWrite(LogError, LogGraphics, "[VulkanInputLayout::VulkanInputLayout]Not implemented!");
		}
	}
}
