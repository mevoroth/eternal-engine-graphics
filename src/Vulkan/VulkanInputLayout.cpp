#include "Vulkan/VulkanInputLayout.hpp"

#include <cstdint>
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Graphics/InputLayout.hpp"
#include "Vulkan/VulkanFormat.hpp"

using namespace Eternal::Graphics;

VulkanInputLayout::VulkanInputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t DataTypeCount)
{
	_VertexInputAttributeDescriptions.resize(DataTypeCount);

	uint32_t AttributeOffset = 0u;
	for (uint32_t DataTypeIndex = 0; DataTypeIndex < DataTypeCount; ++DataTypeIndex)
	{
		_VertexInputAttributeDescriptions[DataTypeIndex].binding	= 0;
		_VertexInputAttributeDescriptions[DataTypeIndex].location	= DataTypeIndex;
		_VertexInputAttributeDescriptions[DataTypeIndex].format		= VULKAN_FORMATS[VERTEX_FORMATS[DataType[DataTypeIndex]]];
		_VertexInputAttributeDescriptions[DataTypeIndex].offset		= AttributeOffset;

		AttributeOffset += VULKAN_FORMAT_SIZES[VERTEX_FORMATS[DataType[DataTypeIndex]]];
	}

	_VertexInputBindingDescriptions.resize(1);
	_VertexInputBindingDescriptions[0].binding		= 0;
	_VertexInputBindingDescriptions[0].stride		= AttributeOffset;
	_VertexInputBindingDescriptions[0].inputRate	= VK_VERTEX_INPUT_RATE_VERTEX;
}
