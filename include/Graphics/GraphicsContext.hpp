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
		class ShaderFactory;

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
			static constexpr uint32_t CommandListUsage[static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)] = {
				1024,
				256,
				256
			};

			GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
			virtual ~GraphicsContext();

			void Initialize();

			void BeginFrame();
			void EndFrame();
			void ResetFrameStates();

			Device& GetDevice() { return *_Device; }
			Window& GetWindow() { return _Window; }
			SwapChain& GetSwapChain() { return *_SwapChain; }
			Viewport& GetMainViewport() { return *_MainViewportFullScreen; }

			CommandQueue& GetGraphicsQueue();
			CommandQueue& GetComputeQueue() { return *_ComputeQueue; }
			CommandQueue& GetCopyQueue() { return *_CopyQueue; }

			CommandList* CreateNewCommandList(const CommandType& Type);

			Fence& GetCurrentFrameFence() { return *_FrameFences[GetCurrentFrameIndex()]; }
			Fence& GetNextFrameFence() { return *_FrameFences[(GetCurrentFrameIndex() + 1) % static_cast<uint32_t>(_FrameFences.size())]; }

			ShaderFactory& GetShaderFactory() { return *_ShaderFactory; }

			uint32_t& GetCurrentFrameIndex() { return _CurrentFrameIndex; }

		private:

			std::array<
				std::array<CommandAllocator*, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>,
				FrameBufferingCount
			> _CommandAllocators;

			std::array<
				std::array<std::vector<CommandList*>, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>,
				FrameBufferingCount
			> _CommandListPools;

			std::array<uint32_t, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>	_CurrentFrameCommandListIndex;
			std::array<Fence*, FrameBufferingCount>										_FrameFences;

			Window _Window;
			Viewport* _MainViewportFullScreen	= nullptr;
			Device* _Device						= nullptr;
			SwapChain* _SwapChain				= nullptr;

			CommandQueue* _GraphicsQueue		= nullptr;
			CommandQueue* _ComputeQueue			= nullptr;
			CommandQueue* _CopyQueue			= nullptr;

			ShaderFactory* _ShaderFactory		= nullptr;

			uint32_t _CurrentFrameIndex			= ~0; // During first frame, this will be set correct within range
		};

		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
		void DestroyGraphicsContext(GraphicsContext*& Context);
	}
}
