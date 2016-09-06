#ifndef _BLEND_STATE_FACTORY_HPP_
#define _BLEND_STATE_FACTORY_HPP_

#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		BlendState* CreateBlendState(_In_ const BlendState::Blend& Src, _In_ const BlendState::Blend& Dest, _In_ const BlendState::BlendOp& BlendOpCol,
			_In_ const BlendState::Blend& SrcAlpha, _In_ const BlendState::Blend& DestAlpha, _In_ const BlendState::BlendOp& BlendAlphaOp);
	}
}

#endif
