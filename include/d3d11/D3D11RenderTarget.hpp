#ifndef _D3D11_RENDER_TARGET_HPP_
#define _D3D11_RENDER_TARGET_HPP_

#include <d3d11.h>
#include "Graphics/RenderTarget.hpp"
#include "d3d11/D3D11Texture.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11RenderTarget
			: public RenderTarget
			, public D3D11Texture
		{
		private:
			ID3D11RenderTargetView* _RenderTarget;
			ID3D11Texture2D* _Tex2D;
		public:
			D3D11RenderTarget(_In_ int Width, _In_ int Height);
			D3D11RenderTarget(_In_ ID3D11Texture2D* tex);
			~D3D11RenderTarget();
			ID3D11RenderTargetView* GetD3D11RenderTarget();
			void Clear();

#pragma region D3D11Texture
			virtual ID3D11SamplerState* CreateSamplerState() override;
#pragma endregion D3D11Texture

		};
	}
}

#endif
