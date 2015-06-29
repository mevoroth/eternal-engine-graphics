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
			ID3D11RenderTargetView* _RenderTarget = nullptr;
			ID3D11Texture2D* _Tex2D = nullptr;
		public:
			D3D11RenderTarget(_In_ int Width, _In_ int Height);
			D3D11RenderTarget(_In_ ID3D11Texture2D* Tex);
			~D3D11RenderTarget();
			ID3D11RenderTargetView* GetD3D11RenderTarget();
#pragma region RenderTarget
			virtual void Clear(Context* ContextObj) override;
#pragma endregion RenderTarget
#pragma region D3D11Resource
			virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView() override;
			//virtual ID3D11Buffer* GetD3D11Buffer() override;
#pragma endregion D3D11Resource
#pragma region D3D11Texture
			//virtual ID3D11SamplerState* CreateSamplerState() override;
			//virtual ID3D11ShaderResourceView* CreateShaderResourceView() override;
			//ID3D11UnorderedAccessView* CreateUnorderedAccessView();
#pragma endregion D3D11Texture

		};
	}
}

#endif
