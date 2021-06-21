#pragma once

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
		class CommandQueue;

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
			static constexpr int32_t FrameBufferingCount = 2;

			GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
			virtual ~GraphicsContext();

			virtual void UpdateGraphicsContext() {}

			Device& GetDevice() { return *_Device; }
			Window& GetWindow() { return _Window; }
			SwapChain& GetSwapChain() { return *_SwapChain; }
			Viewport& GetMainViewport() { return _MainViewportFullScreen; }

			CommandQueue& GetGraphicsQueue() { return *_GraphicsQueue; }
			CommandQueue& GetComputeQueue() { return *_ComputeQueue; }
			CommandQueue& GetCopyQueue() { return *_CopyQueue; }

		private:
			Window _Window;
			Viewport _MainViewportFullScreen;
			Device* _Device = nullptr;
			SwapChain* _SwapChain = nullptr;

			CommandQueue* _GraphicsQueue = nullptr;
			CommandQueue* _ComputeQueue = nullptr;
			CommandQueue* _CopyQueue = nullptr;
		};

		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
		void DestroyGraphicsContext(GraphicsContext*& Context);
	}
}
