#include "d3d11/D3D11StructuredBuffer.hpp"

#include <d3d11.h>

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11StructuredBuffer::D3D11StructuredBuffer(_In_ const Usage& UsageObj, _In_ const CPUAccess& CPUMode, _In_ size_t Stride, _In_ uint32_t ElementsCount, _In_ const void* Data /* = nullptr */)
{
	ID3D11Buffer* Buffer = nullptr;
	
	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.ByteWidth = (UINT)(Stride * ElementsCount);
	BufferDesc.Usage = (D3D11_USAGE)UsageObj;
	BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	BufferDesc.CPUAccessFlags = CPUAccessToD3D11(CPUMode);
	BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	BufferDesc.StructureByteStride = (UINT)Stride;

	D3D11_SUBRESOURCE_DATA SubResourceData;
	if (Data)
	{
		SubResourceData.pSysMem = Data;
		SubResourceData.SysMemPitch = (UINT)Stride;
		SubResourceData.SysMemSlicePitch = 0;
	}

	HRESULT hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateBuffer(&BufferDesc, Data ? &SubResourceData : nullptr, &Buffer);
	ETERNAL_ASSERT(hr == S_OK);

	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	ShaderResourceViewDesc.Buffer.ElementOffset = 0;
	ShaderResourceViewDesc.Buffer.ElementWidth = (UINT)Stride;

	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateShaderResourceView(Buffer, &ShaderResourceViewDesc, &_ShaderResourceView);
	ETERNAL_ASSERT(hr == S_OK);

	_SetD3D11Resource(Buffer);
	_SetCPUAccess(CPUMode);
}
