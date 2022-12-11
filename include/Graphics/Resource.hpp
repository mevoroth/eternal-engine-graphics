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
		class Resource;

		enum class ResourceMemoryType
		{
			RESOURCE_MEMORY_TYPE_GPU_MEMORY,
			RESOURCE_MEMORY_TYPE_GPU_UPLOAD,
			RESOURCE_MEMORY_TYPE_GPU_READBACK,
			RESOURCE_MEMORY_TYPE_COUNT,
			RESOURCE_MEMORY_TYPE_INVALID = RESOURCE_MEMORY_TYPE_COUNT
		};

		enum class TextureResourceUsage
		{
			TEXTURE_RESOURCE_USAGE_NONE				= 0x0,
			TEXTURE_RESOURCE_USAGE_RENDER_TARGET	= 0x1,
			TEXTURE_RESOURCE_USAGE_DEPTH_STENCIL	= 0x2,
			TEXTURE_RESOURCE_USAGE_UNORDERED_ACCESS	= 0x4,
			TEXTURE_RESOURCE_USAGE_SHADER_RESOURCE	= 0x8,
			TEXTURE_RESOURCE_USAGE_COPY_READ		= 0x10,
			TEXTURE_RESOURCE_USAGE_COPY_WRITE		= 0x20
		};
		static constexpr TextureResourceUsage TextureResourceUsageRenderTargetDepthStencil = TextureResourceUsage::TEXTURE_RESOURCE_USAGE_RENDER_TARGET | TextureResourceUsage::TEXTURE_RESOURCE_USAGE_DEPTH_STENCIL;

		enum class BufferResourceUsage
		{
			BUFFER_RESOURCE_USAGE_NONE					= 0x0,
			BUFFER_RESOURCE_USAGE_COPY_READ				= 0x1,
			BUFFER_RESOURCE_USAGE_COPY_WRITE			= 0x2,
			BUFFER_RESOURCE_USAGE_STRUCTURED_BUFFER		= 0x4,
			BUFFER_RESOURCE_USAGE_RW_STRUCTURED_BUFFER	= 0x8,
			BUFFER_RESOURCE_USAGE_CONSTANT_BUFFER		= 0x10,
			BUFFER_RESOURCE_USAGE_BUFFER				= 0x20,
			BUFFER_RESOURCE_USAGE_RW_BUFFER				= 0x40,
			BUFFER_RESOURCE_USAGE_INDEX_BUFFER			= 0x80,
			BUFFER_RESOURCE_USAGE_VERTEX_BUFFER			= 0x100,
			BUFFER_RESOURCE_USAGE_INDIRECT_BUFFER		= 0x200
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
			RESOURCE_TYPE_UNKNOWN			= 0x0,
			RESOURCE_TYPE_BUFFER			= 0x1,
			RESOURCE_TYPE_TEXTURE			= 0x2,
			RESOURCE_TYPE_BACK_BUFFER		= 0x4 | RESOURCE_TYPE_TEXTURE,
			RESOURCE_TYPE_CONSTANT_BUFFER	= 0x8 | RESOURCE_TYPE_BUFFER
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

		template<typename ResourceStructureType = void>
		class MapScope
		{
		public:
			MapScope(_In_ Resource& InResource, _In_ const MapRange& InRange)
				: _Resource(InResource)
				, _Range(InRange)
				, _DataPointer(InResource.Map<ResourceStructureType>(InRange))
			{
			}
			~MapScope()
			{
				_Resource.Unmap(_Range);
			}

			ResourceStructureType* GetDataPointer() const { return _DataPointer; }
			ResourceStructureType* operator->() const { return _DataPointer; }

		private:
			Resource&				_Resource;
			const MapRange&			_Range;
			ResourceStructureType*	_DataPointer = nullptr;
		};

		// TODO: Add multisample resource
		struct TextureCreateInformation
		{
			static constexpr uint32_t ComponentCount					= 4;
			static constexpr float DefaultClearValue[ComponentCount]	= { 0.0f };
			static constexpr float DefaultClearValueOne[ComponentCount]	= { 1.0f, 1.0f, 1.0f, 1.0f };
			static constexpr uint8_t DefaultStencilClearValue			= 0x0u;

			TextureCreateInformation(
				_In_ const ResourceDimension& InResourceDimension,
				_In_ const Format& InFormat,
				_In_ const TextureResourceUsage& InResourceUsage,
				_In_ uint32_t InWidth,
				_In_ uint32_t InHeight = 1,
				_In_ uint32_t InDepthOrArraySize = 1,
				_In_ uint32_t InMIPLevels = 1,
				_In_ const float InClearValue[ComponentCount] = DefaultClearValue,
				_In_ uint8_t InStencilClearValue = DefaultStencilClearValue
			)
				: Dimension(InResourceDimension)
				, ResourceFormat(InFormat)
				, Usage(InResourceUsage)
				, Width(InWidth)
				, Height(InHeight)
				, DepthOrArraySize(InDepthOrArraySize)
				, MIPLevels(InMIPLevels)
				, StencilClearValue(InStencilClearValue)
			{
				ETERNAL_ASSERT(InResourceDimension != ResourceDimension::RESOURCE_DIMENSION_UNKNOWN);
				ETERNAL_ASSERT(InResourceDimension != ResourceDimension::RESOURCE_DIMENSION_BUFFER);

				for (int32_t ValueIndex = 0; ValueIndex < ComponentCount; ++ValueIndex)
					ClearValue[ValueIndex] = InClearValue[ValueIndex];
			}

			float ClearValue[ComponentCount]	= { 0.0f };
			uint8_t StencilClearValue			= 0x0u;
			ResourceDimension Dimension			= ResourceDimension::RESOURCE_DIMENSION_UNKNOWN;
			Format ResourceFormat				= Format::FORMAT_INVALID;
			TextureResourceUsage Usage			= TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE;
			uint32_t Width						= 1;
			uint32_t Height						= 1;
			uint32_t DepthOrArraySize			= 1;
			uint32_t MIPLevels					= 1;
		};
		
		struct BufferCreateInformation
		{
			BufferCreateInformation(
				_In_ const Format& InFormat,
				_In_ const BufferResourceUsage& InResourceUsage,
				_In_ uint32_t InStride,
				_In_ uint32_t InElementCount = 1
			)
				: ResourceFormat(InFormat)
				, Usage(InResourceUsage)
				, Stride(InStride)
				, ElementCount(InElementCount)
			{
				if (InFormat == Format::FORMAT_UNKNOWN)
				{
					ETERNAL_ASSERT(InStride > 0);
				}
			}

			Format ResourceFormat		= Format::FORMAT_INVALID;
			BufferResourceUsage Usage	= BufferResourceUsage::BUFFER_RESOURCE_USAGE_NONE;
			uint32_t Stride				= 0;
			uint32_t ElementCount		= 1;
		};

		struct VertexBufferCreateInformation : public BufferCreateInformation
		{
			VertexBufferCreateInformation(
				_In_ uint32_t InStride,
				_In_ uint32_t InElementCount
			)
				: BufferCreateInformation(
					Format::FORMAT_UNKNOWN,
					BufferResourceUsage::BUFFER_RESOURCE_USAGE_VERTEX_BUFFER,
					InStride,
					InElementCount
				)
			{
			}
		};

		struct IndexBufferCreateInformation : public BufferCreateInformation
		{
			IndexBufferCreateInformation(
				_In_ uint32_t InStride,
				_In_ uint32_t InElementCount
			)
				: BufferCreateInformation(
					InStride == sizeof(uint16_t) ? Format::FORMAT_R16_UINT : Format::FORMAT_INVALID,
					BufferResourceUsage::BUFFER_RESOURCE_USAGE_INDEX_BUFFER,
					InStride,
					InElementCount
				)
			{
			}
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
				_In_ const std::string& InName,
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
				_In_ const std::string& InName,
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
			virtual ~Resource();

			template<typename ResourceStructureType = void>
			ResourceStructureType* Map(_In_ const MapRange& InMapRange)
			{
				return static_cast<ResourceStructureType*>(Map(InMapRange));
			}
			virtual void Unmap(_In_ const MapRange& InMapRange) = 0;

			inline void SetResourceState(_In_ const TransitionState& InTransitionState) { _ResourceCreateInformation.ResourceState = InTransitionState; }
			inline bool IsMultisample() const { return _Multisample; }
			inline const TransitionState& GetResourceState() const { return _ResourceCreateInformation.ResourceState; }
			const ResourceDimension& GetResourceDimension() const;
			uint32_t GetWidth() const;
			uint32_t GetHeight() const;
			uint32_t GetDepth() const;
			uint32_t GetMIPLevels() const;
			uint32_t GetArraySize() const;
			uint32_t GetBufferSize() const;
			uint32_t GetBufferStride() const;
			ResourceType GetResourceType() const;
			const ResourceType& GetResourceTypeRaw() const { return _ResourceType; }
			const float* GetClearValue() const;
			uint8_t GetStencilClearValue() const;
			const Format& GetFormat() const;
			const std::string& GetResourceName() const;

		protected:
			Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceType& InResourceType);

			virtual void* Map(_In_ const MapRange& InMapRange) = 0;
			inline ResourceCreateInformation& GetResourceCreateInformation() { return _ResourceCreateInformation; }
			const ResourceType& GetRawResourceType() const { return _ResourceType; }

		private:
			ResourceType				_ResourceType = ResourceType::RESOURCE_TYPE_UNKNOWN; // Used to track down type of resource
			ResourceCreateInformation	_ResourceCreateInformation;
			bool						_Multisample = false;
		};
	}
}
