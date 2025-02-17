#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Format.hpp"
#include "d3d12/D3D12Library.h"
#include <string>

#if ETERNAL_USE_DEBUG_VERBOSE
#include "Log/Log.hpp"
#include <sstream>
#include <iomanip>
#endif

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		static void LogResourceAddress(_In_ D3D12Resource& InResource)
		{
#if ETERNAL_USE_DEBUG_VERBOSE
			using namespace std;
			using namespace Eternal::LogSystem;

			stringstream LogResourceAddressStream;
			LogResourceAddressStream << "Resource: { Name = " << InResource.GetResourceName() << ", CPUPtr = 0x" << hex << setfill('0') << setw(16) << InResource.GetD3D12Resource() << ", GPUVA = 0x" << setfill('0') << setw(16) << InResource.GetD3D12Resource()->GetGPUVirtualAddress() << " }";
			LogWrite(LogInfo, LogGraphics, LogResourceAddressStream.str());
#endif
		}

		D3D12Resource::D3D12Resource(_In_ const D3D12ResourceBackBufferCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_BACK_BUFFER)
			, _Resource(InResourceCreateInformation.BackBufferResource)
		{
			ETERNAL_ASSERT(_Resource);

			_SetDebugName();
		}

		D3D12Resource::D3D12Resource(_In_ const TextureResourceCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceType::RESOURCE_TYPE_TEXTURE)
		{
			const TextureCreateInformation& InTextureInformation = InResourceCreateInformation.TextureInformation;

			uint32_t SubResourcesCount = GetMIPLevels() * InTextureInformation.DepthOrArraySize;
			GetSubResourceStates().resize(SubResourcesCount);
			for (uint32_t SubResourceIndex = 0; SubResourceIndex < SubResourcesCount; ++SubResourceIndex)
				GetSubResourceStates()[SubResourceIndex] = GetResourceState();

			if (InResourceCreateInformation.MemoryType == ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_MEMORY)
			{
				ETERNAL_ASSERT(InResourceCreateInformation.ResourceState != TransitionState::TRANSITION_UNDEFINED)
			}

			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InResourceCreateInformation.GfxDevice);
			
			D3D12_HEAP_PROPERTIES D3D12HeapProperties;
			D3D12HeapProperties.Type					= ConvertResourceMemoryTypeToD3D12HeapType(InResourceCreateInformation.MemoryType);
			D3D12HeapProperties.CPUPageProperty			= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;//ConvertResourceMemoryTypeToD3D12CPUPageProperty(InResourceCreateInformation.MemoryType);
			D3D12HeapProperties.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
			D3D12HeapProperties.CreationNodeMask		= InD3DDevice.GetDeviceMask();
			D3D12HeapProperties.VisibleNodeMask			= InD3DDevice.GetDeviceMask();

			D3D12_RESOURCE_DESC D3D12ResourceDesc;
			D3D12ResourceDesc.Dimension				= ConvertResourceDimensionToD3D12ResourceDimension(InTextureInformation.Dimension);
			D3D12ResourceDesc.Alignment				= 0;
			D3D12ResourceDesc.Width					= InTextureInformation.Width;
			D3D12ResourceDesc.Height				= InTextureInformation.Height;
			D3D12ResourceDesc.DepthOrArraySize		= static_cast<UINT16>(InTextureInformation.DepthOrArraySize);
			D3D12ResourceDesc.MipLevels				= static_cast<UINT16>(InTextureInformation.MIPLevels);
			D3D12ResourceDesc.Format				= ConvertFormatToD3D12Format(InTextureInformation.ResourceFormat).Format;
			D3D12ResourceDesc.SampleDesc.Count		= 1;
			D3D12ResourceDesc.SampleDesc.Quality	= 0;
			D3D12ResourceDesc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;
			D3D12ResourceDesc.Flags					= ConvertTextureResourceUsageToD3D12ResourceFlags(InTextureInformation.Usage);

			D3D12_RESOURCE_ALLOCATION_INFO D3D12ResourceAllocationInfo = InD3DDevice.GetD3D12Device()->GetResourceAllocationInfo(
				InD3DDevice.GetDeviceMask(),
				1, &D3D12ResourceDesc
			);
			D3D12ResourceDesc.Alignment				= D3D12ResourceAllocationInfo.Alignment;

			switch (InResourceCreateInformation.MemoryType)
			{
			case ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_READBACK:
				GetResourceCreateInformation().ResourceState = TransitionState::TRANSITION_COPY_WRITE;
				break;

			case ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD:
				GetResourceCreateInformation().ResourceState = TransitionState::TRANSITION_GENERIC_READ;
				break;
			}

			D3D12_RESOURCE_STATES InitialStates = ConvertTransitionStateToD3D12ResourceStates(InResourceCreateInformation.ResourceState);

			D3D12_CLEAR_VALUE ClearValue;
			ClearValue.Format	= D3D12ResourceDesc.Format;
			memcpy(ClearValue.Color, GetClearValue(), sizeof(float) * ETERNAL_ARRAYSIZE(ClearValue.Color));

			VerifySuccess(
				InD3DDevice,
				InD3DDevice.GetD3D12Device()->CreateCommittedResource(
					&D3D12HeapProperties,
					D3D12_HEAP_FLAG_NONE,
					&D3D12ResourceDesc,
					InitialStates,
					(InTextureInformation.Usage & TextureResourceUsageRenderTargetDepthStencil) != TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE ? &ClearValue : nullptr,
					__uuidof(ID3D12Resource),
					reinterpret_cast<void**>(&_Resource)
				)
			);

			_SetDebugName();
			LogResourceAddress(*this);
		}

		D3D12Resource::D3D12Resource(_In_ const BufferResourceCreateInformation& InResourceCreateInformation)
			: Resource(
				InResourceCreateInformation,
				((InResourceCreateInformation.BufferInformation.Usage & BufferResourceUsage::BUFFER_RESOURCE_USAGE_CONSTANT_BUFFER) != BufferResourceUsage::BUFFER_RESOURCE_USAGE_NONE
					? ResourceType::RESOURCE_TYPE_CONSTANT_BUFFER
					: ResourceType::RESOURCE_TYPE_BUFFER)
			)
		{
			if (InResourceCreateInformation.MemoryType == ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_MEMORY)
			{
				ETERNAL_ASSERT(InResourceCreateInformation.ResourceState != TransitionState::TRANSITION_UNDEFINED)
			}

			if (GetResourceTypeRaw() == ResourceType::RESOURCE_TYPE_CONSTANT_BUFFER)
				GetResourceCreateInformation().BufferInformation.Stride = Align<UINT>(GetResourceCreateInformation().BufferInformation.Stride, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);

			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InResourceCreateInformation.GfxDevice);
			
			D3D12_HEAP_PROPERTIES D3D12HeapProperties;
			D3D12HeapProperties.Type					= ConvertResourceMemoryTypeToD3D12HeapType(InResourceCreateInformation.MemoryType);
			D3D12HeapProperties.CPUPageProperty			= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;//ConvertResourceMemoryTypeToD3D12CPUPageProperty(InResourceCreateInformation.MemoryType);
			D3D12HeapProperties.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
			D3D12HeapProperties.CreationNodeMask		= InD3DDevice.GetDeviceMask();
			D3D12HeapProperties.VisibleNodeMask			= InD3DDevice.GetDeviceMask();

			const BufferCreateInformation& InBufferInformation = GetResourceCreateInformation().BufferInformation;

			D3D12_RESOURCE_DESC D3D12ResourceDesc;
			D3D12ResourceDesc.Dimension				= D3D12_RESOURCE_DIMENSION_BUFFER;
			D3D12ResourceDesc.Alignment				= D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			D3D12ResourceDesc.Width					= InBufferInformation.Stride * InBufferInformation.ElementCount;
			D3D12ResourceDesc.Height				= 1;
			D3D12ResourceDesc.DepthOrArraySize		= 1;
			D3D12ResourceDesc.MipLevels				= 1;
			D3D12ResourceDesc.Format				= DXGI_FORMAT_UNKNOWN;
			D3D12ResourceDesc.SampleDesc.Count		= 1;
			D3D12ResourceDesc.SampleDesc.Quality	= 0;
			D3D12ResourceDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			D3D12ResourceDesc.Flags					= ConvertBufferResourceUsageToD3D12ResourceFlags(InBufferInformation.Usage);

			switch (InResourceCreateInformation.MemoryType)
			{
			case ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_READBACK:
				GetResourceCreateInformation().ResourceState = TransitionState::TRANSITION_COPY_WRITE;
				break;

			case ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD:
				GetResourceCreateInformation().ResourceState = TransitionState::TRANSITION_GENERIC_READ;
				break;
			}

			D3D12_RESOURCE_STATES InitialStates = ConvertTransitionStateToD3D12ResourceStates(GetResourceCreateInformation().ResourceState);

			VerifySuccess(
				InD3DDevice,
				InD3DDevice.GetD3D12Device()->CreateCommittedResource(
					&D3D12HeapProperties,
					D3D12_HEAP_FLAG_NONE,
					&D3D12ResourceDesc,
					InitialStates,
					nullptr,
					__uuidof(ID3D12Resource),
					reinterpret_cast<void**>(&_Resource)
				)
			);

			_SetDebugName();
			LogResourceAddress(*this);
		}

		D3D12Resource::~D3D12Resource()
		{
			_Resource->Release();
			_Resource = nullptr;
		}

		void D3D12Resource::_SetDebugName()
		{
			ETERNAL_ASSERT(_Resource);
			std::wstring UTF8String(GetResourceCreateInformation().Name.begin(), GetResourceCreateInformation().Name.end());
			VerifySuccess(
				_Resource->SetName(UTF8String.c_str())
			);
		}

		void* D3D12Resource::Map(_In_ const MapRange& InMapRange)
		{
			D3D12_RANGE D3DMapRange;
			D3DMapRange.Begin	= InMapRange.MapOffset;
			D3DMapRange.End		= InMapRange.MapSize;

			void* OutData = nullptr;

			VerifySuccess(
				GetResourceCreateInformation().GfxDevice,
				_Resource->Map(
					//D3D12CalcSubresource(
					//	InMapRange.MIPIndex,
					//	InMapRange.ArraySlice,
					//	InMapRange.PlaneSlice,
					//	GetMIPLevels(),
					//	GetArraySize()
					//),
					0,
					&D3DMapRange,
					&OutData
				)
			);

			return OutData;
		}

		void D3D12Resource::Unmap(_In_ const MapRange& InMapRange)
		{
			D3D12_RANGE D3DWrittenRange;
			D3DWrittenRange.Begin	= InMapRange.MapOffset;
			D3DWrittenRange.End		= InMapRange.MapSize;

			_Resource->Unmap(
				//D3D12CalcSubresource(
				//	InMapRange.MIPIndex,
				//	InMapRange.ArraySlice,
				//	InMapRange.PlaneSlice,
				//	GetMIPLevels(),
				//	GetArraySize()
				//),
				0,
				&D3DWrittenRange
			);
		}

		TextureToBufferMemoryFootprint D3D12Resource::GetTextureToBufferMemoryFootprint(_In_ Device& InDevice) const
		{
			D3D12_PLACED_SUBRESOURCE_FOOTPRINT Footprint	= {};
			UINT NumRows									= 0;
			UINT64 RowSizeInBytes							= 0;
			UINT64 TotalBytes								= 0;

			D3D12_RESOURCE_DESC DestinationResourceDesc	= _Resource->GetDesc();
			static_cast<D3D12Device&>(InDevice).GetD3D12Device()->GetCopyableFootprints(
				&DestinationResourceDesc,
				0, 1, 0,
				&Footprint,
				&NumRows,
				&RowSizeInBytes,
				&TotalBytes
			);

			return {
				Footprint.Footprint.RowPitch,
				static_cast<uint32_t>(TotalBytes)
			};
		}
	}
}

#endif
