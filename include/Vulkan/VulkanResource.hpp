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
				: ResourceCreateInformation(InDevice, InName, TransitionState::TRANSITION_PRESENT)
				, BackBufferResource(InBackBufferResource)
			{
			}
			vk::Image BackBufferResource;
		};

		class VulkanResource : public Resource
		{
		public:
			VulkanResource(_In_ const VulkanResourceBackBufferCreateInformation& InResourceCreateInformation);
			virtual ~VulkanResource();

			inline VulkanResourceType GetVulkanResourceType() const { return _VulkanResourceType; }
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

				vk::Image ImageResource;
				vk::Buffer BufferResource;
			};
			VulkanResourceType		_VulkanResourceType = VulkanResourceType::BUFFER;
			VulkanResourceMetaData	_VulkanResourceMetaData;
		};
	}
}
