#ifndef _D3D12_DEVICE_HPP_
#define _D3D12_DEVICE_HPP_

#include <cstdint>
#include "NextGenGraphics/Device.hpp"

struct IDXGISwapChain;
struct IDXGISwapChain3;
struct IDXGIFactory4;
struct IDXGIAdapter1;
struct ID3D12Device;

#ifdef ETERNAL_DEBUG
struct ID3D12Debug;
#endif

namespace Eternal
{
	namespace Graphics
	{
		class Window;
		class D3D12CommandQueue;
		class D3D12RenderTarget;
		class D3D12DescriptorHeap;

		class D3D12Device : public Device
		{
		public:
			static void Initialize();
			static IDXGIFactory4* GetDXGIFactory();

			D3D12Device(_In_ uint32_t DeviceIndex);

			void CreateSwapChain(_In_ const Window& WindowObj);

			inline ID3D12Device* GetD3D12Device() { return _Device; }
			virtual uint32_t GetDeviceMask() const;

			inline uint32_t GetBackBufferFrameCount() const
			{
				return _BackBufferFrameCount;
			}

			D3D12RenderTarget*const & GetBackBuffer(_In_ uint32_t BackBufferIndex);
			D3D12CommandQueue*&	GetCommandQueue() { return _CommandQueue; }

		private:
			void _CreateDirectCommandQueue();

#ifdef ETERNAL_DEBUG
			static ID3D12Debug* _Debug;
#endif
			static IDXGIFactory4* _DXGIFactory;

			D3D12CommandQueue* _CommandQueue = nullptr;
			D3D12DescriptorHeap* _BackBufferDescriptorHeap = nullptr;
			D3D12RenderTarget** _BackBuffers;
			uint32_t _BackBufferFrameCount = 2;

			IDXGIAdapter1* _DXGIAdapter = nullptr;
			ID3D12Device* _Device = nullptr;

			uint32_t _DeviceMask = 0xFFFFFFFF;
		};
	}
}

#endif
