#ifndef _D3D12_COMMAND_QUEUE_HPP_
#define _D3D12_COMMAND_QUEUE_HPP_

#include "Graphics/CommandQueue.hpp"
#include <cstdint>
#include <vector>
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class D3D12Device;
		class D3D12CommandAllocator;
		class D3D12CommandList;

		class D3D12CommandQueue : public CommandQueue
		{
		public:
			D3D12CommandQueue(_In_ D3D12Device& Device);

			virtual void Reset(_In_ uint32_t FrameIndex) override;
			void Flush(_In_ D3D12CommandList CommandLists[], _In_ uint32_t CommandListsCount);
			D3D12CommandAllocator* GetCommandAllocator(_In_ uint32_t FrameIndex);

			inline ID3D12CommandQueue* GetD3D12CommandQueue() { return _CommandQueue; }
			inline D3D12_COMMAND_LIST_TYPE GetCommandListType() const { return _CommandListType; }

		private:
			vector<D3D12CommandAllocator*> _CommandAllocators;
			ID3D12CommandQueue* _CommandQueue = nullptr;
			D3D12_COMMAND_LIST_TYPE _CommandListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
		};
	}
}

#endif
