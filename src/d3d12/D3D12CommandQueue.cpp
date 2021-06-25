#include "d3d12/D3D12CommandQueue.hpp"

#include "Graphics/SwapChain.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandList.hpp"
#include "d3d12/D3D12Utils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12CommandQueue::D3D12CommandQueue(_In_ Device& InDevice, _In_ const CommandType& Type)
			: CommandQueue(Type)
			, _CommandQueueType(D3D12::ConvertCommandTypeToD3D12CommandListType(Type))
		{
			D3D12Device& InD3D12Device = static_cast<D3D12Device&>(InDevice);

			D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
			CommandQueueDesc.Type		= _CommandQueueType;
			CommandQueueDesc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
			CommandQueueDesc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
			CommandQueueDesc.NodeMask	= InD3D12Device.GetDeviceMask();

			D3D12::VerifySuccess(
				InD3D12Device.GetD3D12Device()->CreateCommandQueue(
					&CommandQueueDesc,
					__uuidof(ID3D12CommandQueue),
					(void**)&_CommandQueue
				)
			);
		}

		D3D12CommandQueue::~D3D12CommandQueue()
		{
			_CommandQueue->Release();
			_CommandQueue = nullptr;
		}

		void D3D12CommandQueue::SubmitCommandLists(_In_ GraphicsContext& Context, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount)
		{
			vector<ID3D12CommandList*> D3D12CommandLists;
			D3D12CommandLists.resize(CommandListsCount);
			for (uint32_t CommandListIndex = 0; CommandListIndex < CommandListsCount; ++CommandListIndex)
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
