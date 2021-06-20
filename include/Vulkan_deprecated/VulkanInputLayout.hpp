#ifndef _VULKAN_INPUT_LAYOUT_HPP_
#define _VULKAN_INPUT_LAYOUT_HPP_

#include <vulkan/vulkan.hpp>
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

			const vector<vk::VertexInputAttributeDescription>& GetVertexInputAttributeDescriptions()
			{
				return _VertexInputAttributeDescriptions;
			}

			const vector<vk::VertexInputBindingDescription>& GetVertexInputBindingDescriptions()
			{
				return _VertexInputBindingDescriptions;
			}

		private:
			vector<vk::VertexInputAttributeDescription>	_VertexInputAttributeDescriptions;
			vector<vk::VertexInputBindingDescription>		_VertexInputBindingDescriptions;
		};

		extern const VulkanInputLayout VulkanEmptyInputLayout;
	}
}

#endif
