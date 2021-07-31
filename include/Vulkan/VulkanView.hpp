#pragma once

#include "Graphics/View.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class VulkanView final : public View
		{
		public:
			VulkanView(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation);
			VulkanView(_In_ const ConstantBufferViewCreateInformation& InViewCreateInformation);
			VulkanView(_In_ const ShaderResourceViewCreateInformation& InViewCreateInformation);
			VulkanView(_In_ const UnorderedAccessViewCreateInformation& InViewCreateInformation);
			VulkanView(_In_ const DepthStencilViewCreateInformation& InViewCreateInformation);
			~VulkanView();

			const vk::ImageSubresourceRange& GetVulkanSubresourceRange() const { return _SubresourceRange; }

			const vk::ImageView& GetVulkanImageView() const;
			const vk::BufferView& GetVulkanBufferView() const;
			vk::ImageView& GetVulkanImageView();
			vk::BufferView& GetVulkanBufferView();

		private:
			union VulkanViewMetaData
			{
				VulkanViewMetaData()
					: ImageView(nullptr)
					, BufferView(nullptr)
				{
				}

				VulkanViewMetaData(vk::ImageView InImageView)
					: ImageView(InImageView)
				{
				}

				VulkanViewMetaData(vk::BufferView InBufferView)
					: BufferView(InBufferView)
				{
				}

				vk::ImageView ImageView;
				vk::BufferView BufferView;
			};

			VulkanViewMetaData			_VulkanViewMetaData;
			vk::ImageSubresourceRange	_SubresourceRange;
		};
	}
}
