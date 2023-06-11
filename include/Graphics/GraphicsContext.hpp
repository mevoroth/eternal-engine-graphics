#pragma once

#include "GraphicsSettings.hpp"
#include "Graphics/Types.hpp"
#include "Window/Window.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/CommandUtils.hpp"
#include "Tools/PipelineDependency.hpp"
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
		class View;
		class Resource;
		class RenderPass;

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
			virtual void Execute(_In_ GraphicsContext& InContext) = 0;
		};


		class CommandListScope
		{
			friend class GraphicsContext;
		public:
			virtual ~CommandListScope();

			inline CommandList* operator->() { return _CommandList; }
			inline CommandList& operator*() { return *_CommandList; }
			inline operator CommandList*() { return _CommandList; }

		protected:
			CommandListScope(_In_ GraphicsContext& InContext, _In_ CommandList* InCommandList, _In_ const string& InName);

			GraphicsContext&	_Context;
			CommandList*		_CommandList = nullptr;
		};

		class GraphicsCommandListScope : public CommandListScope
		{
			friend class GraphicsContext;
		public:
			virtual ~GraphicsCommandListScope();

		protected:
			GraphicsCommandListScope(_In_ GraphicsContext& InContext, _In_ CommandList* InCommandList, _In_ const string& InName, _In_ const RenderPass& InRenderPass);
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

			PipelineDependency& GetPipelineDependency() { return _PipelineDependency; }
			Device& GetDevice() { return *_Device; }
			Window& GetWindow() { return _Window; }
			SwapChain& GetSwapChain() { return *_SwapChain; }
			Viewport& GetMainViewport() { return *_MainViewportFullScreen; }
			Viewport& GetBackBufferViewport() { return *_BackBufferViewportFullScreen; }
			InputLayout* GetEmptyInputLayout() { return _EmptyInputLayout; }
			Sampler* GetPointClampSampler() { return _PointClampSampler; }
			Sampler* GetBilinearClampSampler() { return _BilinearClampSampler; }
			Sampler* GetBilinearWrapSampler() { return _BilinearWrapSampler; }

			CommandQueue& GetGraphicsQueue();
			CommandQueue& GetComputeQueue() { return *_ComputeQueue; }
			CommandQueue& GetCopyQueue() { return *_CopyQueue; }

			Shader* GetShader(_In_ const ShaderCreateInformation& InShaderCreateInformation);
			CommandListScope CreateNewCommandList(_In_ const CommandType& InType, _In_ const string& InName);
			GraphicsCommandListScope CreateNewGraphicsCommandList(_In_ const RenderPass& InRenderPass, _In_ const string& InName);

			Fence& GetCurrentFrameFence() { return *_FrameFences[GetCurrentFrameIndex()]; }
			Fence& GetNextFrameFence() { return *_FrameFences[(GetCurrentFrameIndex() + 1) % static_cast<uint32_t>(_FrameFences.size())]; }
			View& GetCurrentFrameBackBufferView();
			Resource& GetCurrentFrameBackBuffer();
			RenderPass& GetCurrentFrameBackBufferRenderPass();
			RenderPass* GetBackBufferRenderPass();

			uint32_t& GetCurrentFrameIndex() { return _CurrentFrameIndex; }

			template<typename ResourceType>
			void DelayedDelete(_In_ ResourceType* InResource);

			void RegisterGraphicsCommands(_In_ vector<GraphicsCommand*>* InCommands);

		protected:
			GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);

			void WaitForAllFences();

		private:

			CommandList* _CreateNewCommandList(_In_ const CommandType& InType, _In_ const string& InName);

			array<
				array<CommandAllocator*, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>,
				FrameBufferingCount
			> _CommandAllocators;

			array<
				array<vector<CommandList*>, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>,
				FrameBufferingCount
			> _CommandListPools;

			array<RenderPass*, FrameBufferingCount>									_BackBufferRenderPasses;
			array<uint32_t, static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT)>	_CurrentFrameCommandListIndex;
			array<Fence*, FrameBufferingCount>										_FrameFences;
			array<vector<View*>, FrameBufferingCount>								_ViewsToClear;
			array<vector<Resource*>, FrameBufferingCount>							_ResourcesToClear;

			vector<GraphicsCommand*>*												_GraphicsCommands = nullptr;

			Window _Window;
			PipelineDependency _PipelineDependency;
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
			Sampler* _BilinearWrapSampler			= nullptr;

			uint32_t _CurrentFrameIndex				= FrameBufferingCount - 1; // During first frame, this will be set correct within range
		};

		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
		void DestroyGraphicsContext(_Inout_ GraphicsContext*& Context);
	}
}
