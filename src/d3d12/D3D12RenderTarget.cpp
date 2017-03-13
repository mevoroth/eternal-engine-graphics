#include "d3d12/D3D12RenderTarget.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>

#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12SwapChain.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"

using namespace Eternal::Graphics;

D3D12RenderTarget::D3D12RenderTarget(_In_ Device& DeviceObj, _In_ SwapChain& SwapChainObj, _In_ D3D12DescriptorHeap& DescriptorHeap, _In_ uint32_t BackBufferFrameIndex)
{
	ID3D12Device* D3D12DeviceObj = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device();

	HRESULT hr = static_cast<D3D12SwapChain&>(SwapChainObj).GetDXGISwapChain()->GetBuffer(BackBufferFrameIndex, __uuidof(ID3D12Resource), (void**)&GetResource());
	ETERNAL_ASSERT(hr == S_OK);
	GetCpuDescriptor() = DescriptorHeap.Reserve(BackBufferFrameIndex);
	D3D12DeviceObj->CreateRenderTargetView(GetResource(), nullptr, GetCpuDescriptor());
}
