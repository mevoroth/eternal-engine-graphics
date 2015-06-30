#include "d3d11/D3D11RenderTarget.hpp"

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;
using namespace DirectX;

D3D11RenderTarget::D3D11RenderTarget(_In_ ID3D11Texture2D* Tex)
	: D3D11Texture(Tex)
{
	D3D11_TEXTURE2D_DESC desc;
	Tex->GetDesc(&desc);
	SetWidth(desc.Width);
	SetHeight(desc.Height);

	static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateRenderTargetView(Tex, nullptr, &_RenderTarget);
}
D3D11RenderTarget::D3D11RenderTarget(_In_ int Width, _In_ int Height)
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
	Tex2DDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	Tex2DDesc.SampleDesc.Count = 1;
	Tex2DDesc.SampleDesc.Quality = 0;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	Tex2DDesc.CPUAccessFlags = 0;
	Tex2DDesc.MiscFlags = 0;

	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateTexture2D(&Tex2DDesc, 0, &Texture2D);
	ETERNAL_ASSERT(hr == S_OK);

	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetDesc;
	RenderTargetDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	RenderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetDesc.Texture2D.MipSlice = 0;

	static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateRenderTargetView(Texture2D, &RenderTargetDesc, &_RenderTarget);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D11RenderTarget::~D3D11RenderTarget()
{
	_RenderTarget->Release();
	_RenderTarget = nullptr;
}

void D3D11RenderTarget::Clear(Context* ContextObj)
{
	FLOAT ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	static_cast<D3D11Context*>(ContextObj)->GetD3D11Context()->ClearRenderTargetView(_RenderTarget, ClearColor);
}

ID3D11RenderTargetView* D3D11RenderTarget::GetD3D11RenderTarget()
{
	return _RenderTarget;
}

//ID3D11Buffer* D3D11RenderTarget::GetD3D11Buffer()
//{
//	return nullptr;
//}

ID3D11ShaderResourceView* D3D11RenderTarget::GetD3D11ShaderResourceView()
{
	return nullptr;
}

//ID3D11SamplerState* D3D11RenderTarget::CreateSamplerState()
//{
//	D3D11_SAMPLER_DESC SamplerDesc;
//	SamplerDesc.AddressU = GetD3D11UAddressMode();
//	SamplerDesc.AddressV = GetD3D11VAddressMode();
//	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // TODO: Support W mapping
//	SamplerDesc.MipLODBias = 0.f;
//	SamplerDesc.MaxAnisotropy = 16; // TODO: Support MaxAnisotropy
//	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//	//SamplerDesc.BorderColor[0] = 1.f;
//	//SamplerDesc.BorderColor[1] = 1.f;
//	//SamplerDesc.BorderColor[2] = 1.f;
//	//SamplerDesc.BorderColor[3] = 1.f;
//	SamplerDesc.MinLOD = 0;
//	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	ID3D11SamplerState* SamplerState = 0;
//
//	static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);
//
//	return SamplerState;
//}
//
//ID3D11ShaderResourceView* D3D11RenderTarget::CreateShaderResourceView()
//{
//	D3D11_TEXTURE2D_DESC Tex2DDesc;
//	_Tex2D->GetDesc(&Tex2DDesc);
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
//	ShaderResourceViewDesc.Format = Tex2DDesc.Format;
//	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//	ShaderResourceViewDesc.Texture2D.MipLevels = -1;
//	
//	ID3D11ShaderResourceView* ShaderResourceView = 0;
//
//	static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateShaderResourceView(_Tex2D, &ShaderResourceViewDesc, &ShaderResourceView);
//
//	return ShaderResourceView;
//}
//
//ID3D11UnorderedAccessView* D3D11RenderTarget::CreateUnorderedAccessView()
//{
//	D3D11_TEXTURE2D_DESC Tex2DDesc;
//	_Tex2D->GetDesc(&Tex2DDesc);
//	
//	D3D11_UNORDERED_ACCESS_VIEW_DESC UnorderedAccessViewDesc;
//	UnorderedAccessViewDesc.Format = Tex2DDesc.Format;
//	UnorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
//	UnorderedAccessViewDesc.Texture2D.MipSlice = 0;
//
//	ID3D11UnorderedAccessView* UnorderedAccessView;
//
//	static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateUnorderedAccessView(_Tex2D, &UnorderedAccessViewDesc, &UnorderedAccessView);
//
//	return UnorderedAccessView;
//}
