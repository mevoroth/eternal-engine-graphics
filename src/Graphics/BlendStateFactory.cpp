#include "Graphics/BlendStateFactory.hpp"

#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		BlendState* CreateBlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendColorOp,
			_In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp,
			_In_ const BlendChannel& BlendChannelObj)
		{
			return new BlendState(Src, Dest, BlendColorOp, SrcAlpha, DestAlpha, BlendAlphaOp, BlendChannelObj);
		}
	}
}
