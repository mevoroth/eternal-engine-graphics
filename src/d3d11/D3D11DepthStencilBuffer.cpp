#include "d3d11/D3D11DepthStencilBuffer.hpp"

#include <d3d11.h>

#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11Context.hpp"

using namespace Eternal::Graphics;

D3D11DepthStencilBuffer::D3D11DepthStencilBuffer(uint32_t Width, uint32_t Height)
	: D3D11Texture()
{
	ID3D11Texture2D* Texture2D;
	HRESULT hr;

	SetWidth(Width);
	SetHeight(Height);

	D3D11_TEXTURE2D_DESC Tex2DDesc;
	Tex2DDesc.Width = Width;
	Tex2DDesc.Height = Height;
	Tex2DDesc.MipLevels = 1;
	Tex2DDesc.ArraySize = 1;
	Tex2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	Tex2DDesc.SampleDesc.Count = 1;
	Tex2DDesc.SampleDesc.Quality = 0;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	Tex2DDesc.CPUAccessFlags = 0;
	Tex2DDesc.MiscFlags = 0;

	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateTexture2D(&Tex2DDesc, 0, &Texture2D);
	ETERNAL_ASSERT(hr == S_OK);

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Flags = 0;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateDepthStencilView(Texture2D, &DepthStencilViewDesc, &_DepthStencilView);
	ETERNAL_ASSERT(hr == S_OK);

	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;

	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateShaderResourceView(Texture2D, &ShaderResourceViewDesc, &_ShaderResourceView);
	ETERNAL_ASSERT(hr == S_OK);

	_SetD3D11Resource(Texture2D);
}

D3D11DepthStencilBuffer::~D3D11DepthStencilBuffer()
{
	_DepthStencilView->Release();
	_DepthStencilView = nullptr;
}

void D3D11DepthStencilBuffer::Clear(Context* ContextObj)
{
	ETERNAL_ASSERT(_DepthStencilView);
	static_cast<D3D11Context*>(ContextObj)->GetD3D11Context()->ClearDepthStencilView(_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
