#ifndef _D3D11_BLEND_STATE_HPP_
#define _D3D11_BLEND_STATE_HPP_

#include <d3d11.h>

#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class D3D11BlendState : public BlendState
		{
		private:
			D3D11_BLEND_DESC _BlendDesc;
			ID3D11BlendState* _BlendState;
			FLOAT _Color[4];
		public:
			D3D11BlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendOpCol, _In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp);
			virtual void Apply(Context* DrawContext) override;
		};
	}
}

#endif
