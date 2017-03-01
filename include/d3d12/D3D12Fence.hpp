#ifndef _D3D12_FENCE_HPP_
#define _D3D12_FENCE_HPP_

#include <cstdint>
#include <vector>

struct ID3D12Fence;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;
		class D3D12CommandQueue;

		using namespace std;

		class D3D12Fence
		{
		public:
			D3D12Fence(_In_ D3D12Device& DeviceObj, _In_ uint32_t SimultaneousResourcesCount = 1u);
			~D3D12Fence();

			void Signal(_In_ D3D12CommandQueue& CommandQueueObj);
			void Wait();

		private:
			vector<uint64_t>	_FenceValues;
			ID3D12Fence*		_Fence = nullptr;
			void*				_FenceEvent = nullptr;
			uint32_t			_CurrentFence = 0u;
		};
	}
}

#endif
