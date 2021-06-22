#pragma once

#include "Graphics/Fence.hpp"

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
			D3D12Fence(_In_ Device& InDevice);
			~D3D12Fence();

			virtual void Wait(_In_ Device& InDevice) override;
			virtual void Reset(_In_ Device& InDevice) override;

		private:
			uint64_t		_PreviousFenceValue	= 0ull;
			uint64_t		_FenceValue			= 0ull;
			ID3D12Fence*	_Fence				= nullptr;
			void*			_FenceEvent			= nullptr;
		};
	}
}
