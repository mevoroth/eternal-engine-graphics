#include "d3d11/D3D11RenderTarget.hpp"

#include <DirectXMath.h>
#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;
using namespace DirectX;

D3D11RenderTarget::D3D11RenderTarget(_In_ ID3D11Texture2D* tex)
	: _tex2D(tex)
{
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	SetWidth(desc.Width);
	SetHeight(desc.Height);

	D3D11Renderer::Get()->GetDevice()->CreateRenderTargetView(_tex2D, 0, &_renderTarget);
}
D3D11RenderTarget::D3D11RenderTarget(_In_ int width, _In_ int height)
	: _renderTarget(0)
	, _tex2D(0)
{
	SetWidth(width);
	SetHeight(height);
	D3D11_TEXTURE2D_DESC tex2DDesc;
	tex2DDesc.Width = width;
	tex2DDesc.Height = height;
	tex2DDesc.MipLevels = 1;
	tex2DDesc.ArraySize = 1;
	tex2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	tex2DDesc.SampleDesc.Count = 1;
	tex2DDesc.SampleDesc.Quality = 0;
	tex2DDesc.Usage = D3D11_USAGE_DEFAULT;
	tex2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	tex2DDesc.CPUAccessFlags = 0;
	tex2DDesc.MiscFlags = 0;

	D3D11Renderer::Get()->GetDevice()->CreateTexture2D(&tex2DDesc, 0, &_tex2D);

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	renderTargetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetDesc.Texture2D.MipSlice = 1;

	D3D11Renderer::Get()->GetDevice()->CreateRenderTargetView(_tex2D, &renderTargetDesc, &_renderTarget);
}

D3D11RenderTarget::~D3D11RenderTarget()
{
	_tex2D->Release();
	_tex2D = 0;
	_renderTarget->Release();
	_renderTarget = 0;
}

void D3D11RenderTarget::Clear()
{
	FLOAT clear[4] = { 1.f, 0.f, 0.f, 1.f };
	D3D11Renderer::Get()->GetDeviceContext()->ClearRenderTargetView(_renderTarget, clear);
}
