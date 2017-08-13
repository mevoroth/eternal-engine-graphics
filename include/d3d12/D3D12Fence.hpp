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
		class CommandQueue;

		using namespace std;

		class D3D12Fence : public Fence
		{
		public:
			D3D12Fence(_In_ Device& DeviceObj);
			~D3D12Fence();

			virtual void Signal(_In_ CommandQueue& CommandQueueObj) override;
			virtual void Wait(_In_ Device& DeviceObj) override;
			virtual void Reset(_In_ Device& DeviceObj) override;

		private:
			uint64_t		_PreviousFenceValue	= 0ull;
			uint64_t		_FenceValue			= 0ull;
			ID3D12Fence*	_Fence				= nullptr;
			void*			_FenceEvent			= nullptr;
		};
	}
}

#endif
