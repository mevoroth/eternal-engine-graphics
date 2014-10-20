#ifndef _D3D10_RENDER_TARGET_HPP_
#define _D3D10_RENDER_TARGET_HPP_

#include <d3d10.h>
#include "Graphics/RenderTarget.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D10RenderTarget : public RenderTarget
		{
		private:
			ID3D10RenderTargetView* _renderTarget;
			ID3D10Texture2D* _tex2D;
		public:
			D3D10RenderTarget(_In_ int width, _In_ int height);
			D3D10RenderTarget(_In_ ID3D10Texture2D* tex);
			~D3D10RenderTarget();
			ID3D10RenderTargetView* GetD3D10RenderTarget();
			void Clear();
		};
	}
}

#endif
