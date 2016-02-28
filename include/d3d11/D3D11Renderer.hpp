#ifndef _D3D11_RENDERER_HPP_
#define _D3D11_RENDERER_HPP_

#include "Graphics/Renderer.hpp"
#include "D3D11Context.hpp"

struct ID3D11Device;
struct IDXGISwapChain;

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class D3D11Renderer : public Renderer
		{
		public:
			D3D11Renderer(_In_ const RenderMode& Mode = HARDWARE, _In_ const AntiAliasing& AA = MSAA_4X);
			ID3D11Device* GetDevice();
			virtual Context* GetMainContext() override;
			virtual Context* CreateDeferredContext() override;

			virtual void Flush();
		private:
			ID3D11Device* _Device = nullptr;
			D3D11Context* _MainContext = nullptr;
			IDXGISwapChain* _SwapChain = nullptr;

			int _CreateDevice();
			int _CreateSwapChain();
		};
	}
}

#endif
