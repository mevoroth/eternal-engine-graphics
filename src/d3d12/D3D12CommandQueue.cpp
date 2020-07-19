#include "d3d12/D3D12CommandQueue.hpp"

#include "Graphics/SwapChain.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "d3d12/D3D12Fence.hpp"

using namespace Eternal::Graphics;

D3D12CommandQueue::D3D12CommandQueue(_In_ Device& DeviceObj, _In_ const CommandListType& Type)
	: _CommandListType(D3D12_COMMAND_LIST_TYPES[Type])
{
	D3D12Device& D3D12DeviceObj = static_cast<D3D12Device&>(DeviceObj);

	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
	CommandQueueDesc.Type		= _CommandListType;
	CommandQueueDesc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	CommandQueueDesc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
	CommandQueueDesc.NodeMask	= D3D12DeviceObj.GetDeviceMask();

	HRESULT hr = D3D12DeviceObj.GetD3D12Device()->CreateCommandQueue(&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&_CommandQueue);
	ETERNAL_ASSERT(hr == S_OK);

	//_CommandAllocators.resize(FrameCount);
	//for (uint32_t CommandAllocatorIndex = 0; CommandAllocatorIndex < _CommandAllocators.size(); ++CommandAllocatorIndex)
	//{
	//	_CommandAllocators[CommandAllocatorIndex] = new D3D12CommandAllocator(DeviceObj, _CommandListType);
	//}

	//_CommandList = new D3D12CommandList(DeviceObj, *_CommandAllocators[0], COMMAND_LIST_GRAPHIC);
}

D3D12CommandQueue::~D3D12CommandQueue()
{
	//for (uint32_t CommandAllocatorIndex = 0; CommandAllocatorIndex < _CommandAllocators.size(); ++CommandAllocatorIndex)
	//{
	//	delete _CommandAllocators[CommandAllocatorIndex];
	//}
	//_CommandAllocators.clear();
	_CommandQueue->Release();
	_CommandQueue = nullptr;
}

void D3D12CommandQueue::Submit(_In_ Context& GfxContext, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount)
{
	vector<ID3D12CommandList*> D3D12CommandLists;
	D3D12CommandLists.resize(CommandListsCount);
	for (uint32_t CommandListIndex = 0; CommandListIndex < CommandListsCount; ++CommandListIndex)
	{
		D3D12CommandLists[CommandListIndex] = static_cast<D3D12CommandList*>(CommandLists[CommandListIndex])->GetD3D12GraphicsCommandList();
	}
	GetD3D12CommandQueue()->ExecuteCommandLists(CommandListsCount, D3D12CommandLists.data());
}
