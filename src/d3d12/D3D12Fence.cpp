#include "d3d12/D3D12Fence.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

D3D12Fence::D3D12Fence(_In_ Device& InDevice)
	: _PreviousFenceValue(0ull)
	, _FenceValue(0ull)
{
	HRESULT hr = static_cast<D3D12Device&>(InDevice).GetD3D12Device()->CreateFence(_FenceValue, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&_Fence);
	ETERNAL_ASSERT(hr == S_OK);

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
}

void D3D12Fence::Reset(_In_ Device& InDevice)
{
}
