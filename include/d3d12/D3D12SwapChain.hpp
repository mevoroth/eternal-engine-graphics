#pragma once

#include "Graphics/SwapChain.hpp"

struct IDXGISwapChain;
struct IDXGISwapChain3;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class Window;
		class CommandQueue;
		class GraphicsContext;

		class D3D12SwapChain : public SwapChain
		{
		public:
			D3D12SwapChain(_In_ GraphicsContext& Context);
			virtual ~D3D12SwapChain();

			virtual void Acquire(GraphicsContext& Context) override;
			virtual void Present(GraphicsContext& Context) override;

			IDXGISwapChain* GetDXGISwapChain() { return _SwapChain; }

		private:
			IDXGISwapChain*					_SwapChain					= nullptr;
			IDXGISwapChain3*				_SwapChain3					= nullptr;
			uint32_t						_BackBuffersCount			= 0u;
		};
	}
}
