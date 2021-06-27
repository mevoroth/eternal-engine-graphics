#include "Vulkan/VulkanResource.hpp"

#include "Vulkan/VulkanDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanResource::VulkanResource(_In_ const VulkanResourceBackBufferCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceSource::BACK_BUFFER)
			, _VulkanResourceType(VulkanResourceType::IMAGE)
			, _VulkanResourceMetaData(InResourceCreateInformation.BackBufferResource)
		{
		}

		VulkanResource::~VulkanResource()
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetResourceCreateInformation().GfxDevice).GetVulkanDevice();
			switch (GetResourceSource())
			{
			case ResourceSource::BACK_BUFFER:
			case ResourceSource::UNKNOWN:
				ETERNAL_ASSERT(_VulkanResourceType == VulkanResourceType::IMAGE);
				VkDevice.destroyImage(_VulkanResourceMetaData.ImageResource);
				break;
			}
		}

		vk::Image& VulkanResource::GetVulkanImage()
		{
			ETERNAL_ASSERT(_VulkanResourceType == VulkanResourceType::IMAGE);
			return _VulkanResourceMetaData.ImageResource;
		}

		vk::Buffer& VulkanResource::GetVulkanBuffer()
		{
			ETERNAL_ASSERT(_VulkanResourceType == VulkanResourceType::BUFFER);
			return _VulkanResourceMetaData.BufferResource;
		}

		const vk::Image& VulkanResource::GetVulkanImage() const
		{
			ETERNAL_ASSERT(_VulkanResourceType == VulkanResourceType::IMAGE);
			return _VulkanResourceMetaData.ImageResource;
		}

		const vk::Buffer& VulkanResource::GetVulkanBuffer() const
		{
			ETERNAL_ASSERT(_VulkanResourceType == VulkanResourceType::BUFFER);
			return _VulkanResourceMetaData.BufferResource;
		}
	}
}
