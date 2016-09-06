#include "Graphics/BlendStateFactory.hpp"

#include "d3d11/D3D11BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		BlendState* CreateBlendState(_In_ const BlendState::Blend& Src, _In_ const BlendState::Blend& Dest, _In_ const BlendState::BlendOp& BlendOpCol,
			_In_ const BlendState::Blend& SrcAlpha, _In_ const BlendState::Blend& DestAlpha, _In_ const BlendState::BlendOp& BlendAlphaOp)
		{
			return new D3D11BlendState(Src, Dest, BlendOpCol, SrcAlpha, DestAlpha, BlendAlphaOp);
		}
	}
}
