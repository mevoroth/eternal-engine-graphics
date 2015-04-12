#include "d3d11/D3D11RenderTarget.hpp"

#include <DirectXMath.h>
#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;
using namespace DirectX;

D3D11RenderTarget::D3D11RenderTarget(_In_ ID3D11Texture2D* tex)
	: _Tex2D(tex)
{
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	SetWidth(desc.Width);
	SetHeight(desc.Height);

	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateRenderTargetView(_Tex2D, 0, &_RenderTarget);
}
D3D11RenderTarget::D3D11RenderTarget(_In_ int width, _In_ int height)
	: _RenderTarget(0)
	, _Tex2D(0)
{
	SetWidth(width);
	SetHeight(height);
	D3D11_TEXTURE2D_DESC Tex2DDesc;
	Tex2DDesc.Width = width;
	Tex2DDesc.Height = height;
	Tex2DDesc.MipLevels = 1;
	Tex2DDesc.ArraySize = 1;
	Tex2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	Tex2DDesc.SampleDesc.Count = 1;
	Tex2DDesc.SampleDesc.Quality = 0;
	Tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	Tex2DDesc.CPUAccessFlags = 0;
	Tex2DDesc.MiscFlags = 0;

	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateTexture2D(&Tex2DDesc, 0, &_Tex2D);

	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetDesc;
	RenderTargetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	RenderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetDesc.Texture2D.MipSlice = 1;

	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateRenderTargetView(_Tex2D, &RenderTargetDesc, &_RenderTarget);
}

D3D11RenderTarget::~D3D11RenderTarget()
{
	_Tex2D->Release();
	_Tex2D = 0;
	_RenderTarget->Release();
	_RenderTarget = 0;
}

void D3D11RenderTarget::Clear()
{
	FLOAT clear[4] = { 0.f, 0.f, 0.f, 1.f };
	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDeviceContext()->ClearRenderTargetView(_RenderTarget, clear);
}

ID3D11RenderTargetView* D3D11RenderTarget::GetD3D11RenderTarget()
{
	return _RenderTarget;
}

ID3D11SamplerState* D3D11RenderTarget::CreateSamplerState()
{
	D3D11_SAMPLER_DESC SamplerDesc;
	SamplerDesc.AddressU = GetD3D11UAddressMode();
	SamplerDesc.AddressV = GetD3D11VAddressMode();
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // TODO: Support W mapping
	SamplerDesc.MipLODBias = 0.f;
	SamplerDesc.MaxAnisotropy = 16; // TODO: Support MaxAnisotropy
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//SamplerDesc.BorderColor[0] = 1.f;
	//SamplerDesc.BorderColor[1] = 1.f;
	//SamplerDesc.BorderColor[2] = 1.f;
	//SamplerDesc.BorderColor[3] = 1.f;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* SamplerState = 0;

	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateSamplerState(&SamplerDesc, &SamplerState);

	return SamplerState;
}

ID3D11ShaderResourceView* D3D11RenderTarget::CreateShaderResourceView()
{
	D3D11_TEXTURE2D_DESC Tex2DDesc;
	_Tex2D->GetDesc(&Tex2DDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format = Tex2DDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;
	
	ID3D11ShaderResourceView* ShaderResourceView = 0;

	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateShaderResourceView(_Tex2D, &ShaderResourceViewDesc, &ShaderResourceView);

	return ShaderResourceView;
}

