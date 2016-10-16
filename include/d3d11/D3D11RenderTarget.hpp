#ifndef _D3D11_RENDER_TARGET_HPP_
#define _D3D11_RENDER_TARGET_HPP_

#include "Graphics/RenderTarget.hpp"
#include "d3d11/D3D11Texture.hpp"

struct ID3D11RenderTargetView;

namespace Eternal
{
	namespace Graphics
	{
		enum Format;

		class D3D11RenderTarget
			: public RenderTarget
			, public D3D11Texture
		{
		public:
			D3D11RenderTarget(_In_ const Format& FormatObj, uint32_t Width, _In_ uint32_t Height);
			D3D11RenderTarget(_In_ ID3D11Texture2D* Tex);
			~D3D11RenderTarget();
			ID3D11RenderTargetView* GetD3D11RenderTarget();
#pragma region RenderTarget
			virtual Resource* GetAsResource() override;
			virtual void Clear(_In_ Context* ContextObj) override;
			virtual void Clear(_In_ Context* ContextObj, _In_ float Value) override;
#pragma endregion RenderTarget

		private:
			ID3D11RenderTargetView* _RenderTarget = nullptr;
		};
	}
}

#endif
