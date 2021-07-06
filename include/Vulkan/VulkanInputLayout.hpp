#pragma once

#include "Graphics/InputLayout.hpp"
#include "Vulkan/VulkanHeader.hpp"

struct VkVertexInputAttributeDescription;
struct VkVertexInputBindingDescription;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class VulkanInputLayout final : public InputLayout
		{
		public:
			VulkanInputLayout(_In_ const vector<VertexStreamBase>& InVertexStreams);

			const vector<vk::VertexInputAttributeDescription>& GetVertexInputAttributeDescriptions() const { return _VertexInputAttributeDescriptions; }
			const vector<vk::VertexInputBindingDescription>& GetVertexInputBindingDescriptions() const { return _VertexInputBindingDescriptions; }

		private:
			vector<vk::VertexInputAttributeDescription>	_VertexInputAttributeDescriptions;
			vector<vk::VertexInputBindingDescription>	_VertexInputBindingDescriptions;
		};

		extern const VulkanInputLayout VulkanEmptyInputLayout;
	}
}
