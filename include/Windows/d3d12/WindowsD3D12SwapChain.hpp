#pragma once

#include "Graphics/SwapChain.hpp"

#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS

struct IDXGISwapChain;
struct IDXGISwapChain3;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class OutputDevice;
		class CommandQueue;
		class GraphicsContext;

		class D3D12WindowsSwapChain final : public SwapChain
		{
		public:
			D3D12WindowsSwapChain(_In_ GraphicsContext& InContext);
			virtual ~D3D12WindowsSwapChain() override final;

			virtual void Acquire(_In_ GraphicsContext& InContext) override final;
			virtual void Present(_In_ GraphicsContext& InContext) override final;

			IDXGISwapChain* GetDXGISwapChain() { return _SwapChain; }

		private:
			IDXGISwapChain*					_SwapChain					= nullptr;
			IDXGISwapChain3*				_SwapChain3					= nullptr;
			uint32_t						_BackBuffersCount			= 0u;
		};
	}
}

#endif
