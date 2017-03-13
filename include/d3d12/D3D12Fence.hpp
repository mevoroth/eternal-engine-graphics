#ifndef _D3D12_FENCE_HPP_
#define _D3D12_FENCE_HPP_

#include "Graphics/Fence.hpp"

#include <cstdint>
#include <vector>

struct ID3D12Fence;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class D3D12CommandQueue;

		using namespace std;

		class D3D12Fence : public Fence
		{
		public:
			D3D12Fence(_In_ Device& DeviceObj, _In_ uint32_t SimultaneousResourcesCount = 1u);
			~D3D12Fence();

			virtual void Signal(_In_ SwapChain& SwapChainObj, _In_ CommandQueue& CommandQueueObj, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) override;
			virtual void Wait(_In_ Device& DeviceObj) override;
			virtual void Reset(_In_ Device& DeviceObj) override;

		private:
			vector<uint64_t>	_FenceValues;
			ID3D12Fence*		_Fence = nullptr;
			void*				_FenceEvent = nullptr;
			uint32_t			_CurrentFence = 0u;
		};
	}
}

#endif
