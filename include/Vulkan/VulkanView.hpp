#pragma once

#include "Graphics/View.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class VulkanResourceType;

		extern const vk::ImageViewType VULKAN_IMAGE_VIEW_TYPES[];
		
		class VulkanView : public View
		{
		public:
			VulkanView(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation);
			~VulkanView();

			const vk::ImageSubresourceRange& GetVulkanSubresourceRange() const { return _SubresourceRange; }

			vk::ImageView& GetVulkanImageView();
			vk::BufferView& GetVulkanBufferView();

		private:
			VulkanResourceType GetVulkanResourceType();

			union VulkanViewMetaData
			{
				VulkanViewMetaData()
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
