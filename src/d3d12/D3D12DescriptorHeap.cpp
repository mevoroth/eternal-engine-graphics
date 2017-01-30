#include "d3d12/D3D12DescriptorHeap.hpp"

#include "Macros/Macros.hpp"
#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

D3D12DescriptorHeap::D3D12DescriptorHeap(_In_ D3D12Device& DeviceObj, _In_ const HeapType HeapTypeObj, _In_ uint32_t ResourceCount)
{
	ETERNAL_ASSERT(ResourceCount > 0);
	_ResourceCount = ResourceCount;

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;

	DescriptorHeapDesc.Type = (D3D12_DESCRIPTOR_HEAP_TYPE)HeapTypeObj;
	DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DescriptorHeapDesc.NumDescriptors = ResourceCount;
	DescriptorHeapDesc.NodeMask = DeviceObj.GetDeviceMask();

	HRESULT hr = DeviceObj.GetDevice()->CreateDescriptorHeap(&DescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&_DescriptorHeap);
	ETERNAL_ASSERT(hr == S_OK);

	_DescriptorHandleStart = _DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	_DescriptorHandleSize = DeviceObj.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12DescriptorHeap::GetSlot(_In_ uint32_t Slot) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE Handle = _DescriptorHandleStart;
	Handle.ptr += Slot * _DescriptorHandleSize;
	return Handle;
}
