#include "Vulkan/VulkanResource.hpp"

#include "Vulkan/VulkanDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanResource::VulkanResource(_In_ const VulkanResourceBackBufferCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_BACK_BUFFER)
			, _VulkanResourceMetaData(InResourceCreateInformation.BackBufferResource)
		{
		}

		VulkanResource::VulkanResource(_In_ const TextureResourceCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_TEXTURE)
		{
			//_VulkanResourceMetaData = 
		}

		VulkanResource::VulkanResource(_In_ const BufferResourceCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_BUFFER)
		{
		}

		VulkanResource::~VulkanResource()
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetResourceCreateInformation().GfxDevice).GetVulkanDevice();
			switch (GetResourceType())
			{
			case ResourceType::RESOURCE_TYPE_TEXTURE:
				VkDevice.destroyImage(_VulkanResourceMetaData.ImageResource);
				break;

			case ResourceType::RESOURCE_TYPE_BUFFER:
				VkDevice.destroyBuffer(_VulkanResourceMetaData.BufferResource);
				break;
			}
		}

		vk::Image& VulkanResource::GetVulkanImage()
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _VulkanResourceMetaData.ImageResource;
		}

		vk::Buffer& VulkanResource::GetVulkanBuffer()
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _VulkanResourceMetaData.BufferResource;
		}

		const vk::Image& VulkanResource::GetVulkanImage() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _VulkanResourceMetaData.ImageResource;
		}

		const vk::Buffer& VulkanResource::GetVulkanBuffer() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _VulkanResourceMetaData.BufferResource;
		}
	}
}
