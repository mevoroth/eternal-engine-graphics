#include "d3d11/D3D11Viewport.hpp"

#include <d3d11.h>

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11Viewport::D3D11Viewport(_In_ int x, _In_ int y, _In_ int width, _In_ int height)
	: Viewport(x, y, width, height)
{
}

void D3D11Viewport::Apply()
{
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = X();
	viewport.TopLeftY = Y();
	viewport.Height = Height();
	viewport.Width = Width();
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	
	dynamic_cast<D3D11Renderer*>(D3D11Renderer::Get())->GetDeviceContext()->RSSetViewports(1, &viewport);
}
