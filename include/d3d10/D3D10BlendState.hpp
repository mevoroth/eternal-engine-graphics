#ifndef _D3D10_BLEND_STATE_HPP_
#define _D3D10_BLEND_STATE_HPP_

#include <d3d10.h>

#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D10BlendState : public BlendState
		{
		private:
			D3D10_BLEND_DESC _blendDesc;
			ID3D10BlendState* _blendState;
			FLOAT _color[4];
		public:
			D3D10BlendState(_In_ const Blend& src, _In_ const Blend& dest, _In_ const BlendOp& blendOp, _In_ const Blend& srcAlpha, _In_ const Blend& destAlpha, _In_ const BlendOp& blendAlphaOp);
			virtual void Apply();
		};
	}
}

#endif
