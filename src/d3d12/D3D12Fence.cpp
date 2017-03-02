#include "d3d12/D3D12Fence.hpp"

#include <d3d12.h>
#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include <cstdio>
using namespace Eternal::Graphics;

D3D12Fence::D3D12Fence(_In_ D3D12Device& DeviceObj, _In_ uint32_t SimultaneousResourcesCount)
{
	_FenceValues.resize(SimultaneousResourcesCount);

	for (uint32_t FenceValueIndex = 0; FenceValueIndex < SimultaneousResourcesCount; ++FenceValueIndex)
	{
		_FenceValues[FenceValueIndex] = 0ull;
	}

	HRESULT hr = DeviceObj.GetD3D12Device()->CreateFence(_FenceValues[0], D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&_Fence);
	ETERNAL_ASSERT(hr == S_OK);

	_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (!_FenceEvent)
	{
		DWORD LastError = GetLastError();
		ETERNAL_ASSERT(LastError != ERROR_ALREADY_EXISTS);
		ETERNAL_ASSERT(false);
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

void D3D12Fence::Signal(_In_ D3D12CommandQueue& CommandQueueObj)
{
	HRESULT hr = CommandQueueObj.GetD3D12CommandQueue()->Signal(_Fence, _FenceValues[_CurrentFence]);
	ETERNAL_ASSERT(hr == S_OK);
	_CurrentFence = (_CurrentFence + 1) % _FenceValues.size();
}

void D3D12Fence::Wait()
{
	UINT64 completed = _Fence->GetCompletedValue();
	//char variable[256];
	//sprintf_s(variable, sizeof(char) * 256, "Completed value: %d; i: %d; FenceValue0: %d; FenceValue1: %d\n", completed, _CurrentFence, _FenceValues[_CurrentFence], _FenceValues[_CurrentFence ^ 1]);
	if (completed < _FenceValues[_CurrentFence])
	{
		//OutputDebugString("wait\n");
		HRESULT hr = _Fence->SetEventOnCompletion(_FenceValues[_CurrentFence], _FenceEvent);
		ETERNAL_ASSERT(hr == S_OK);
		DWORD WaitResult = WaitForSingleObjectEx(_FenceEvent, INFINITE, FALSE);
		ETERNAL_ASSERT(WaitResult != WAIT_FAILED);
	}
	//OutputDebugString(variable);
	_FenceValues[_CurrentFence] = _FenceValues[(_CurrentFence + _FenceValues.size() - 1) % _FenceValues.size()] + 1ull;
}
