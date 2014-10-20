#include "d3d10/D3D10RenderTarget.hpp"

#include <DirectXMath.h>
#include "d3d10/D3D10Renderer.hpp"

using namespace Eternal::Graphics;
using namespace DirectX;

D3D10RenderTarget::D3D10RenderTarget(_In_ ID3D10Texture2D* tex)
	: _tex2D(tex)
{
	D3D10_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	SetWidth(desc.Width);
	SetHeight(desc.Height);

	dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->CreateRenderTargetView(_tex2D, 0, &_renderTarget);
}
D3D10RenderTarget::D3D10RenderTarget(_In_ int width, _In_ int height)
	: _renderTarget(0)
	, _tex2D(0)
{
	SetWidth(width);
	SetHeight(height);
	D3D10_TEXTURE2D_DESC tex2DDesc;
	tex2DDesc.Width = width;
	tex2DDesc.Height = height;
	tex2DDesc.MipLevels = 1;
	tex2DDesc.ArraySize = 1;
	tex2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	tex2DDesc.SampleDesc.Count = 1;
	tex2DDesc.SampleDesc.Quality = 0;
	tex2DDesc.Usage = D3D10_USAGE_DEFAULT;
	tex2DDesc.BindFlags = D3D10_BIND_RENDER_TARGET;
	tex2DDesc.CPUAccessFlags = 0;
	tex2DDesc.MiscFlags = 0;

	dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->CreateTexture2D(&tex2DDesc, 0, &_tex2D);

	D3D10_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	renderTargetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	renderTargetDesc.Texture2D.MipSlice = 1;

	dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->CreateRenderTargetView(_tex2D, &renderTargetDesc, &_renderTarget);
}

D3D10RenderTarget::~D3D10RenderTarget()
{
	_tex2D->Release();
	_tex2D = 0;
	_renderTarget->Release();
	_renderTarget = 0;
}

void D3D10RenderTarget::Clear()
{
	FLOAT clear[4] = { 0.f, 0.f, 0.f, 1.f };
	dynamic_cast<D3D10Renderer*>(D3D10Renderer::Get())->GetDevice()->ClearRenderTargetView(_renderTarget, clear);
}

ID3D10RenderTargetView* D3D10RenderTarget::GetD3D10RenderTarget()
{
	return _renderTarget;
}
