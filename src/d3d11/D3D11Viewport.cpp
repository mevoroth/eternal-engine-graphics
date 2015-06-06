#include "d3d11/D3D11Viewport.hpp"

#include <d3d11.h>

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11Viewport::D3D11Viewport(_In_ int x, _In_ int y, _In_ int width, _In_ int height)
	: Viewport(x, y, width, height)
{
}

void D3D11Viewport::Apply(Context* DrawContext)
{
	D3D11_VIEWPORT ViewportDesc;
	
	ViewportDesc.TopLeftX = X();
	ViewportDesc.TopLeftY = Y();
	ViewportDesc.Height = Height();
	ViewportDesc.Width = Width();
	ViewportDesc.MinDepth = 0.f;
	ViewportDesc.MaxDepth = 1.f;

	static_cast<D3D11Context*>(DrawContext)->GetD3D11Context()->RSSetViewports(1, &ViewportDesc);
}
