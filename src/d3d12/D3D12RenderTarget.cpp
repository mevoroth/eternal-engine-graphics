#include "d3d12/D3D12RenderTarget.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>

#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12SwapChain.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"

using namespace Eternal::Graphics;

D3D12RenderTarget::D3D12RenderTarget(_In_ D3D12Device& Device, _In_ D3D12DescriptorHeap& DescriptorHeap, _In_ uint32_t BackBufferFrameIndex)
{
	//ID3D12Device* D3D12Device = Device.GetD3D12Device();

	//HRESULT hr = Device.GetSwapChain()->GetBuffer(BackBufferFrameIndex, __uuidof(ID3D12Resource), (void**)&GetResource());
	//ETERNAL_ASSERT(hr == S_OK);
	//GetCpuDescriptor() = DescriptorHeap.Reserve(BackBufferFrameIndex);
	//D3D12Device->CreateRenderTargetView(GetResource(), nullptr, GetCpuDescriptor());
}
