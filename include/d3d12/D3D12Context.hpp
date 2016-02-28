#ifndef _D3D12_CONTEXT_HPP_
#define _D3D12_CONTEXT_HPP_

#include <cstdint>

struct ID3D12CommandAllocator;
struct ID3D12CommandQueue;
struct ID3D12Fence;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Context
		{
		public:
			D3D12Context();

			ID3D12CommandAllocator* GetCommandAllocator()
			{
				return _CommandAllocator;
			}

		private:
			ID3D12CommandQueue* _CommandQueue = nullptr;
			ID3D12CommandAllocator* _CommandAllocator = nullptr;
			ID3D12Fence* _Fence = nullptr;
			uint64_t _FenceValue = 0ull;
		};
	}
}

#endif
