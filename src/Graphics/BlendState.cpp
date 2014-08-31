#include "Graphics/BlendState.hpp"

using namespace Eternal::Graphics;

BlendState::BlendState(_In_ const Blend& src, _In_ const Blend& dest, _In_ const BlendOp& blendOp, _In_ const Blend& srcAlpha, _In_ const Blend& destAlpha, _In_ const Blend& blendAlphaOp)
	: _src(src)
	, _dest(dest)
	, _blendOp(blendOp)
	, _srcAlpha(srcAlpha)
	, _destAlpha(destAlpha)
	, _blendAlphaOp(blendAlphaOp)
{

}
