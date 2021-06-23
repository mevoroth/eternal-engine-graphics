#pragma once

#include "GraphicsSettings.hpp"
#include "Graphics/Types.hpp"
#include "Window/Window.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/CommandUtils.h"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class SwapChain;
		class CommandQueue;
		class Fence;
		class CommandAllocator;
		class CommandList;

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

			CommandQueue& GetGraphicsQueue();
			CommandQueue& GetComputeQueue() { return *_ComputeQueue; }
			CommandQueue& GetCopyQueue() { return *_CopyQueue; }

			CommandList* CreateNewCommandList(const CommandType& Type);

			Fence& GetSubmitFence() { return *_SubmitFence; }

			uint32_t& GetCurrentFrameIndex() { return CurrentFrameIndex; }

		private:
			Window _Window;
			Viewport _MainViewportFullScreen;
			Device* _Device					= nullptr;
			SwapChain* _SwapChain			= nullptr;

			CommandQueue* _GraphicsQueue	= nullptr;
			CommandQueue* _ComputeQueue		= nullptr;
			CommandQueue* _CopyQueue		= nullptr;

			Fence* _SubmitFence				= nullptr;

			std::array<
				std::array<CommandAllocator*, FrameBufferingCount>,
				int32_t(CommandType::COMMAND_TYPE_COUNT)
			> _CommandAllocators;

			uint32_t CurrentFrameIndex		= 0;
		};

		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
		void DestroyGraphicsContext(GraphicsContext*& Context);
	}
}
