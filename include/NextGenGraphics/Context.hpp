#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include "GraphicsSettings.hpp"
#include "NextGenGraphics/Types.hpp"
#include "Window/Window.hpp"
#include "Graphics_deprecated/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class SwapChain;
		class Fence;
		class RenderPass;
		class Resource;

		class Context
		{
		public:
			Context(_In_ Device& DeviceObj);
			virtual ~Context();

			void		SetFrameIndex(_In_ uint32_t FrameIndex)					{ _FrameIndex = FrameIndex; }
			uint32_t	GetFrameIndex() const									{ return _FrameIndex; }
			Fence*		GetFrameFence()											{ return _FrameFence; }
			void		SetFrameRenderPass(_In_ RenderPass* FrameRenderPass)	{ _FrameRenderPass = FrameRenderPass; }
			RenderPass*	GetFrameRenderPass()									{ return _FrameRenderPass; }
			void		SetFrameBackBuffer(_In_ Resource* FrameBackBuffer)		{ _FrameBackBuffer = FrameBackBuffer; }
			Resource*	GetFrameBackBuffer()									{ return _FrameBackBuffer; }

		private:
			Fence*			_FrameFence			=	nullptr;
			uint32_t		_FrameIndex			=	0u;
			RenderPass*		_FrameRenderPass	=	nullptr;
			Resource*		_FrameBackBuffer	=	nullptr;
		};

		struct GraphicsContextCreateInformation
		{
			GraphicsContextCreateInformation(const RenderSettings& Settings, const WindowsArguments& Arguments)
				: Settings(Settings)
				, Arguments(Arguments)
			{}

			RenderSettings Settings;
			WindowsArguments Arguments;
		};

		class GraphicsContext
		{
		public:
			GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
			~GraphicsContext();

			Device& GetDevice() { return *_Device; }
			Window& GetWindow() { return _Window; }
			SwapChain& GetSwapChain() { return *_SwapChain; }
			Viewport& GetMainViewport() { return _MainViewportFullScreen; }

		private:
			Window _Window;
			Viewport _MainViewportFullScreen;
			Device* _Device = nullptr;
			SwapChain* _SwapChain = nullptr;
		};

		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
		void DestroyGraphicsContext(GraphicsContext*& Context);
	}
}

#endif
