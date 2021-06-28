#pragma once

#include "Graphics/Resource.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class VulkanResourceType
		{
			BUFFER,
			IMAGE
		};

		struct VulkanResourceBackBufferCreateInformation : public ResourceCreateInformation
		{
			VulkanResourceBackBufferCreateInformation(_In_ Device& InDevice, _In_ const std::string& InName, _In_ vk::Image InBackBufferResource)
				: ResourceCreateInformation(InDevice, InName, ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_MEMORY, TransitionState::TRANSITION_PRESENT)
				, BackBufferResource(InBackBufferResource)
			{
			}
			vk::Image BackBufferResource;
		};

		class VulkanResource : public Resource
		{
		public:
			/**
			 * Back buffer
			 */
			VulkanResource(_In_ const VulkanResourceBackBufferCreateInformation& InResourceCreateInformation);
			/**
			 * Texture
			 */
			VulkanResource(_In_ const TextureResourceCreateInformation& InResourceCreateInformation);
			/**
			 * Buffer
			 */
			VulkanResource(_In_ const BufferResourceCreateInformation& InResourceCreateInformation);
			virtual ~VulkanResource();

			const vk::Image& GetVulkanImage() const;
			const vk::Buffer& GetVulkanBuffer() const;
			vk::Image& GetVulkanImage();
			vk::Buffer& GetVulkanBuffer();

		private:
			union VulkanResourceMetaData
			{
				VulkanResourceMetaData(vk::Image InImageResource)
					: ImageResource(InImageResource)
				{
				}
				VulkanResourceMetaData(vk::Buffer InBufferResource)
					: BufferResource(InBufferResource)
				{
				}
				VulkanResourceMetaData()
				{
				}

				vk::Image ImageResource;
				vk::Buffer BufferResource;
			};
			VulkanResourceMetaData	_VulkanResourceMetaData;
		};
	}
}
