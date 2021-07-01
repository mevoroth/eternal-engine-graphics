#pragma once

#include "Graphics/CommandUtils.h"
#include "Graphics/Format.hpp"
#include "Graphics/CommandUtils.h"
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		enum class ResourceMemoryType
		{
			RESOURCE_MEMORY_TYPE_GPU_MEMORY,
			RESOURCE_MEMORY_TYPE_GPU_UPLOAD,
			RESOURCE_MEMORY_TYPE_GPU_READBACK,
			RESOURCE_MEMORY_TYPE_COUNT,
			RESOURCE_MEMORY_TYPE_INVALID = RESOURCE_MEMORY_TYPE_COUNT
		};

		enum class ResourceUsage
		{
			RESOURCE_USAGE_NONE							= 0x0,
			RESOURCE_USAGE_RENDER_TARGET				= 0x1,
			RESOURCE_USAGE_DEPTH_STENCIL				= 0x2,
			RESOURCE_USAGE_UNORDERED_ACCESS				= 0x4,
			RESOURCE_USAGE_SHADER_RESOURCE				= 0x8,
			RESOURCE_USAGE_RENDER_TARGET_DEPTH_STENCIL	= RESOURCE_USAGE_RENDER_TARGET | RESOURCE_USAGE_DEPTH_STENCIL
		};

		enum class ResourceDimension
		{
			RESOURCE_DIMENSION_UNKNOWN,
			RESOURCE_DIMENSION_BUFFER,
			RESOURCE_DIMENSION_TEXTURE_1D,
			RESOURCE_DIMENSION_TEXTURE_1D_ARRAY,
			RESOURCE_DIMENSION_TEXTURE_2D,
			RESOURCE_DIMENSION_TEXTURE_2D_ARRAY,
			RESOURCE_DIMENSION_TEXTURE_3D,
			RESOURCE_DIMENSION_TEXTURE_CUBE,
			RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY,
			RESOURCE_DIMENSION_COUNT
		};

		enum class ResourceType
		{
			RESOURCE_TYPE_UNKNOWN		= 0x0,
			RESOURCE_TYPE_BUFFER		= 0x1,
			RESOURCE_TYPE_TEXTURE		= 0x2,
			RESOURCE_TYPE_BACK_BUFFER	= 0x4 | RESOURCE_TYPE_TEXTURE
		};
		
		struct MapRange
		{
			MapRange(_In_ uint32_t InMapSize, _In_ uint32_t InMapOffset = 0, _In_ uint32_t InMIPIndex = 0, _In_ uint32_t InPlaneSlice = 0, _In_ uint32_t InArraySlice = 0)
				: MapSize(InMapSize)
				, MapOffset(InMapOffset)
				, MIPIndex(InMIPIndex)
				, PlaneSlice(InPlaneSlice)
				, ArraySlice(InArraySlice)
			{
			}
			
			uint32_t MapSize	= 0;
			uint32_t MapOffset	= 0;
			uint32_t MIPIndex	= 0;
			uint32_t PlaneSlice	= 0;
			uint32_t ArraySlice	= 0;
		};

		// TODO: Add multisample resource
		struct TextureCreateInformation
		{
			static constexpr uint32_t ComponentCount					= 4;
			static constexpr float DefaultClearValue[ComponentCount]	= { 0.0f };

			TextureCreateInformation(
				_In_ const ResourceDimension& InResourceDimension,
				_In_ const Format& InFormat,
				_In_ const ResourceUsage& InResourceUsage,
				_In_ uint32_t InWidth,
				_In_ uint32_t InHeight = 1,
				_In_ uint32_t InDepthOrArraySize = 1,
				_In_ uint32_t InMIPLevels = 1,
				_In_ const float InClearValue[ComponentCount] = DefaultClearValue
			)
				: Dimension(InResourceDimension)
				, ResourceFormat(InFormat)
				, Usage(InResourceUsage)
				, Width(InWidth)
				, Height(InHeight)
				, DepthOrArraySize(InDepthOrArraySize)
				, MIPLevels(InMIPLevels)
			{
				ETERNAL_ASSERT(InResourceDimension != ResourceDimension::RESOURCE_DIMENSION_UNKNOWN);
				ETERNAL_ASSERT(InResourceDimension != ResourceDimension::RESOURCE_DIMENSION_BUFFER);

				for (int32_t ValueIndex = 0; ValueIndex < ComponentCount; ++ValueIndex)
					ClearValue[ValueIndex] = InClearValue[ValueIndex];
			}

			float ClearValue[ComponentCount]	= { 0.0f };
			ResourceDimension Dimension			= ResourceDimension::RESOURCE_DIMENSION_UNKNOWN;
			Format ResourceFormat				= Format::FORMAT_INVALID;
			ResourceUsage Usage					= ResourceUsage::RESOURCE_USAGE_NONE;
			uint32_t Width						= 1;
			uint32_t Height						= 1;
			uint32_t DepthOrArraySize			= 1;
			uint32_t MIPLevels					= 1;
		};
		
		struct BufferCreateInformation
		{
			BufferCreateInformation(
				_In_ const Format& InFormat,
				_In_ const ResourceUsage& InResourceUsage,
				_In_ uint32_t InSize
			)
				: ResourceFormat(InFormat)
				, Usage(InResourceUsage)
				, Size(InSize)
			{
				ETERNAL_ASSERT((InResourceUsage & ResourceUsage::RESOURCE_USAGE_RENDER_TARGET_DEPTH_STENCIL) == ResourceUsage::RESOURCE_USAGE_NONE);
			}

			Format ResourceFormat	= Format::FORMAT_INVALID;
			ResourceUsage Usage		= ResourceUsage::RESOURCE_USAGE_NONE;
			uint32_t Size			= 1;
		};

		struct ResourceCreateInformation
		{
			ResourceCreateInformation(_In_ Device& InDevice, _In_  const std::string& InName, _In_ const ResourceMemoryType& InMemoryType, _In_ const TransitionState& InInitialTransitionState)
				: GfxDevice(InDevice)
				, Name(InName)
				, ResourceState(InInitialTransitionState)
				, MemoryType(InMemoryType)
			{
			}

			ResourceCreateInformation(
				_In_ Device& InDevice,
				_In_ const std::string& InName,
				_In_ const TextureCreateInformation& InTextureCreateInformation,
				_In_ const ResourceMemoryType& InMemoryType,
				_In_ const TransitionState& InInitialTransitionState
			)
				: GfxDevice(InDevice)
				, Name(InName)
				, ResourceState(InInitialTransitionState)
				, MemoryType(InMemoryType)
				, TextureInformation(InTextureCreateInformation)
			{
			}

			ResourceCreateInformation(
				_In_ Device& InDevice,
				_In_ const std::string& InName,
				_In_ const BufferCreateInformation& InBufferCreateInformation,
				_In_ const ResourceMemoryType& InMemoryType,
				_In_ const TransitionState& InInitialTransitionState
			)
				: GfxDevice(InDevice)
				, Name(InName)
				, ResourceState(InInitialTransitionState)
				, MemoryType(InMemoryType)
				, BufferInformation(InBufferCreateInformation)
			{
			}

			Device&				GfxDevice;
			std::string			Name;
			TransitionState		ResourceState	= TransitionState::TRANSITION_UNDEFINED;
			ResourceMemoryType	MemoryType		= ResourceMemoryType::RESOURCE_MEMORY_TYPE_INVALID;
			union
			{
				TextureCreateInformation	TextureInformation;
				BufferCreateInformation		BufferInformation;
			};
		};

		struct TextureResourceCreateInformation : public ResourceCreateInformation
		{
			TextureResourceCreateInformation(
				_In_ Device& InDevice,
				_In_  const std::string& InName,
				_In_ const TextureCreateInformation& InTextureCreateInformation,
				_In_ const ResourceMemoryType& InMemoryType,
				_In_ const TransitionState& InInitialTransitionState = TransitionState::TRANSITION_UNDEFINED
			)
				: ResourceCreateInformation(InDevice, InName, InTextureCreateInformation, InMemoryType, InInitialTransitionState)
			{
			}
		};

		struct BufferResourceCreateInformation : public ResourceCreateInformation
		{
			BufferResourceCreateInformation(
				_In_ Device& InDevice,
				_In_  const std::string& InName,
				_In_ const BufferCreateInformation& InBufferCreateInformation,
				_In_ const ResourceMemoryType& InMemoryType,
				_In_ const TransitionState& InInitialTransitionState = TransitionState::TRANSITION_UNDEFINED
			)
				: ResourceCreateInformation(InDevice, InName, InBufferCreateInformation, InMemoryType, InInitialTransitionState)
			{
			}
		};

		class Resource
		{
		public:
			Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceType& InResourceType);
			virtual ~Resource() {}

			virtual void* Map(_In_ const MapRange& InMapRange) = 0;
			virtual void Unmap(_In_ const MapRange& InMapRange) = 0;

			inline void SetResourceState(_In_ const TransitionState& InTransitionState) { _ResourceCreateInformation.ResourceState = InTransitionState; }
			inline bool IsMultisample() const { return _Multisample; }
			inline const TransitionState& GetResourceState() const { return _ResourceCreateInformation.ResourceState; }
			const ResourceDimension& GetResourceDimension() const;
			uint32_t GetMIPLevels() const;
			uint32_t GetArraySize() const;
			ResourceType GetResourceType() const;
			const float* GetClearValue() const;
			const Format& GetFormat() const;

		protected:
			inline ResourceCreateInformation& GetResourceCreateInformation() { return _ResourceCreateInformation; }

		private:
			ResourceType				_ResourceType = ResourceType::RESOURCE_TYPE_UNKNOWN; // Used to track down type of resource
			ResourceCreateInformation	_ResourceCreateInformation;
			bool						_Multisample = false;
		};
	}
}
