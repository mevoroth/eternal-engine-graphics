#include "d3d12/D3D12StructuredBuffer.hpp"

#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"

using namespace Eternal::Graphics;

D3D12StructuredBuffer::D3D12StructuredBuffer(_In_ D3D12Device& DeviceObj, _In_ uint32_t Stride, _In_ uint32_t Count)
{
	D3D12_HEAP_PROPERTIES HeapProperties;
	HeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L1;
	HeapProperties.CreationNodeMask = 0;
	HeapProperties.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = Stride * Count;
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	HRESULT hr = DeviceObj.GetDevice()->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_COPY_SOURCE,
		nullptr,
		__uuidof(ID3D12Resource),
		(void**)&GetResource()
	);
	ETERNAL_ASSERT(hr == S_OK);
}
