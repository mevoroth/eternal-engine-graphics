#include "d3d11/D3D11BlendState.hpp"

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

static D3D11_BLEND BLENDS[] =
{
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR,
	D3D11_BLEND_SRC_ALPHA_SAT,
	D3D11_BLEND_BLEND_FACTOR,
	D3D11_BLEND_INV_BLEND_FACTOR
};

static D3D11_BLEND_OP BLENDOPS[] =
{
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};

D3D11BlendState::D3D11BlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendOpCol, _In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp)
	: BlendState(Src, Dest, BlendOpCol, SrcAlpha, DestAlpha, BlendAlphaOp)
	, _BlendState(0)
{
	_Color[0] = 1.f; // R
	_Color[1] = 1.f; // G
	_Color[2] = 1.f; // B
	_Color[3] = 1.f; // A

	_BlendDesc.AlphaToCoverageEnable = FALSE;
	_BlendDesc.IndependentBlendEnable = FALSE;
	_BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	_BlendDesc.RenderTarget[0].SrcBlend = BLENDS[Src];
	_BlendDesc.RenderTarget[0].DestBlend = BLENDS[Dest];
	_BlendDesc.RenderTarget[0].BlendOp = BLENDOPS[BlendOpCol];
	_BlendDesc.RenderTarget[0].SrcBlendAlpha = BLENDS[SrcAlpha];
	_BlendDesc.RenderTarget[0].DestBlendAlpha = BLENDS[DestAlpha];
	_BlendDesc.RenderTarget[0].BlendOpAlpha = BLENDOPS[BlendAlphaOp];
	_BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateBlendState(&_BlendDesc, &_BlendState);
	assert(hr == S_OK);
}
void D3D11BlendState::Apply(Context* DrawContext)
{
	static_cast<D3D11Context*>(DrawContext)->GetD3D11Context()->OMSetBlendState(_BlendState, _Color, 0xffffffff);
}
