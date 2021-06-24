#pragma once

#include "Graphics/View.hpp"
#include <vulkan/vulkan.hpp>

namespace Eternal
{
	namespace Graphics
	{
		extern const vk::ImageViewType VULKAN_IMAGE_VIEW_TYPES[];
		
		class VulkanView : public View
		{
		public:
			VulkanView(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation);
			~VulkanView();

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

			VulkanViewMetaData _VulkanViewMetaData;
		};
	}
}
