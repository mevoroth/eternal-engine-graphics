#ifndef _D3D11_RENDERER_HPP_
#define _D3D11_RENDERER_HPP_

#include <DirectXMath.h>
#include "Graphics/Renderer.hpp"
#include "D3D11Context.hpp"

using namespace DirectX;

namespace Eternal
{
	namespace Graphics
	{
		class Material;
		class Camera;
		class D3D11VertexShader;
		class D3D11PixelShader;
		class D3D11InputLayout;

		class D3D11Renderer : public Renderer
		{
		public:
			D3D11Renderer(_In_ const RenderMode& Mode = HARDWARE, _In_ const AntiAliasing& AA = MSAA_4X);
			ID3D11Device* GetDevice();
			D3D11Context* GetMainContext();
			D3D11Context* CreateDeferredContext();

			virtual void Flush();
		private:
			ID3D11Device* _Device = nullptr;
			D3D11Context* _MainContext = nullptr;
			IDXGISwapChain* _SwapChain = nullptr;

			HRESULT _CreateDevice();
			HRESULT _CreateSwapChain();
			void _Settings();
		};
	}
}

#endif
