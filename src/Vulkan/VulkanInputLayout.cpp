#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanInputLayout.hpp"

#include "Graphics/InputLayout.hpp"
#include "Vulkan/VulkanFormat.hpp"
#include "Vulkan/VulkanUtils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static const vk::VertexInputRate ConvertVertexStreamFrequencyToVulkanVertexInputRate(_In_ const VertexStreamFrequency& InVertexStreamFrequency)
		{
			return static_cast<vk::VertexInputRate>(InVertexStreamFrequency);
		}

		VulkanInputLayout::VulkanInputLayout(_In_ const vector<VertexStreamBase>& InVertexStreams)
			: InputLayout(InVertexStreams)
		{
			if (InVertexStreams.size() == 0)
				return;

			uint32_t TotalElementsCount = 0;
			for (uint32_t VertexStreamIndex = 0; VertexStreamIndex < InVertexStreams.size(); ++VertexStreamIndex)
				TotalElementsCount += static_cast<uint32_t>(InVertexStreams[VertexStreamIndex].GetElements().size());

			_VertexInputAttributeDescriptions.reserve(TotalElementsCount);
			_VertexInputBindingDescriptions.resize(InVertexStreams.size());
			for (uint32_t VertexStreamIndex = 0; VertexStreamIndex < InVertexStreams.size(); ++VertexStreamIndex)
			{
				_VertexInputBindingDescriptions[VertexStreamIndex] = vk::VertexInputBindingDescription(
					VertexStreamIndex,
					InVertexStreams[VertexStreamIndex].GetStreamStride(),
					ConvertVertexStreamFrequencyToVulkanVertexInputRate(InVertexStreams[VertexStreamIndex].GetVertexStreamFrequency())
				);

				uint32_t VertexAttributeOffset = 0;
				const vector<VertexStreamElement>& Elements = InVertexStreams[VertexStreamIndex].GetElements();
				for (uint32_t ElementIndex = 0; ElementIndex < Elements.size(); ++ElementIndex)
				{
					_VertexInputAttributeDescriptions.push_back(vk::VertexInputAttributeDescription());
					vk::VertexInputAttributeDescription& CurrentVertexInputAttributeDescription = _VertexInputAttributeDescriptions.back();

					const VulkanFormat& VertexFormat = ConvertFormatToVulkanFormat(Elements[ElementIndex].VertexFormat);

					CurrentVertexInputAttributeDescription = vk::VertexInputAttributeDescription(
						ElementIndex,
						VertexStreamIndex,
						VertexFormat.Format,
						VertexAttributeOffset
					);

					VertexAttributeOffset += VertexFormat.Size;
				}
			}
		}
	}
}

#endif
