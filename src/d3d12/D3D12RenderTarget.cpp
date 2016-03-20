#include "d3d12/D3D12RenderTarget.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>

#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

D3D12RenderTarget::D3D12RenderTarget(D3D12Device& Device)
{
	ID3D12Device* D3D12Device = Device.GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;

	DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = D3D12Device->CreateDescriptorHeap(&DescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&GetDescriptorHeap());
	ETERNAL_ASSERT(hr == S_OK);

	hr = Device.GetSwapChain()->GetBuffer(0, __uuidof(ID3D12Resource), (void**)GetResource());
	ETERNAL_ASSERT(hr == S_OK);

	D3D12Device->CreateRenderTargetView(GetResource(), nullptr, GetCpuDescriptor());
}
