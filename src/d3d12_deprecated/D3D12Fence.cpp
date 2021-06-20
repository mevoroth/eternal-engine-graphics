#include "d3d12_deprecated/D3D12Fence.hpp"

#include <d3d12.h>
#include "d3d12_deprecated/D3D12Device.hpp"
#include "d3d12_deprecated/D3D12CommandQueue.hpp"
#include <cstdio>
using namespace Eternal::Graphics;

D3D12Fence::D3D12Fence(_In_ Device& DeviceObj)
	: _PreviousFenceValue(0ull)
	, _FenceValue(0ull)
{
	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateFence(_FenceValue, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&_Fence);
	ETERNAL_ASSERT(hr == S_OK);

	_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (!_FenceEvent)
	{
		DWORD LastError = GetLastError();
		ETERNAL_ASSERT(LastError != ERROR_ALREADY_EXISTS);
		ETERNAL_BREAK();
		return;
	}
}

D3D12Fence::~D3D12Fence()
{
	CloseHandle(_FenceEvent);
	_FenceEvent = nullptr;

	_Fence->Release();
	_Fence = nullptr;
}

void D3D12Fence::Signal(_In_ CommandQueue& CommandQueueObj)
{
	_PreviousFenceValue = _FenceValue;
	HRESULT hr = static_cast<D3D12CommandQueue&>(CommandQueueObj).GetD3D12CommandQueue()->Signal(_Fence, _FenceValue);
	ETERNAL_ASSERT(hr == S_OK);
}

void D3D12Fence::Wait(_In_ Device& DeviceObj)
{
	UINT64 Completed = _Fence->GetCompletedValue();
	//char variable[256];
	//sprintf_s(variable, sizeof(char) * 256, "Completed value: %d; i: %d; FenceValue0: %d; FenceValue1: %d\n", Completed, _CurrentFence, _FenceValues[_CurrentFence], _FenceValues[_CurrentFence ^ 1]);
	//OutputDebugString(variable);
	if (Completed <= _FenceValue)
	{
		//OutputDebugString("wait\n");
		HRESULT hr = _Fence->SetEventOnCompletion(_FenceValue, _FenceEvent);
		ETERNAL_ASSERT(hr == S_OK);
		DWORD WaitResult = WaitForSingleObjectEx(_FenceEvent, INFINITE, FALSE);
		ETERNAL_ASSERT(WaitResult != WAIT_FAILED);
	}
	_FenceValue = _PreviousFenceValue + 1;
}

void D3D12Fence::Reset(_In_ Device& DeviceObj)
{
}
