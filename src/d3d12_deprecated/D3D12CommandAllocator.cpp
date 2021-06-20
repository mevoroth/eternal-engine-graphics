#include "d3d12/D3D12CommandAllocator.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandList.hpp"

using namespace Eternal::Graphics;

D3D12CommandAllocator::D3D12CommandAllocator(_In_ Device& DeviceObj, _In_ const CommandListType& Type)
{
	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPES[Type], __uuidof(ID3D12CommandAllocator), (void**)&_CommandAllocator);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D12CommandAllocator::~D3D12CommandAllocator()
{
	_CommandAllocator->Release();
	_CommandAllocator = nullptr;
}

void D3D12CommandAllocator::Reset()
{
	HRESULT hr = _CommandAllocator->Reset();
	ETERNAL_ASSERT(hr == S_OK);
}
