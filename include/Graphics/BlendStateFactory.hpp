#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class Blend;
		enum class BlendOp;
		enum class BlendChannel;
		class BlendState;

		BlendState* CreateBlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendColorOp,
			_In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp,
			_In_ const BlendChannel& BlendChannelObj);
	}
}
