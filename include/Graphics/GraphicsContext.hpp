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
		using namespace std;

		struct ShaderCreateInformation;

		class Device;
		class SwapChain;
		class CommandQueue;
		class Fence;
		class CommandAllocator;
		class CommandList;
		class ShaderFactory;
		class Shader;
		class GraphicsContext;
		class InputLayout;
		class Sampler;

		struct GraphicsContextCreateInformation
		{
			GraphicsContextCreateInformation(const RenderSettings& Settings, const WindowsArguments& Arguments)
				: Settings(Settings)
				, Arguments(Arguments)
			{}

			RenderSettings Settings;
			WindowsArguments Arguments;
		};

		struct GraphicsCommand
		{
			virtual ~GraphicsCommand() {}
			virtual void Execute(_In_ GraphicsContext& InContext, _In_ CommandList& InCommandList) = 0;
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

			virtual ~GraphicsContext();

			void Initialize();

			void BeginFrame();
			void EndFrame();
			virtual void ResetFrameStates();

			Device& GetDevice() { return *_Device; }
			Window& GetWindow() { return _Window; }
			SwapChain& GetSwapChain() { return *_SwapChain; }
			Viewport& GetMainViewport() { return *_MainViewportFullScreen; }
			Viewport& GetBackBufferViewport() { return *_BackBufferViewportFullScreen; }
			InputLayout& GetEmptyInputLayout() { return *_EmptyInputLayout; }
			Sampler* GetPointClampSampler() { return _PointClampSampler; }
			Sampler* GetBilinearClampSampler() { return _BilinearClampSampler; }

			CommandQueue& GetGraphicsQueue();
			CommandQueue& GetComputeQueue() { return *_ComputeQueue; }
			CommandQueue& GetCopyQueue() { return *_CopyQueue; }

			Shader* GetShader(_In_ const ShaderCreateInformation& InShaderCreateInformation);
			CommandList* CreateNewCommandList(_In_ const CommandType& Type, _In_ const std::string& InName);

			Fence& GetCurrentFrameFence() { return *_FrameFences[GetCurrentFrameIndex()]; }
			Fence& GetNextFrameFence() { return *_FrameFences[(GetCurrentFrameIndex() + 1) % static_cast<uint32_t>(_FrameFences.size())]; }

			uint32_t& GetCurrentFrameIndex() { return _CurrentFrameIndex; }

			template<typename ResourceType>
			void DelayedDelete(_In_ ResourceType* InResource);

			void RegisterGraphicsCommands(_In_ vector<GraphicsCommand*>* InCommands);

		protected:
			GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);

			void WaitForAllFences();

		private:

			array<
				array<CommandAllocator*, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>,
				FrameBufferingCount
			> _CommandAllocators;

			array<
				array<vector<CommandList*>, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>,
				FrameBufferingCount
			> _CommandListPools;

			array<uint32_t, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>	_CurrentFrameCommandListIndex;
			array<Fence*, FrameBufferingCount>										_FrameFences;
			array<vector<View*>, FrameBufferingCount>								_ViewsToClear;
			array<vector<Resource*>, FrameBufferingCount>							_ResourcesToClear;

			vector<GraphicsCommand*>*												_GraphicsCommands = nullptr;

			Window _Window;
			Device* _Device							= nullptr;
			SwapChain* _SwapChain					= nullptr;

			CommandQueue* _GraphicsQueue			= nullptr;
			CommandQueue* _ComputeQueue				= nullptr;
			CommandQueue* _CopyQueue				= nullptr;

			ShaderFactory* _ShaderFactory			= nullptr;
			
			Viewport* _MainViewportFullScreen		= nullptr;
			Viewport* _BackBufferViewportFullScreen	= nullptr;
			InputLayout* _EmptyInputLayout			= nullptr;
			Sampler* _PointClampSampler				= nullptr;
			Sampler* _BilinearClampSampler			= nullptr;

			uint32_t _CurrentFrameIndex				= FrameBufferingCount - 1; // During first frame, this will be set correct within range
		};

		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
		void DestroyGraphicsContext(_Inout_ GraphicsContext*& Context);
	}
}
