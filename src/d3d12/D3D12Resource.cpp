#include "d3d12/D3D12Resource.hpp"

#include <d3d12.h>
#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12SwapChain.hpp"
#include "d3d12/D3D12Heap.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "d3d12/D3D12View.hpp"

using namespace Eternal::Graphics;

D3D12Resource::D3D12Resource(_In_ Device& DeviceObj, _In_ Heap& HeapObj)
	: Resource(HeapObj)
{
	D3D12_RESOURCE_DESC ResourceDesc;
	
	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreatePlacedResource(
		static_cast<D3D12Heap&>(HeapObj).GetD3D12Heap(),
		GetHeapSlot(),
		&ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&_Resource
	);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D12Resource::D3D12Resource(_In_ ID3D12Resource* BackBuffer)
	: Resource(*(Heap*)nullptr)
	, _Resource(BackBuffer)
{
}

D3D12Resource::~D3D12Resource()
{
	_Resource->Release();
	_Resource = nullptr;
}

void D3D12Resource::Transition(_In_ D3D12CommandList& CommandList, _In_ const ResourceState& NewState)
{
	if (_OldState == NewState)
		return;

	D3D12_RESOURCE_BARRIER ResourceBarrier;
	ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	ResourceBarrier.Transition.pResource	= _Resource;
	ResourceBarrier.Transition.StateBefore	= (D3D12_RESOURCE_STATES)_OldState;
	ResourceBarrier.Transition.StateAfter	= (D3D12_RESOURCE_STATES)NewState;
	ResourceBarrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	ResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	CommandList.GetD3D12GraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);

	_OldState = NewState;
}

View* D3D12Resource::CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj)
{
	return new D3D12View(DeviceObj, DescriptorHeapObj, *this, ViewType, FormatObj);
}

View* D3D12Resource::CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj, _In_ uint64_t Offset, _In_ uint64_t Size)
{
	ETERNAL_ASSERT(false);
	return nullptr;
}

View* D3D12Resource::CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj)
{
	return new D3D12View(DeviceObj, DescriptorHeapObj, *this);
}

void* D3D12Resource::Map(_In_ Device& DeviceObj)
{
	ETERNAL_ASSERT(false);
	return nullptr;
}

void D3D12Resource::Unmap(_In_ Device& DeviceObj)
{
	ETERNAL_ASSERT(false);
}
