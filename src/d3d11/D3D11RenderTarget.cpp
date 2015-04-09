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

ID3D11SamplerState* Graphics::D3D11RenderTarget::CreateSamplerState()
{

}

