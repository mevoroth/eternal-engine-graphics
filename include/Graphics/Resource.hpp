#pragma once

#include "Graphics/CommandUtils.hpp"
#include "Graphics/Format.hpp"
#include "Graphics/CommandUtils.hpp"
#include <string>
#include <unordered_set>
#include <vector>

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
			TEXTURE_RESOURCE_USAGE_COPY_WRITE		= 0x20,
			TEXTURE_RESOURCE_USAGE_COUNT			= (TEXTURE_RESOURCE_USAGE_COPY_WRITE << 1)
		};
		static constexpr TextureResourceUsage TextureResourceUsageRenderTargetDepthStencil = TextureResourceUsage::TEXTURE_RESOURCE_USAGE_RENDER_TARGET | TextureResourceUsage::TEXTURE_RESOURCE_USAGE_DEPTH_STENCIL;

		enum class BufferResourceUsage
		{
			BUFFER_RESOURCE_USAGE_NONE										= 0x0,
			BUFFER_RESOURCE_USAGE_COPY_READ									= 0x1,
			BUFFER_RESOURCE_USAGE_COPY_WRITE								= 0x2,
			BUFFER_RESOURCE_USAGE_STRUCTURED_BUFFER							= 0x4,
			BUFFER_RESOURCE_USAGE_RW_STRUCTURED_BUFFER						= 0x8,
			BUFFER_RESOURCE_USAGE_CONSTANT_BUFFER							= 0x10,
			BUFFER_RESOURCE_USAGE_BUFFER									= 0x20,
			BUFFER_RESOURCE_USAGE_RW_BUFFER									= 0x40,
			BUFFER_RESOURCE_USAGE_INDEX_BUFFER								= 0x80,
			BUFFER_RESOURCE_USAGE_VERTEX_BUFFER								= 0x100,
			BUFFER_RESOURCE_USAGE_INDIRECT_BUFFER							= 0x200,
			BUFFER_RESOURCE_USAGE_RAYTRACING_ACCELERATION_STRUCTURE_BUFFER	= 0x400
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

		static constexpr const char* ResourceDimensionString[] =
		{
			"RESOURCE_DIMENSION_UNKNOWN",
			"RESOURCE_DIMENSION_BUFFER",
			"RESOURCE_DIMENSION_TEXTURE_1D",
			"RESOURCE_DIMENSION_TEXTURE_1D_ARRAY",
			"RESOURCE_DIMENSION_TEXTURE_2D",
			"RESOURCE_DIMENSION_TEXTURE_2D_ARRAY",
			"RESOURCE_DIMENSION_TEXTURE_3D",
			"RESOURCE_DIMENSION_TEXTURE_CUBE",
			"RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY"
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(ResourceDimensionString) == static_cast<uint32_t>(ResourceDimension::RESOURCE_DIMENSION_COUNT), "ResourceDimensionString count needs to be equals to ResourceDimension::RESOURCE_DIMENSION_COUNT");

		struct ClearValue
		{
			static constexpr uint32_t ClearValuesPoolSize		= 32u;
			static constexpr uint32_t ComponentCount			= 4;
			static constexpr uint8_t DefaultClearValueStencil	= 0x0u;

			static std::vector<ClearValue> ClearValues;

			static ClearValue DefaultClearValueZero();
			static ClearValue DefaultClearValueOne();
			static ClearValue DefaultClearValueDepth();

			static void InitializeClearValues();
			static uint8_t FindOrAddClearValue(_In_ const ClearValue& InClearValue);

			ClearValue(_In_ float InX, _In_ float InY, _In_ float InZ, _In_ float InW)
			{
				Color[0] = InX;
				Color[1] = InY;
				Color[2] = InZ;
				Color[3] = InW;
			}

			ClearValue(_In_ float InDepth, _In_ uint8_t InStencil = DefaultClearValueStencil)
			{
				DepthStencil.Depth = InDepth;
				DepthStencil.Stencil = InStencil;
			}

			ClearValue()
			{
				for (uint32_t ValueIndex = 0; ValueIndex < ComponentCount; ++ValueIndex)
					RawClearValue[ValueIndex] = 0;
			}

			bool operator==(const ClearValue& Other) const;
			bool operator!=(const ClearValue& Other) const;

			union
			{
				uint32_t RawClearValue[ComponentCount] = {};
				float Color[ComponentCount];
				struct
				{
					float Depth;
					uint8_t Stencil;
				} DepthStencil;
			};
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
			static constexpr uint32_t ResourceDimensionBitCount		= 4u;
			static constexpr uint32_t TextureResourceUsageBitCount	= 6u;

			TextureCreateInformation(
				_In_ const ResourceDimension& InResourceDimension = ResourceDimension::RESOURCE_DIMENSION_UNKNOWN,
				_In_ const Format& InFormat = Format::FORMAT_INVALID,
				_In_ const TextureResourceUsage& InResourceUsage = TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE,
				_In_ uint16_t InWidth = 1,
				_In_ uint16_t InHeight = 1,
				_In_ uint8_t InDepthOrArraySize = 1,
				_In_ uint8_t InMIPLevels = 1,
				_In_ const ClearValue& InClearValue = ClearValue::DefaultClearValueZero()
			)
				: Width(InWidth)
				, Height(InHeight)
				, DepthOrArraySize(InDepthOrArraySize)
				, MIPLevels(InMIPLevels)
				, Dimension(InResourceDimension)
				, ResourceFormat(InFormat)
				, Usage(InResourceUsage)
			{
				if (InResourceDimension != ResourceDimension::RESOURCE_DIMENSION_UNKNOWN ||
					InFormat != Format::FORMAT_INVALID ||
					InResourceUsage != TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE ||
					InWidth != 1 ||
					InHeight != 1 ||
					InDepthOrArraySize != 1 ||
					InMIPLevels != 1 ||
					InClearValue != ClearValue::DefaultClearValueZero())
				{
					ETERNAL_ASSERT(InResourceDimension != ResourceDimension::RESOURCE_DIMENSION_UNKNOWN);
					ETERNAL_ASSERT(InResourceDimension != ResourceDimension::RESOURCE_DIMENSION_BUFFER);
				}

				ClearValueIndex = ClearValue::FindOrAddClearValue(InClearValue);
			}

			uint16_t Width						= 1;
			uint16_t Height						= 1;
			uint8_t DepthOrArraySize			= 1;
			uint8_t MIPLevels					= 1;
			uint8_t ClearValueIndex				= 0;
			ResourceDimension Dimension	: ResourceDimensionBitCount;
			Format ResourceFormat		: FormatBitCount;
			TextureResourceUsage Usage	: TextureResourceUsageBitCount;
		};
		
		struct BufferCreateInformation
		{
			static constexpr uint32_t BufferResourceUsageBitCount	= 10u;
			static constexpr uint32_t MaxBufferStrideBitCount		= 16u;
			static constexpr uint32_t MaxBufferStride				= (1u << MaxBufferStrideBitCount) - 1u;

			ETERNAL_STATIC_ASSERT(sizeof(BufferResourceUsage) <= BufferResourceUsageBitCount, "BufferResourceUsage must fit in 10 bits");

			BufferCreateInformation()
				: ResourceFormat(Format::FORMAT_INVALID)
				, Usage(BufferResourceUsage::BUFFER_RESOURCE_USAGE_NONE)
				, Stride(0u)
				, ElementCount(1u)
			{
			}

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
					ETERNAL_ASSERT(InStride <= MaxBufferStride);
				}
			}

			Format ResourceFormat		: FormatBitCount;
			BufferResourceUsage Usage	: BufferResourceUsageBitCount;
			uint32_t Stride				: MaxBufferStrideBitCount;
			uint32_t ElementCount;
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

		struct AccelerationStructureBufferCreateInformation : public BufferCreateInformation
		{
			AccelerationStructureBufferCreateInformation(
				_In_ uint32_t InSize
			)
				: BufferCreateInformation(
					Format::FORMAT_UNKNOWN,
					BufferResourceUsage::BUFFER_RESOURCE_USAGE_RAYTRACING_ACCELERATION_STRUCTURE_BUFFER,
					1u,
					InSize
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
				TextureInformation	= TextureCreateInformation();
				BufferInformation	= BufferCreateInformation();
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

		struct TextureToBufferMemoryFootprint
		{
			uint32_t RowPitch	= 0u;
			uint32_t TotalBytes	= 0u;
		};

		class Resource
		{
		public:
			static std::unordered_set<Resource*> ResourceHistory;

			virtual ~Resource();

			template<typename ResourceStructureType = void>
			ResourceStructureType* Map(_In_ const MapRange& InMapRange)
			{
				return static_cast<ResourceStructureType*>(Map(InMapRange));
			}
			virtual void Unmap(_In_ const MapRange& InMapRange) = 0;
			virtual TextureToBufferMemoryFootprint GetTextureToBufferMemoryFootprint(_In_ Device& InDevice) const = 0;

			void SetResourceState(_In_ const TransitionState& InTransitionState);
			void SetSubResourceState(_In_ uint32_t InSubResourceIndex, _In_ const TransitionState& InTransitionState);
			inline bool IsMultisample() const { return _Multisample; }
			inline const TransitionState& GetResourceState() const { return _ResourceCreateInformation.ResourceState; }
			const TransitionState& GetResourceState(_In_ uint32_t InSubResourceIndex) const;
			uint32_t GetSubResourceStatesCount() const;
			bool IsSubResourceStatesDirty() const { return _IsSubResourceStatesDirty; }
			void ClearSubResourceStatesDirty() { _IsSubResourceStatesDirty = false; }
			const ResourceDimension& GetResourceDimension() const;
			uint32_t GetWidth() const;
			uint32_t GetHeight() const;
			uint32_t GetDepth() const;
			uint32_t GetMIPLevels() const;
			uint32_t GetArraySize() const;
			uint32_t GetDepthOrArraySize() const;
			uint32_t GetBufferSize() const;
			uint32_t GetBufferStride() const;
			uint32_t GetElementCount() const;
			ResourceType GetResourceType() const;
			const ResourceType& GetResourceTypeRaw() const { return _ResourceType; }
			const ClearValue& GetClearValue() const;
			const Format& GetFormat() const;
			const std::string& GetResourceName() const;
			bool IsBackBuffer() const;
			inline const ResourceCreateInformation& GetResourceCreateInformation() const { return _ResourceCreateInformation; }
			inline ResourceCreateInformation& GetResourceCreateInformation() { return _ResourceCreateInformation; }

		protected:
			Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceType& InResourceType);

			virtual void* Map(_In_ const MapRange& InMapRange) = 0;
			const ResourceType& GetRawResourceType() const { return _ResourceType; }
			std::vector<TransitionState>& GetSubResourceStates() { return _SubResourceStates; }

		private:
			ResourceCreateInformation		_ResourceCreateInformation;
			std::vector<TransitionState>	_SubResourceStates;
			ResourceType					_ResourceType				= ResourceType::RESOURCE_TYPE_UNKNOWN; // Used to track down type of resource
			bool							_Multisample				= false;
			bool							_IsSubResourceStatesDirty	= false;
		};

		template<typename ResourceStructureType = uint8_t>
		class MapScope
		{
		public:
			MapScope(_In_ Resource& InResource, _In_ uint32_t InMapSize, _In_ uint32_t InMapOffset = 0, _In_ uint32_t InMIPIndex = 0, _In_ uint32_t InPlaneSlice = 0, _In_ uint32_t InArraySlice = 0)
				: _Resource(InResource)
				, _Range(InMapSize, InMapOffset, InMIPIndex, InPlaneSlice, InArraySlice)
				, _DataPointer(InResource.Map<uint8_t>(_Range))
			{
			}
			MapScope(_In_ Resource& InResource, _In_ const MapRange& InRange)
				: _Resource(InResource)
				, _Range(InRange)
				, _DataPointer(InResource.Map<uint8_t>(InRange))
			{
			}
			MapScope(_In_ Resource& InResource)
				: _Resource(InResource)
				, _Range(InResource.GetBufferSize())
				, _DataPointer(InResource.Map<uint8_t>(_Range))
			{
			}
			~MapScope()
			{
				_Resource.Unmap(_Range);
			}

			ResourceStructureType* GetDataPointer() const { return reinterpret_cast<ResourceStructureType*>(_DataPointer); }
			ResourceStructureType* operator->() const { return reinterpret_cast<ResourceStructureType*>(_DataPointer); }
			ResourceStructureType& operator[](_In_ uint32_t ElementIndex)
			{
				ETERNAL_ASSERT(ElementIndex * _Resource.GetBufferStride() < _Range.MapSize);
				return *reinterpret_cast<ResourceStructureType*>(&_DataPointer[ElementIndex * _Resource.GetBufferStride()]);
			}

		private:
			Resource&	_Resource;
			MapRange	_Range;
			uint8_t*	_DataPointer = nullptr;
		};
	}
}
