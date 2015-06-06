#include "Graphics/BlendState.hpp"

using namespace Eternal::Graphics;

BlendState::BlendState(_In_ const Blend& src, _In_ const Blend& dest, _In_ const BlendOp& blendOp, _In_ const Blend& srcAlpha, _In_ const Blend& destAlpha, _In_ const BlendOp& blendAlphaOp)
	: _Src(src)
	, _Dest(dest)
	, _BlendOp(blendOp)
	, _SrcAlpha(srcAlpha)
	, _DestAlpha(destAlpha)
	, _BlendAlphaOp(blendAlphaOp)
{
}
