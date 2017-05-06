#ifndef _VULKAN_INPUT_LAYOUT_HPP_
#define _VULKAN_INPUT_LAYOUT_HPP_

#include <vector>
#include <cstdint>
#include "Graphics/InputLayout.hpp"

struct VkVertexInputAttributeDescription;
struct VkVertexInputBindingDescription;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class VulkanInputLayout : public InputLayout
		{
		public:
			VulkanInputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t DataTypeCount);

			const vector<VkVertexInputAttributeDescription>& GetVertexInputAttributeDescriptions()
			{
				return _VertexInputAttributeDescriptions;
			}

			const vector<VkVertexInputBindingDescription>& GetVertexInputBindingDescriptions()
			{
				return _VertexInputBindingDescriptions;
			}

		private:
			vector<VkVertexInputAttributeDescription>	_VertexInputAttributeDescriptions;
			vector<VkVertexInputBindingDescription>		_VertexInputBindingDescriptions;
		};
	}
}

#endif
