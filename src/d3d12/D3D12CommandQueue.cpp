#include "d3d12/D3D12CommandQueue.hpp"

#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"

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

	_CommandAllocators.resize(Device.GetBackBufferFrameCount());
	for (uint32_t CommandAllocatorIndex = 0; CommandAllocatorIndex < _CommandAllocators.size(); ++CommandAllocatorIndex)
	{
		_CommandAllocators[CommandAllocatorIndex] = new D3D12CommandAllocator(Device, _CommandListType);
	}
}

void D3D12CommandQueue::Reset(_In_ uint32_t FrameIndex)
{
	ETERNAL_ASSERT(FrameIndex < _CommandAllocators.size());
	_CommandAllocators[FrameIndex]->Reset();
}

D3D12CommandAllocator* D3D12CommandQueue::GetCommandAllocator(_In_ uint32_t FrameIndex)
{
	ETERNAL_ASSERT(FrameIndex < _CommandAllocators.size());
	return _CommandAllocators[FrameIndex];
}
