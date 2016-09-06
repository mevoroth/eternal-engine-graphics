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
{
	_Color[0] = 1.f; // R
	_Color[1] = 1.f; // G
	_Color[2] = 1.f; // B
	_Color[3] = 1.f; // A

	_BlendDesc.AlphaToCoverageEnable = FALSE;
	_BlendDesc.IndependentBlendEnable = TRUE;
	for (uint32_t renderTargetIndex = 0; renderTargetIndex < 8; ++renderTargetIndex)
	{
		_BlendDesc.RenderTarget[renderTargetIndex].BlendEnable = TRUE;
		_BlendDesc.RenderTarget[renderTargetIndex].SrcBlend = BLENDS[Src];
		_BlendDesc.RenderTarget[renderTargetIndex].DestBlend = BLENDS[Dest];
		_BlendDesc.RenderTarget[renderTargetIndex].BlendOp = BLENDOPS[BlendOpCol];
		_BlendDesc.RenderTarget[renderTargetIndex].SrcBlendAlpha = BLENDS[SrcAlpha];
		_BlendDesc.RenderTarget[renderTargetIndex].DestBlendAlpha = BLENDS[DestAlpha];
		_BlendDesc.RenderTarget[renderTargetIndex].BlendOpAlpha = BLENDOPS[BlendAlphaOp];
		_BlendDesc.RenderTarget[renderTargetIndex].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	HRESULT hr = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateBlendState(&_BlendDesc, &_BlendState);
	ETERNAL_ASSERT(hr == S_OK);
}
void D3D11BlendState::Apply(Context* DrawContext)
{
	static_cast<D3D11Context*>(DrawContext)->GetD3D11Context()->OMSetBlendState(_BlendState, _Color, 0xffffffff);
}
