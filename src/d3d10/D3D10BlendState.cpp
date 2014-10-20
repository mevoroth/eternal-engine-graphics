#include "d3d10/D3D10BlendState.hpp"

#include "d3d10/D3D10Renderer.hpp"

using namespace Eternal::Graphics;

static D3D10_BLEND BLENDS[] =
{
	D3D10_BLEND_ZERO,
	D3D10_BLEND_ONE,
	D3D10_BLEND_SRC_COLOR,
	D3D10_BLEND_INV_SRC_COLOR,
	D3D10_BLEND_SRC_ALPHA,
	D3D10_BLEND_INV_SRC_ALPHA,
	D3D10_BLEND_DEST_ALPHA,
	D3D10_BLEND_INV_DEST_ALPHA,
	D3D10_BLEND_DEST_COLOR,
	D3D10_BLEND_INV_DEST_COLOR,
	D3D10_BLEND_SRC_ALPHA_SAT,
	D3D10_BLEND_BLEND_FACTOR,
	D3D10_BLEND_INV_BLEND_FACTOR
};

static D3D10_BLEND_OP BLENDOPS[] =
{
	D3D10_BLEND_OP_ADD,
	D3D10_BLEND_OP_SUBTRACT,
	D3D10_BLEND_OP_REV_SUBTRACT,
	D3D10_BLEND_OP_MIN,
	D3D10_BLEND_OP_MAX
};

D3D10BlendState::D3D10BlendState(_In_ const Blend& src, _In_ const Blend& dest, _In_ const BlendOp& blendOp, _In_ const Blend& srcAlpha, _In_ const Blend& destAlpha, _In_ const BlendOp& blendAlphaOp)
	: BlendState(src, dest, blendOp, srcAlpha, destAlpha, blendAlphaOp)
	, _blendState(0)
{
	_color[0] = 1.f; // R
	_color[1] = 1.f; // G
	_color[2] = 1.f; // B
	_color[3] = 1.f; // A

	_blendDesc.AlphaToCoverageEnable = FALSE;
	_blendDesc.BlendEnable[0] = TRUE;
	_blendDesc.BlendEnable[1] = FALSE;
	_blendDesc.BlendEnable[2] = FALSE;
	_blendDesc.BlendEnable[3] = FALSE;
	_blendDesc.BlendEnable[4] = FALSE;
	_blendDesc.BlendEnable[5] = FALSE;
	_blendDesc.BlendEnable[6] = FALSE;
	_blendDesc.BlendEnable[7] = FALSE;
	_blendDesc.SrcBlend = BLENDS[src];
	_blendDesc.DestBlend = BLENDS[dest];
	_blendDesc.BlendOp = BLENDOPS[blendOp];
	_blendDesc.SrcBlendAlpha = BLENDS[srcAlpha];
	_blendDesc.DestBlendAlpha = BLENDS[destAlpha];
	_blendDesc.BlendOpAlpha = BLENDOPS[blendAlphaOp];
	_blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	_blendDesc.RenderTargetWriteMask[1] = D3D10_COLOR_WRITE_ENABLE_ALL;
	_blendDesc.RenderTargetWriteMask[2] = D3D10_COLOR_WRITE_ENABLE_ALL;
	_blendDesc.RenderTargetWriteMask[3] = D3D10_COLOR_WRITE_ENABLE_ALL;
	_blendDesc.RenderTargetWriteMask[4] = D3D10_COLOR_WRITE_ENABLE_ALL;
	_blendDesc.RenderTargetWriteMask[5] = D3D10_COLOR_WRITE_ENABLE_ALL;
	_blendDesc.RenderTargetWriteMask[6] = D3D10_COLOR_WRITE_ENABLE_ALL;
	_blendDesc.RenderTargetWriteMask[7] = D3D10_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->CreateBlendState(&_blendDesc, &_blendState);
}
void D3D10BlendState::Apply()
{
	dynamic_cast<D3D10Renderer*>(Renderer::Get())->GetDevice()->OMSetBlendState(_blendState, _color, 0xffffffff);
}
