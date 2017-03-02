#include "d3d12/D3D12CommandAllocator.hpp"

#include "Macros/Macros.hpp"
#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

D3D12CommandAllocator::D3D12CommandAllocator(_In_ D3D12Device& DeviceObj, _In_ const D3D12_COMMAND_LIST_TYPE& CommandListType)
{
	HRESULT hr = DeviceObj.GetD3D12Device()->CreateCommandAllocator(CommandListType, __uuidof(ID3D12CommandAllocator), (void**)&_CommandAllocator);
	ETERNAL_ASSERT(hr == S_OK);
}

void D3D12CommandAllocator::Reset()
{
	HRESULT hr = _CommandAllocator->Reset();
	ETERNAL_ASSERT(hr == S_OK);
}
