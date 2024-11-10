#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12Fence.hpp"

#include "d3d12/D3D12Library.h"
#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12Fence::D3D12Fence(_In_ Device& InDevice)
			: _FenceValue(InvalidFenceValue)
		{
			D3D12::VerifySuccess(
				static_cast<D3D12Device&>(InDevice).GetD3D12Device()->CreateFence(
					_FenceValue,
					D3D12_FENCE_FLAG_NONE,
					__uuidof(ID3D12Fence),
					reinterpret_cast<void**>(&_Fence)
				)
			);

			_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (!_FenceEvent)
			{
				DWORD LastError = GetLastError();
				ETERNAL_ASSERT(LastError != ERROR_ALREADY_EXISTS);
				ETERNAL_BREAK();
			}
		}

		D3D12Fence::~D3D12Fence()
		{
			CloseHandle(_FenceEvent);
			_FenceEvent = nullptr;

			_Fence->Release();
			_Fence = nullptr;
		}

		void D3D12Fence::Wait(_In_ Device& InDevice)
		{
			if (_FenceValue != InvalidFenceValue && _Fence->GetCompletedValue() != _FenceValue)
			{
				D3D12::VerifySuccess(
					_Fence->SetEventOnCompletion(_FenceValue, _FenceEvent)
				);
				ETERNAL_ASSERT(WaitForSingleObjectEx(_FenceEvent, INFINITE, FALSE) == WAIT_OBJECT_0);
			}
		}

		void D3D12Fence::Reset(_In_ Device& InDevice)
		{
		}

		void D3D12Fence::Signal(_Inout_ D3D12CommandQueue& InCommandQueue)
		{
			if (++_FenceValue == InvalidFenceValue)
				++_FenceValue;

			D3D12::VerifySuccess(
				InCommandQueue.GetD3D12CommandQueue()->Signal(
					_Fence,
					_FenceValue
				)
			);
		}
	}
}

#endif
