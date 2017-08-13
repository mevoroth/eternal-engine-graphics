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
	//if (VisibleFromCPU)
	//{
	//	if (Coherent)
	//	{
	//		ETERNAL_ASSERT(!Cached);
	//		return D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE;
	//	}
	//	else if (Cached)
	//	{
	//		return D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//	}
	//}
	//else
	//{
	//	ETERNAL_ASSERT(!Coherent);
	//	ETERNAL_ASSERT(!Cached);
	//	return D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
	//}
	//ETERNAL_ASSERT(false);
	if (VisibleFromCPU && !(Coherent || Cached))
	{
		ETERNAL_ASSERT(false); // Wrong flags
	}
	return D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
}

D3D12Heap::D3D12Heap(_In_ Device& DeviceObj, _In_ const HeapType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached)
	: Heap(DeviceObj, HeapTypeObj, ResourcesCount)
{
	_HeapType			= BuildD3D12HeapType(InVRAM, VisibleFromCPU, Coherent, Cached);
	_CpuPageProperty	= BuildD3D12CPUPageProperty(VisibleFromCPU, Coherent, Cached);
}

void D3D12Heap::Initialize(_In_ size_t Size)
{
	ETERNAL_ASSERT(!_Heap);

	D3D12_HEAP_DESC HeapDesc;
	HeapDesc.SizeInBytes						= Size * GetResourcesCount();
	HeapDesc.Properties.Type					= _HeapType;
	HeapDesc.Properties.CPUPageProperty			= _CpuPageProperty;
	HeapDesc.Properties.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	HeapDesc.Properties.CreationNodeMask		= GetDevice().GetDeviceMask();
	HeapDesc.Properties.VisibleNodeMask			= GetDevice().GetDeviceMask();
	HeapDesc.Alignment							= D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
	HeapDesc.Flags								= GetHeapType() == HEAP_TYPE_TEXTURE ? D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES : D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;

	HRESULT hr = static_cast<D3D12Device&>(GetDevice()).GetD3D12Device()->CreateHeap(&HeapDesc, __uuidof(ID3D12Heap), (void**)&_Heap);
	ETERNAL_ASSERT(hr == S_OK);

	SetSize(Size);
}

bool D3D12Heap::IsInitialized() const
{
	return _Heap != nullptr;
}

D3D12Heap::~D3D12Heap()
{
	_Heap->Release();
	_Heap = nullptr;
}
