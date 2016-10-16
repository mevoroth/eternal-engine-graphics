#ifndef _D3D11_DEPTH_STENCIL_BUFFER_HPP_
#define _D3D11_DEPTH_STENCIL_BUFFER_HPP_

#include <cstdint>

#include "Graphics/RenderTarget.hpp"
#include "d3d11/D3D11Texture.hpp"

struct ID3D11DepthStencilView;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11DepthStencilBuffer
			: public RenderTarget
			, public D3D11Texture
		{
		public:
			D3D11DepthStencilBuffer(uint32_t Width, uint32_t Height);
			~D3D11DepthStencilBuffer();
			inline ID3D11DepthStencilView* GetD3D11DepthStencilView()
			{
				return _DepthStencilView;
			}
#pragma region RenderTarget
			virtual Resource* GetAsResource();
#pragma endregion RenderTarget
#pragma region Clearable
			virtual void Clear(_In_ Context* ContextObj) override;
			virtual void Clear(_In_ Context* ContextObj, _In_ float Value) override;
#pragma endregion Clearable

		private:
			ID3D11DepthStencilView* _DepthStencilView = nullptr;
		};
	}
}

#endif
