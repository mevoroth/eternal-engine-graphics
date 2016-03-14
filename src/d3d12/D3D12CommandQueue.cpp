#include "d3d12/D3D12CommandQueue.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

D3D12CommandQueue::D3D12CommandQueue(_In_ D3D12Device& Device)
{
	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
	CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	CommandQueueDesc.NodeMask = 0;

	HRESULT hr = Device.GetDevice()->CreateCommandQueue(&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&_CommandQueue);
	ETERNAL_ASSERT(hr == S_OK);

	hr = Device.GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&_CommandAllocator);
	ETERNAL_ASSERT(hr == S_OK);
}
