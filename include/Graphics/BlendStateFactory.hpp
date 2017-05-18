#ifndef _BLEND_STATE_FACTORY_HPP_
#define _BLEND_STATE_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum Blend;
		enum BlendOp;
		enum BlendChannel;
		class BlendState;

		BlendState* CreateBlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendColorOp,
			_In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp,
			_In_ const BlendChannel& BlendChannelObj);
	}
}

#endif
