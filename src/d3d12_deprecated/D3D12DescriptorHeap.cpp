#include "d3d12_deprecated/D3D12DescriptorHeap.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12_deprecated/D3D12DescriptorTable.hpp"

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

ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_HEAP_TYPES) == int(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "D3D12 Heap Types declaration not complete");

D3D12DescriptorHeap::D3D12DescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount)
	: DescriptorHeap(Resources, ResourcesCount)
{
	D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType = D3D12_HEAP_TYPES[int(Resources[0].Parameter)];

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;

	DescriptorHeapDesc.Type				= DescriptorHeapType;
	DescriptorHeapDesc.Flags			= DescriptorHeapType <= D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DescriptorHeapDesc.NumDescriptors	= ResourcesCount;
	DescriptorHeapDesc.NodeMask			= static_cast<D3D12Device&>(DeviceObj).GetDeviceMask();

	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateDescriptorHeap(&DescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&_DescriptorHeap);
	ETERNAL_ASSERT(hr == S_OK);

	_CPUDescriptorHandleStart	= _DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	_DescriptorHandleSize		= static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->GetDescriptorHandleIncrementSize(DescriptorHeapType);

	_GPUDescriptorHandleStart	= _DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
}

D3D12DescriptorHeap::~D3D12DescriptorHeap()
{
	_DescriptorHeap->Release();
	_DescriptorHeap = nullptr;
}

//D3D12DescriptorHeap::D3D12DescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ uint32_t Register, _In_ const RootSignatureParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount)
//	: DescriptorHeap(Space, Register)
//	, _ResourcesPool(ResourcesCount)
//	, _HeapType(D3D12_PARAMETER_TYPES[HeapTypeObj])
//{
//	ETERNAL_ASSERT(ResourcesCount > 0);
//	_ResourcesCount = ResourcesCount;
//
//	for (uint32_t ResourceIndex = 0; ResourceIndex < _ResourcesCount; ++ResourceIndex)
//	{
//		_ResourcesPool.push_back(ResourcesCount - ResourceIndex - 1u);
//	}
//
//	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;
//
//	DescriptorHeapDesc.Type				= D3D12_HEAP_TYPES[HeapTypeObj];
//	DescriptorHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//	DescriptorHeapDesc.NumDescriptors	= ResourcesCount;
//	DescriptorHeapDesc.NodeMask			= static_cast<D3D12Device&>(DeviceObj).GetDeviceMask();
//
//	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateDescriptorHeap(&DescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&_DescriptorHeap);
//	ETERNAL_ASSERT(hr == S_OK);
//
//	_CPUDescriptorHandleStart = _DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
//	_DescriptorHandleSize = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->GetDescriptorHandleIncrementSize(D3D12_HEAP_TYPES[HeapTypeObj]);
//
//	_GPUDescriptorHandleStart = _DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
//}

DescriptorTable* D3D12DescriptorHeap::CreateView(_In_ Device& DeviceObj)
{
	//return new D3D12DescriptorTable(*this);
	return nullptr;
}

DescriptorHeapHandle D3D12DescriptorHeap::Reserve(_In_ uint32_t Slot)
{
	const bool IsAllocated = (_ResourcesPool & (1ull << Slot));
	ETERNAL_ASSERT(Slot < GetResourcesCount());
	ETERNAL_ASSERT(!IsAllocated);
	return GetSlot(Slot);
}

DescriptorHeapHandle D3D12DescriptorHeap::Pop()
{
	uint32_t Slot = 0;
	for (; Slot < GetResourcesCount(); ++Slot)
	{
		const bool IsAllocated = (_ResourcesPool & (1ull << Slot));
		if (!IsAllocated)
			break;
	}
	ETERNAL_ASSERT(Slot < GetResourcesCount());
	_ResourcesPool |= (1 << Slot);
	return GetSlot(Slot);
}

void D3D12DescriptorHeap::Push(_In_ const DescriptorHeapHandle& Handle)
{
	uint64_t Slot = (Handle.Cpu.ptr - _CPUDescriptorHandleStart.ptr) / _DescriptorHandleSize;
	const bool IsAllocated = (_ResourcesPool & (1ull << Slot));
	ETERNAL_ASSERT(IsAllocated);
	_ResourcesPool &= ~(1 << Slot);
}

DescriptorHeapHandle D3D12DescriptorHeap::GetBase() const
{
	return GetSlot(0u);
}

DescriptorHeapHandle D3D12DescriptorHeap::GetSlot(_In_ uint32_t Slot) const
{
	DescriptorHeapHandle OutDescriptor;
	SIZE_T Offset = Slot * _DescriptorHandleSize;
	OutDescriptor.Cpu.ptr = _CPUDescriptorHandleStart.ptr + Offset;
	OutDescriptor.Gpu.ptr = _GPUDescriptorHandleStart.ptr + Offset;
	return OutDescriptor;
}
