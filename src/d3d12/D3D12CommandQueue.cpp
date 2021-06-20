#include "d3d12/D3D12CommandQueue.hpp"

#include "Graphics_deprecated/SwapChain.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "d3d12/D3D12Utils.hpp"

#include "d3d12_deprecated/D3D12CommandAllocator.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12CommandQueue::D3D12CommandQueue(_In_ Device& DeviceObj, _In_ const CommandType& Type)
			: CommandQueue(DeviceObj, Type)
			, _CommandQueueType(D3D12_COMMAND_LIST_TYPES[uint32_t(Type)])
		{
			D3D12Device& D3D12DeviceObj = static_cast<D3D12Device&>(DeviceObj);

			D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
			CommandQueueDesc.Type		= _CommandQueueType;
			CommandQueueDesc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
			CommandQueueDesc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
			CommandQueueDesc.NodeMask	= D3D12DeviceObj.GetDeviceMask();

			HRESULT hr = D3D12DeviceObj.GetD3D12Device()->CreateCommandQueue(&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&_CommandQueue);
			ETERNAL_ASSERT(hr == S_OK);
		}

		D3D12CommandQueue::~D3D12CommandQueue()
		{
			_CommandQueue->Release();
			_CommandQueue = nullptr;
		}

		void D3D12CommandQueue::SubmitCommandLists(_In_ GraphicsContext& GfxContext, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount)
		{
			vector<ID3D12CommandList*> D3D12CommandLists;
			D3D12CommandLists.resize(CommandListsCount);
			for (int32_t CommandListIndex = 0; CommandListIndex < CommandListsCount; ++CommandListIndex)
			{
				D3D12CommandLists[CommandListIndex] = static_cast<D3D12CommandList*>(CommandLists[CommandListIndex])->GetD3D12CommandList();
			}

			_CommandQueue->ExecuteCommandLists(
				CommandListsCount,
				D3D12CommandLists.data()
			);
		}
	}
}
