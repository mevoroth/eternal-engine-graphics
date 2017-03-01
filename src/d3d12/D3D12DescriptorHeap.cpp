#include "d3d12/D3D12DescriptorHeap.hpp"

#include "Macros/Macros.hpp"
#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

D3D12DescriptorHeap::D3D12DescriptorHeap(_In_ D3D12Device& DeviceObj, _In_ const HeapType HeapTypeObj, _In_ uint32_t ResourceCount)
	: _ResourcesPool(ResourceCount)
{
	ETERNAL_ASSERT(ResourceCount > 0);
	_ResourceCount = ResourceCount;

	for (uint32_t ResourceIndex = 0; ResourceIndex < _ResourceCount; ++ResourceIndex)
	{
		_ResourcesPool.push_back(ResourceCount - ResourceIndex - 1u);

	}

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;

	DescriptorHeapDesc.Type = (D3D12_DESCRIPTOR_HEAP_TYPE)HeapTypeObj;
	DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DescriptorHeapDesc.NumDescriptors = ResourceCount;
	DescriptorHeapDesc.NodeMask = DeviceObj.GetDeviceMask();

	HRESULT hr = DeviceObj.GetD3D12Device()->CreateDescriptorHeap(&DescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&_DescriptorHeap);
	ETERNAL_ASSERT(hr == S_OK);

	_DescriptorHandleStart = _DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	_DescriptorHandleSize = DeviceObj.GetD3D12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
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

void D3D12DescriptorHeap::Push(_In_ uint32_t Slot)
{
	_ResourcesPool.push_back(Slot);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorHeap::GetSlot(_In_ uint32_t Slot) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE Handle = _DescriptorHandleStart;
	Handle.ptr += Slot * _DescriptorHandleSize;
	return Handle;
}
