#ifndef _SWAP_CHAIN_HPP_
#define _SWAP_CHAIN_HPP_

#include "Graphics/Viewport.hpp"
#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Fence;
		class CommandQueue;
		class Resource;
		class View;
		class RenderPass;
		class Context;
		class BlendState;
		class Window;

		class SwapChain
		{
		public:
			SwapChain(_In_ Window& WindowObj);
			virtual ~SwapChain() {}

			virtual void Present() = 0;

			//virtual void AcquireFrame(_In_ Device& DeviceObj, _Inout_ Context& GfxContext) = 0;
			//virtual void Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _Inout_ Context& GfxContext) = 0;

			//virtual Resource& GetBackBuffer(_In_ uint32_t BackBufferIndex) = 0;
			//virtual View& GetBackBufferView(_In_ uint32_t BackBufferIndex) = 0;
			virtual uint32_t GetBackBuffersFrameCount() const = 0;
			virtual RenderPass& GetMainRenderPass() = 0;

			const Viewport& GetMainViewport() const		{ return _BackBufferViewport; }
			BlendState& GetBackBufferBlendState()		{ return _BackBufferBlendState; }

		private:
			Viewport	_BackBufferViewport;
			BlendState	_BackBufferBlendState;
		};
	}
}

#endif
