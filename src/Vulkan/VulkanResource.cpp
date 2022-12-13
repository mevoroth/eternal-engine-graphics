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
			switch (GetResourceCreateInformation().ResourceState)
			{
			case TransitionState::TRANSITION_COPY_WRITE:
			case TransitionState::TRANSITION_RENDER_TARGET:
			case TransitionState::TRANSITION_DEPTH_STENCIL_WRITE:
				GetResourceCreateInformation().ResourceState = TransitionState::TRANSITION_UNDEFINED;
				break;
			}

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

			_SetDebugName();

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

			_SetDebugName();

			vk::MemoryRequirements BufferMemoryRequirements = InVulkanDevice.getBufferMemoryRequirements(
				_VulkanResourceMetaData.BufferResource
			);
			vk::MemoryAllocateInfo BufferMemoryAllocationInfo(
				Align<vk::DeviceSize>(BufferMemoryRequirements.size, InVkDevice.GetNonCoherentMemoryAtomicSize()),
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

		uint32_t VulkanResource::GetTextureToBufferMemoryFootprint(_In_ Device& InDevice) const
		{
			const TextureCreateInformation& CreateInformation = GetResourceCreateInformation().TextureInformation;
			return CreateInformation.Width * CreateInformation.Height * CreateInformation.DepthOrArraySize * ConvertFormatToVulkanFormat(CreateInformation.ResourceFormat).Size;
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

		void VulkanResource::_SetDebugName()
		{
			uint64_t ResourceHandle = 0ull;
			vk::ObjectType ObjectType = vk::ObjectType::eUnknown;

			switch (GetResourceType())
			{
			case ResourceType::RESOURCE_TYPE_BUFFER:
			{
				VkBuffer BufferResourceHandle = _VulkanResourceMetaData.BufferResource;
				ResourceHandle = reinterpret_cast<uint64_t>(BufferResourceHandle);
				ObjectType = vk::ObjectType::eBuffer;
			} break;
			case ResourceType::RESOURCE_TYPE_TEXTURE:
			{
				VkImage ImageResourceHandle = _VulkanResourceMetaData.ImageResource;
				ResourceHandle = reinterpret_cast<uint64_t>(ImageResourceHandle);
				ObjectType = vk::ObjectType::eImage;
			} break;
			default:
				ETERNAL_BREAK();
				break;
			}

			vk::DebugUtilsObjectNameInfoEXT ObjectNameInfo(
				ObjectType,
				ResourceHandle,
				GetResourceCreateInformation().Name.c_str()
			);
			VulkanDevice& Device = static_cast<VulkanDevice&>(GetResourceCreateInformation().GfxDevice);
			VerifySuccess(Device.GetVulkanDevice().setDebugUtilsObjectNameEXT(&ObjectNameInfo, Device.GetDispatchLoader()));
		}
	}
}
