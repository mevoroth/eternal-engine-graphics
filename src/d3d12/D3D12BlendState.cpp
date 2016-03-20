#include "d3d12/D3D12BlendState.hpp"

using namespace Eternal::Graphics;

static D3D12_BLEND BLENDS[] =
{
	D3D12_BLEND_ZERO,
	D3D12_BLEND_ONE,
	D3D12_BLEND_SRC_COLOR,
	D3D12_BLEND_INV_SRC_COLOR,
	D3D12_BLEND_SRC_ALPHA,
	D3D12_BLEND_INV_SRC_ALPHA,
	D3D12_BLEND_DEST_ALPHA,
	D3D12_BLEND_INV_DEST_ALPHA,
	D3D12_BLEND_DEST_COLOR,
	D3D12_BLEND_INV_DEST_COLOR,
	D3D12_BLEND_SRC_ALPHA_SAT,
	D3D12_BLEND_BLEND_FACTOR,
	D3D12_BLEND_INV_BLEND_FACTOR
};

D3D12BlendState::D3D12BlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendOpCol, _In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp)
	: BlendState(Src, Dest, BlendOpCol, SrcAlpha, DestAlpha, BlendAlphaOp)
{
}

D3D12_BLEND D3D12BlendState::GetD3D12Src() const
{
	return BLENDS[GetSrc()];
}

D3D12_BLEND D3D12BlendState::GetD3D12Dest() const
{
	return BLENDS[GetDest()];
}

D3D12_BLEND_OP D3D12BlendState::GetD3D12BlendOp() const
{
	return (D3D12_BLEND_OP)(GetBlendOp() + 1);
}

D3D12_BLEND D3D12BlendState::GetD3D12SrcAlpha() const
{
	return BLENDS[GetSrcAlpha()];
}

D3D12_BLEND D3D12BlendState::GetD3D12DestAlpha() const
{
	return BLENDS[GetDestAlpha()];
}

D3D12_BLEND_OP D3D12BlendState::GetD3D12BlendAlphaOp() const
{
	return (D3D12_BLEND_OP)(GetBlendOp() + 1);
}
