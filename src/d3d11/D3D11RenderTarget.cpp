#include "d3d11/D3D11RenderTarget.hpp"

#include <d3d11.h>
#include "Macros/Macros.hpp"
#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11Context.hpp"

using namespace Eternal::Graphics;

D3D11RenderTarget::D3D11RenderTarget(_In_ ID3D11Texture2D* Tex)
	: D3D11Texture(Tex)
{
	D3D11_TEXTURE2D_DESC desc;
	Tex->GetDesc(&desc);
	SetWidth(desc.Width);
	SetHeight(desc.Height);

	static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateRenderTargetView(Tex, nullptr, &_RenderTarget);
}
D3D11RenderTarget::D3D11RenderTarget(_In_ const Format& FormatObj, _In_ uint32_t Width, _In_ uint32_t Height)
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
	Tex2DDesc.Format = D3D11Texture::GetD3D11TextureFormatInformation(FormatObj).D3D11Format;
	Tex2DDesc.SampleDesc.Count = 1;
	Tex2DDesc.SampleDesc.Quality = 0;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Tex2DDesc.CPUAccessFlags = 0;
	Tex2DDesc.MiscFlags = 0;

	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateTexture2D(&Tex2DDesc, 0, &Texture2D);
	ETERNAL_ASSERT(hr == S_OK);

	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetDesc;
	RenderTargetDesc.Format = D3D11Texture::GetD3D11TextureFormatInformation(FormatObj).D3D11Format;
	RenderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetDesc.Texture2D.MipSlice = 0;

	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateRenderTargetView(Texture2D, &RenderTargetDesc, &_RenderTarget);
	ETERNAL_ASSERT(hr == S_OK);

	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = D3D11Texture::GetD3D11TextureFormatInformation(FormatObj).D3D11Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;
	
	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateShaderResourceView(Texture2D, &ShaderResourceViewDesc, &_ShaderResourceView);
	ETERNAL_ASSERT(hr == S_OK);

	_SetD3D11Resource(Texture2D);
}

D3D11RenderTarget::~D3D11RenderTarget()
{
	_RenderTarget->Release();
	_RenderTarget = nullptr;
}

void D3D11RenderTarget::Clear(_In_ Context* ContextObj)
{
	FLOAT ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	static_cast<D3D11Context*>(ContextObj)->GetD3D11Context()->ClearRenderTargetView(_RenderTarget, ClearColor);
}

void D3D11RenderTarget::Clear(_In_ Context* ContextObj, _In_ float Value)
{
	FLOAT ClearColor[4] = { Value, Value, Value, 1.f };
	static_cast<D3D11Context*>(ContextObj)->GetD3D11Context()->ClearRenderTargetView(_RenderTarget, ClearColor);
}

ID3D11RenderTargetView* D3D11RenderTarget::GetD3D11RenderTarget()
{
	return _RenderTarget;
}

Resource* D3D11RenderTarget::GetAsResource()
{
	return this;
}
