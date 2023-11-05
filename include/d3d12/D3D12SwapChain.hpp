#pragma once

#include "Graphics/SwapChain.hpp"

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

		class D3D12SwapChain final : public SwapChain
		{
		public:
			D3D12SwapChain(_In_ GraphicsContext& Context);
			virtual ~D3D12SwapChain() override final;

			virtual void Acquire(_In_ GraphicsContext& Context) override final;
			virtual void Present(_In_ GraphicsContext& Context) override final;

			IDXGISwapChain* GetDXGISwapChain() { return _SwapChain; }

		private:
			IDXGISwapChain*					_SwapChain					= nullptr;
			IDXGISwapChain3*				_SwapChain3					= nullptr;
			uint32_t						_BackBuffersCount			= 0u;
		};
	}
}
