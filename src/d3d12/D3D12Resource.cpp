#include "d3d12/D3D12Resource.hpp"

#include "d3d12/D3D12CommandList.hpp"

using namespace Eternal::Graphics;

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

void D3D12Resource::SetCpuDescriptor(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& CpuDescriptor)
{
	_CpuDescriptor = CpuDescriptor;
}
