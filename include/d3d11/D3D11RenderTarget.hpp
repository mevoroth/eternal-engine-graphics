#ifndef _D3D11_RENDER_TARGET_HPP_
#define _D3D11_RENDER_TARGET_HPP_

#include <d3d11.h>
#include "Graphics/RenderTarget.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11RenderTarget : public RenderTarget
		{
		private:
			ID3D11RenderTargetView* _renderTarget;
			ID3D11Texture2D* _tex2D;
		public:
			D3D11RenderTarget(_In_ int width, _In_ int height);
			D3D11RenderTarget(_In_ ID3D11Texture2D* tex);
			~D3D11RenderTarget();
		};
	}
}

#endif
