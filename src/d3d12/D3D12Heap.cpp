#include "d3d12/D3D12Heap.hpp"

#include "Macros/Macros.hpp"
#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

static D3D12_HEAP_TYPE BuildD3D12HeapType(_In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
{
	if (InVRAM)
	{
		ETERNAL_ASSERT(!VisibleFromCPU);
		ETERNAL_ASSERT(!Coherent);
		ETERNAL_ASSERT(!Cached);
		return D3D12_HEAP_TYPE_DEFAULT;
	}
	else if (VisibleFromCPU)
	{
		if (Coherent)
		{
			ETERNAL_ASSERT(!Cached);
			return D3D12_HEAP_TYPE_UPLOAD;
		}
		else if (Cached)
		{
			return D3D12_HEAP_TYPE_READBACK;
		}
	}
	ETERNAL_ASSERT(false);
	return D3D12_HEAP_TYPE_CUSTOM;
}

static D3D12_CPU_PAGE_PROPERTY BuildD3D12CPUPageProperty(_In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
{
	if (VisibleFromCPU)
	{
		if (Coherent)
		{
			ETERNAL_ASSERT(!Cached);
			return D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE;
		}
		else if (Cached)
		{
			return D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
		}
	}
	else
	{
		ETERNAL_ASSERT(!Coherent);
		ETERNAL_ASSERT(!Cached);
		return D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
	}
	ETERNAL_ASSERT(false);
	return D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
}

D3D12Heap::D3D12Heap(_In_ Device& DeviceObj, _In_ size_t Size, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
	: Heap(Size, ResourcesCount)
{
	D3D12_HEAP_DESC HeapDesc;
	HeapDesc.SizeInBytes						= Size * (size_t)ResourcesCount;
	HeapDesc.Properties.Type					= BuildD3D12HeapType(InVRAM, VisibleFromCPU, Coherent, Cached);
	HeapDesc.Properties.CPUPageProperty			= BuildD3D12CPUPageProperty(VisibleFromCPU, Coherent, Cached);
	HeapDesc.Properties.MemoryPoolPreference	= D3D12_MEMORY_POOL_L1;
	HeapDesc.Properties.CreationNodeMask		= DeviceObj.GetDeviceMask();
	HeapDesc.Properties.VisibleNodeMask			= DeviceObj.GetDeviceMask();
	HeapDesc.Alignment							= D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	HeapDesc.Flags								= D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES;

	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateHeap(&HeapDesc, __uuidof(ID3D12Heap), (void**)&_Heap);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D12Heap::~D3D12Heap()
{
	_Heap->Release();
	_Heap = nullptr;
}
