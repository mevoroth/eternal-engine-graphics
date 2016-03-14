#ifndef _D3D12_COMMAND_QUEUE_HPP_
#define _D3D12_COMMAND_QUEUE_HPP_

struct ID3D12CommandQueue;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;

		class D3D12CommandQueue
		{
		public:
			D3D12CommandQueue(_In_ D3D12Device& Device);

			inline ID3D12CommandQueue* GetD3D12CommandQueue() { return _CommandQueue; }

		private:
			ID3D12CommandQueue* _CommandQueue = nullptr;
			ID3D12CommandAllocator* _CommandAllocator = nullptr;
		};
	}
}

#endif
