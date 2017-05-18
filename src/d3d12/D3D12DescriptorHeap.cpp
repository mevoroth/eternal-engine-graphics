#include "d3d12/D3D12DescriptorHeap.hpp"

#include "Macros/Macros.hpp"
#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

const D3D12_DESCRIPTOR_HEAP_TYPE D3D12_HEAP_TYPES[] =
{
	D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	D3D12_DESCRIPTOR_HEAP_TYPE_DSV
};

ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_HEAP_TYPES) == ROOT_SIGNATURE_PARAMETER_COUNT, "D3D12 Heap Types declaration not complete");

const D3D12_ROOT_PARAMETER_TYPE D3D12_PARAMETER_TYPES[] =
{
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_UAV,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_UAV,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_UAV,
	D3D12_ROOT_PARAMETER_TYPE_CBV,
	D3D12_ROOT_PARAMETER_TYPE_UAV,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_SRV
};

ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_PARAMETER_TYPES) == ROOT_SIGNATURE_PARAMETER_COUNT, "D3D12 Parameters Types declaration not complete");

D3D12DescriptorHeap::D3D12DescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ uint32_t Register, _In_ const RootSignatureDynamicParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount)
	: DescriptorHeap(Space, Register)
	, _ResourcesPool(ResourcesCount)
	, _HeapType(D3D12_PARAMETER_TYPES[HeapTypeObj])
{
	ETERNAL_ASSERT(ResourcesCount > 0);
	_ResourcesCount = ResourcesCount;

	for (uint32_t ResourceIndex = 0; ResourceIndex < _ResourcesCount; ++ResourceIndex)
	{
		_ResourcesPool.push_back(ResourcesCount - ResourceIndex - 1u);
	}

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;

	DescriptorHeapDesc.Type				= D3D12_HEAP_TYPES[HeapTypeObj];
	DescriptorHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DescriptorHeapDesc.NumDescriptors	= ResourcesCount;
	DescriptorHeapDesc.NodeMask			= static_cast<D3D12Device&>(DeviceObj).GetDeviceMask();

	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateDescriptorHeap(&DescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&_DescriptorHeap);
	ETERNAL_ASSERT(hr == S_OK);

	_DescriptorHandleStart = _DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	_DescriptorHandleSize = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->GetDescriptorHandleIncrementSize(D3D12_HEAP_TYPES[HeapTypeObj]);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorHeap::Reserve(_In_ uint32_t Slot)
{
	vector<uint32_t>::iterator Index = find(_ResourcesPool.begin(), _ResourcesPool.end(), Slot);
	ETERNAL_ASSERT(Index != _ResourcesPool.end());
	_ResourcesPool.erase(Index);
	return GetSlot(Slot);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorHeap::Pop()
{
	ETERNAL_ASSERT(_ResourcesPool.size() > 0);
	const uint32_t Slot = _ResourcesPool.back();
	_ResourcesPool.pop_back();
	return GetSlot(Slot);
}

void D3D12DescriptorHeap::Push(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& Handle)
{
	_ResourcesPool.push_back((Handle.ptr - _DescriptorHandleStart.ptr) / _DescriptorHandleSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorHeap::GetSlot(_In_ uint32_t Slot) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE Handle = _DescriptorHandleStart;
	Handle.ptr += Slot * _DescriptorHandleSize;
	return Handle;
}
