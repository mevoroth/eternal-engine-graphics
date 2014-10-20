#include "d3d10/D3D10Viewport.hpp"

#include <d3d10.h>

#include "d3d10/D3D10Renderer.hpp"

using namespace Eternal::Graphics;

D3D10Viewport::D3D10Viewport(_In_ int x, _In_ int y, _In_ int width, _In_ int height)
	: Viewport(x, y, width, height)
{
}

void D3D10Viewport::Apply()
{
	D3D10_VIEWPORT viewport;
	viewport.TopLeftX = X();
	viewport.TopLeftY = Y();
	viewport.Height = Height();
	viewport.Width = Width();
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->RSSetViewports(1, &viewport);
}
