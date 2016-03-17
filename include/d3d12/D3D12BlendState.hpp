#ifndef _D3D12_BLEND_STATE_HPP_
#define _D3D12_BLEND_STATE_HPP_

#include "Graphics/BlendState.hpp"

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class D3D12BlendState : public BlendState
		{
		public:
			D3D12_BLEND GetD3D12Src() const;
			D3D12_BLEND GetD3D12Dest() const;
			D3D12_BLEND_OP GetD3D12BlendOp() const;
			D3D12_BLEND GetD3D12SrcAlpha() const;
			D3D12_BLEND GetD3D12DestAlpha() const;
			D3D12_BLEND_OP GetD3D12BlendAlphaOp() const;
		};
	}
}

#endif
