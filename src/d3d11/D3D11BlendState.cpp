#include "d3d11/D3D11BlendState.hpp"

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
	D3D11_BLEND_SRC_ALPHA_SAT
};

static D3D11_BLEND_OP BLENDOPS[] =
{
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};

D3D11BlendState::D3D11BlendState(_In_ const Blend& src, _In_ const Blend& dest, _In_ const BlendOp& blendOp, _In_ const Blend& srcAlpha, _In_ const Blend& destAlpha, _In_ const Blend& blendAlphaOp)
	: BlendState(src, dest, blendOp, srcAlpha, destAlpha, blendAlphaOp)
{
	_blendDesc.AlphaToCoverageEnable = FALSE;
	_blendDesc.IndependentBlendEnable = FALSE;
	_blendDesc.RenderTarget[0].BlendEnable = TRUE;
	_blendDesc.RenderTarget[0].SrcBlend = BLENDS[src];
	_blendDesc.RenderTarget[0].DestBlend = BLENDS[dest];
	_blendDesc.RenderTarget[0].BlendOp = BLENDOPS[blendOp];
	_blendDesc.RenderTarget[0].SrcBlendAlpha = BLENDS[srcAlpha];
	_blendDesc.RenderTarget[0].DestBlendAlpha = BLENDS[destAlpha];
	_blendDesc.RenderTarget[0].BlendOpAlpha = BLENDOPS[blendAlphaOp];
	_blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}
void D3D11BlendState::Apply()
{

}
