#ifndef _D3D12_COMMAND_QUEUE_HPP_
#define _D3D12_COMMAND_QUEUE_HPP_

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;

		class D3D12CommandQueue
		{
		public:
			D3D12CommandQueue(_In_ D3D12Device& Device);

			void Reset();

			inline ID3D12CommandQueue* GetD3D12CommandQueue() { return _CommandQueue; }
			inline ID3D12CommandAllocator* GetD3D12CommandAllocator() { return _CommandAllocator; }
			inline D3D12_COMMAND_LIST_TYPE GetCommandListType() const { return _CommandListType; }

		private:
			ID3D12CommandQueue* _CommandQueue = nullptr;
			ID3D12CommandAllocator* _CommandAllocator = nullptr;
			D3D12_COMMAND_LIST_TYPE _CommandListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
		};
	}
}

#endif
