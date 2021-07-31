#include "Vulkan/VulkanResource.hpp"

#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanFormat.hpp"
#include "Math/Math.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::Vulkan;
		using namespace Eternal::Math;

		VulkanResource::VulkanResource(_In_ const VulkanResourceBackBufferCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_BACK_BUFFER)
			, _VulkanResourceMetaData(InResourceCreateInformation.BackBufferResource)
		{
		}

		VulkanResource::VulkanResource(_In_ const TextureResourceCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_TEXTURE)
		{
			VulkanDevice& InVkDevice = static_cast<VulkanDevice&>(InResourceCreateInformation.GfxDevice);
			vk::Device& InVulkanDevice = InVkDevice.GetVulkanDevice();

			ETERNAL_ASSERT(
				InResourceCreateInformation.TextureInformation.Dimension >= ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D &&
				InResourceCreateInformation.TextureInformation.Dimension <= ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY
			);

			QueueFamilyIndicesType QueueFamilies;
			InVkDevice.GetQueueFamilyIndices(QueueFamilies);

			vk::ImageCreateInfo TextureCreateInfo(
				ConvertResourceDimensionToVulkanImageCreateFlags(InResourceCreateInformation.TextureInformation.Dimension),
				ConvertResourceDimensionToVulkanImageType(InResourceCreateInformation.TextureInformation.Dimension),
				ConvertFormatToVulkanFormat(InResourceCreateInformation.TextureInformation.ResourceFormat).Format,
				vk::Extent3D(
					InResourceCreateInformation.TextureInformation.Width,
					InResourceCreateInformation.TextureInformation.Height,
					InResourceCreateInformation.TextureInformation.Dimension == ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D ? InResourceCreateInformation.TextureInformation.DepthOrArraySize : 1
				),
				InResourceCreateInformation.TextureInformation.MIPLevels,
				InResourceCreateInformation.TextureInformation.Dimension == ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D ? 1 : InResourceCreateInformation.TextureInformation.DepthOrArraySize,
				vk::SampleCountFlagBits::e1,
				vk::ImageTiling::eOptimal,
				ConvertTextureResourceUsageToVulkanImageUsageFlags(InResourceCreateInformation.TextureInformation.Usage),
				vk::SharingMode::eExclusive,
				static_cast<uint32_t>(QueueFamilies.size()),
				QueueFamilies.data()
			);

			VerifySuccess(
				InVulkanDevice.createImage(
					&TextureCreateInfo,
					nullptr,
					&_VulkanResourceMetaData.ImageResource
				)
			);

			vk::MemoryRequirements ImageMemoryRequirements = InVulkanDevice.getImageMemoryRequirements(
				_VulkanResourceMetaData.ImageResource
			);
			vk::MemoryAllocateInfo ImageMemoryAllocationInfo(
				ImageMemoryRequirements.size,
				InVkDevice.FindBestMemoryTypeIndex(
					ImageMemoryRequirements.memoryTypeBits,
					ConvertResourceMemoryTypeToMemoryPropertyFlagBits(InResourceCreateInformation.MemoryType)
				)
			);
			VerifySuccess(
				InVulkanDevice.allocateMemory(
					&ImageMemoryAllocationInfo,
					nullptr,
					&_ResourceMemory
				)
			);
			InVulkanDevice.bindImageMemory(
				_VulkanResourceMetaData.ImageResource,
				_ResourceMemory,
				0
			);
		}

		VulkanResource::VulkanResource(_In_ const BufferResourceCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_BUFFER)
		{
			VulkanDevice& InVkDevice = static_cast<VulkanDevice&>(InResourceCreateInformation.GfxDevice);
			vk::Device& InVulkanDevice = InVkDevice.GetVulkanDevice();
			QueueFamilyIndicesType QueueFamilies;
			InVkDevice.GetQueueFamilyIndices(QueueFamilies);

			vk::BufferCreateInfo BufferCreateInformation(
				vk::BufferCreateFlagBits(),
				InResourceCreateInformation.BufferInformation.ElementCount * InResourceCreateInformation.BufferInformation.Stride,
				ConvertBufferResourceUsageToVulkanBufferUsageFlags(InResourceCreateInformation.BufferInformation.Usage),
				vk::SharingMode::eExclusive,
				static_cast<uint32_t>(QueueFamilies.size()),
				QueueFamilies.data()
			);

			VerifySuccess(
				InVulkanDevice.createBuffer(
					&BufferCreateInformation,
					nullptr,
					&_VulkanResourceMetaData.BufferResource
				)
			);

			vk::MemoryRequirements BufferMemoryRequirements = InVulkanDevice.getBufferMemoryRequirements(
				_VulkanResourceMetaData.BufferResource
			);
			vk::MemoryAllocateInfo BufferMemoryAllocationInfo(
				BufferMemoryRequirements.size,
				InVkDevice.FindBestMemoryTypeIndex(
					BufferMemoryRequirements.memoryTypeBits,
					ConvertResourceMemoryTypeToMemoryPropertyFlagBits(InResourceCreateInformation.MemoryType)
				)
			);
			VerifySuccess(
				InVulkanDevice.allocateMemory(
					&BufferMemoryAllocationInfo,
					nullptr,
					&_ResourceMemory
				)
			);
			InVulkanDevice.bindBufferMemory(
				_VulkanResourceMetaData.BufferResource,
				_ResourceMemory,
				0
			);
		}

		VulkanResource::~VulkanResource()
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(GetResourceCreateInformation().GfxDevice).GetVulkanDevice();
			VkDevice.freeMemory(_ResourceMemory);
			if (GetRawResourceType() != ResourceType::RESOURCE_TYPE_BACK_BUFFER)
			{
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
		}

		void* VulkanResource::Map(_In_ const MapRange& InMapRange)
		{
			VulkanDevice& GfxDevice = static_cast<VulkanDevice&>(GetResourceCreateInformation().GfxDevice);
			vk::Device& VkDevice = GfxDevice.GetVulkanDevice();
			
			void* Outdata = nullptr;
			
			VerifySuccess(
				VkDevice.mapMemory(
					_ResourceMemory,
					InMapRange.MapOffset,
					Align<uint32_t>(InMapRange.MapSize, GfxDevice.GetNonCoherentMemoryAtomicSize()),
					vk::MemoryMapFlags(),
					&Outdata
				)
			);

			vk::MappedMemoryRange InvalidMappedMemoryRange(
				_ResourceMemory,
				InMapRange.MapOffset,
				Align<uint32_t>(InMapRange.MapSize, GfxDevice.GetNonCoherentMemoryAtomicSize())
			);
			VerifySuccess(
				VkDevice.invalidateMappedMemoryRanges(
					1, &InvalidMappedMemoryRange
				)
			);

			return Outdata;
		}

		void VulkanResource::Unmap(_In_ const MapRange& InMapRange)
		{
			VulkanDevice& GfxDevice = static_cast<VulkanDevice&>(GetResourceCreateInformation().GfxDevice);
			vk::Device& VkDevice = GfxDevice.GetVulkanDevice();

			vk::MappedMemoryRange MappedMemoryRangeToFlush(
				_ResourceMemory,
				InMapRange.MapOffset,
				Align<uint32_t>(InMapRange.MapSize, GfxDevice.GetNonCoherentMemoryAtomicSize())
			);
			VerifySuccess(
				VkDevice.flushMappedMemoryRanges(
					1, &MappedMemoryRangeToFlush
				)
			);
			VkDevice.unmapMemory(
				_ResourceMemory
			);
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
