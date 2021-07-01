#pragma once

#include "Graphics/Fence.hpp"

struct ID3D12Fence;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class D3D12CommandQueue;

		class D3D12Fence final : public Fence
		{
		public:
			static constexpr uint64_t InvalidFenceValue = ~0ull;

			D3D12Fence(_In_ Device& InDevice);
			~D3D12Fence();

			virtual void Wait(_In_ Device& InDevice) override final;
			virtual void Reset(_In_ Device& InDevice) override final;
			void Signal(_Inout_ D3D12CommandQueue& InCommandQueue);

		private:
			uint64_t		_FenceValue			= 0ull;
			ID3D12Fence*	_Fence				= nullptr;
			void*			_FenceEvent			= nullptr;
		};
	}
}
