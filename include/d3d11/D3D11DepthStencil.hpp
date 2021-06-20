#ifndef _D3D11_DEPTH_STENCIL_HPP_
#define _D3D11_DEPTH_STENCIL_HPP_

#include "Graphics_deprecated/DepthStencil.hpp"

struct ID3D11DepthStencilState;

namespace Eternal
{
	namespace Graphics
	{
		class DepthTest;
		class StencilTest;

		class D3D11DepthStencil : public DepthStencil
		{
		public:
			D3D11DepthStencil(_In_ const DepthTest& DepthTestState, _In_ const StencilTest& StencilState);
			~D3D11DepthStencil();
			inline ID3D11DepthStencilState* GetD3D11DepthStencilState()
			{
				return _DepthStencilState;
			}

		private:
			ID3D11DepthStencilState* _DepthStencilState = nullptr;
		};
	}
}

#endif
