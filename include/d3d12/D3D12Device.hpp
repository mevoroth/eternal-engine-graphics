#ifndef _D3D12_DEVICE_HPP_
#define _D3D12_DEVICE_HPP_

#include <cstdint>

struct IDXGISwapChain;
struct IDXGIFactory4;
struct IDXGIAdapter1;
struct ID3D12Device;
struct ID3D12Debug;

namespace Eternal
{
	namespace Graphics
	{
		class Window;
		class D3D12CommandQueue;
		class D3D12RenderTarget;

		class D3D12Device
		{
		public:
			const uint32_t MAX_DXGI_ADAPTER = 10;
			D3D12Device();

			void CreateSwapChain(Window& WindowObj);

			inline ID3D12Device* GetDevice()
			{
				return _Device;
			}
			inline IDXGISwapChain* GetSwapChain() { return _SwapChain; }

		private:
			void _CreateDirectCommandQueue();

			D3D12CommandQueue* _CommandQueue = nullptr;
			D3D12RenderTarget* _BackBuffer = nullptr;

			IDXGIFactory4* _DXGIFactory = nullptr;
			IDXGIAdapter1* _DXGIAdapter = nullptr;
			ID3D12Device* _Device = nullptr;
			IDXGISwapChain* _SwapChain = nullptr;
#ifdef ETERNAL_DEBUG
			ID3D12Debug* _Debug = nullptr;
#endif
		};
	}
}

#endif
