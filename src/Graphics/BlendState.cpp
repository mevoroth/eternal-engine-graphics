#include "Graphics/BlendState.hpp"

using namespace Eternal::Graphics;

namespace Eternal
{
	namespace Graphics
	{
		const LogicBlend LogicBlendNone;
		const BlendState BlendStateNone;
		const BlendState BlendStateAlphaBlend(
			Blend::BLEND_SRC_ALPHA,
			Blend::BLEND_INV_SRC_ALPHA,
			BlendOp::BLEND_OP_ADD,
			Blend::BLEND_SRC_ALPHA,
			Blend::BLEND_INV_SRC_ALPHA,
			BlendOp::BLEND_OP_ADD,
			BlendChannel::BLEND_CHANNEL_ALL
		);
	}
}

BlendState::BlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendColorOp, _In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp, _In_ const BlendChannel& BlendChannelObj)
	: _Enabled(true)
	, _Src(Src)
	, _Dest(Dest)
	, _BlendOp(BlendColorOp)
	, _SrcAlpha(SrcAlpha)
	, _DestAlpha(DestAlpha)
	, _BlendAlphaOp(BlendAlphaOp)
	, _BlendChannel(BlendChannelObj)
{
}

LogicBlend::LogicBlend(_In_ const LogicOp& LogicOpObj)
	: _Enabled(true)
	, _LogicOp(LogicOpObj)
{
}
