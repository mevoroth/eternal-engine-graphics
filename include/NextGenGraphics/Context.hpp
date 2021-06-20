#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include "GraphicsSettings.hpp"
#include "NextGenGraphics/Types.hpp"
#include "Window/Window.hpp"
#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class SwapChain;

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
			virtual ~GraphicsContext();

			virtual void UpdateGraphicsContext() {}

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
