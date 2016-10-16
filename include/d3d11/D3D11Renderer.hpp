#ifndef _D3D11_RENDERER_HPP_
#define _D3D11_RENDERER_HPP_

#include "Graphics/Renderer.hpp"

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11Debug;
struct ID3D11InfoQueue;

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class D3D11Renderer : public Renderer
		{
		public:
			D3D11Renderer(_In_ const RenderMode& Mode = HARDWARE, _In_ const AntiAliasing& AA = MSAA_4X);
			virtual ~D3D11Renderer();
			ID3D11Device* GetDevice();
			virtual Context* GetMainContext() override;
			virtual Context* CreateDeferredContext() override;

			virtual void Flush() override;
		private:
			Context* _MainContext = nullptr;

			ID3D11Device* _Device = nullptr;
			IDXGISwapChain* _SwapChain = nullptr;
			ID3D11Debug* _Debug = nullptr;
			ID3D11InfoQueue* _InfoQueue = nullptr;

			int _CreateDevice();
			int _CreateSwapChain();
		};
	}
}

#endif
