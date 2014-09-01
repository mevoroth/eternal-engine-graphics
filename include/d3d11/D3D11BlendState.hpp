#ifndef _D3D11_BLEND_STATE_HPP_
#define _D3D11_BLEND_STATE_HPP_

#include <d3d11.h>

#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11BlendState : public BlendState
		{
		private:
			D3D11_BLEND_DESC _blendDesc;
			ID3D11BlendState* _blendState;
			FLOAT _color[4];
		public:
			D3D11BlendState(_In_ const Blend& src, _In_ const Blend& dest, _In_ const BlendOp& blendOp, _In_ const Blend& srcAlpha, _In_ const Blend& destAlpha, _In_ const Blend& blendAlphaOp);
			virtual void Apply();
		};
	}
}

#endif
