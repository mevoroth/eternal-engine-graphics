#include "d3d12/D3D12CommandQueue.hpp"

#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

D3D12CommandQueue::D3D12CommandQueue(_In_ D3D12Device& Device)
	: _CommandListType(D3D12_COMMAND_LIST_TYPE_DIRECT)
{
	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
	CommandQueueDesc.Type = _CommandListType;
	CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	CommandQueueDesc.NodeMask = Device.GetDeviceMask();

	HRESULT hr = Device.GetD3D12Device()->CreateCommandQueue(&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&_CommandQueue);
	ETERNAL_ASSERT(hr == S_OK);

	hr = Device.GetD3D12Device()->CreateCommandAllocator(_CommandListType, __uuidof(ID3D12CommandAllocator), (void**)&_CommandAllocator);
	ETERNAL_ASSERT(hr == S_OK);
}

void D3D12CommandQueue::Reset()
{
	_CommandAllocator->Reset();
}
