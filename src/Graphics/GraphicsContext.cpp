#include "Graphics/GraphicsContext.hpp"

#include "Graphics/DeviceFactory.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12GraphicsContext.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Graphics/CommandUtils.h"
#include "Graphics/SwapChainFactory.hpp"
#include "Graphics/SwapChain.hpp"
#include "Graphics/CommandQueueFactory.hpp"
#include "Graphics/CommandQueue.hpp"
#include "Graphics/FenceFactory.hpp"
#include "Graphics/Fence.hpp"
#include "Graphics/CommandAllocatorFactory.hpp"
#include "Graphics/CommandAllocator.hpp"
#include "Graphics/CommandListFactory.hpp"
#include "Graphics/CommandList.hpp"
#include "Graphics/ShaderFactory.hpp"
#include "Graphics/ViewportFactory.hpp"
#include "Graphics/View.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/InputLayoutFactory.hpp"
#include "Graphics/SamplerFactory.hpp"
#include "Graphics/RenderPassFactory.hpp"
#include "Graphics/RenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
		{
			GraphicsContext* Context = nullptr;

			switch (CreateInformation.Settings.Driver)
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				Context = new D3D12GraphicsContext(CreateInformation);
				break;
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				Context = new VulkanGraphicsContext(CreateInformation);
				break;
#endif
			default:
				ETERNAL_BREAK();
			}

			Context->Initialize();

			return Context;
		}

		void DestroyGraphicsContext(_Inout_ GraphicsContext*& Context)
		{
			delete Context;
			Context = nullptr;
		}

		GraphicsContext::GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
			: _Window(WindowCreateInformation(
				CreateInformation.Arguments.hInstance,
				CreateInformation.Arguments.nCmdShow,
				CreateInformation.Arguments.Name,
				CreateInformation.Arguments.ClassName,
				CreateInformation.Settings.Width,
				CreateInformation.Settings.Height,
				CreateInformation.Settings.IsVSync,
				CreateInformation.Settings.IsWindowed
			))
		{
			static constexpr uint32_t ResourcesToClearInitialCount = 256;

			_Window.Create(CreateInformation.Arguments.WindowEventsHandler);

			_Device							= CreateDevice(*this, CreateInformation.Settings.Driver);

			_GraphicsQueue					= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_GRAPHICS);
			_ComputeQueue					= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_COMPUTE);
			_CopyQueue						= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_COPY);

			for (int32_t FrameIndex = 0; FrameIndex < FrameBufferingCount; ++FrameIndex)
			{
				_FrameFences[FrameIndex] = CreateFence(*_Device);

				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_GRAPHICS)]	= CreateCommandAllocator(*_Device, *_GraphicsQueue);
				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_COMPUTE)]	= CreateCommandAllocator(*_Device, *_ComputeQueue);
				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_COPY)]	= CreateCommandAllocator(*_Device, *_CopyQueue);

				for (int32_t CommandTypeIndex = 0; CommandTypeIndex < static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT); ++CommandTypeIndex)
					_CommandListPools[FrameIndex][CommandTypeIndex].reserve(CommandListUsage[CommandTypeIndex]);

				_ViewsToClear[FrameIndex].reserve(ResourcesToClearInitialCount);
				_ResourcesToClear[FrameIndex].reserve(ResourcesToClearInitialCount);
			}

			_CurrentFrameCommandListIndex.fill(0);

			_ShaderFactory	= new ShaderFactory();
			
			_MainViewportFullScreen			= CreateViewport(*this, CreateInformation.Settings.Width, CreateInformation.Settings.Height);
			_BackBufferViewportFullScreen	= CreateInvertedViewport(*this, CreateInformation.Settings.Width, CreateInformation.Settings.Height);
			_EmptyInputLayout				= CreateInputLayout(*this);
		}

		GraphicsContext::~GraphicsContext()
		{
			for (uint32_t RenderPassIndex = 0; RenderPassIndex < _BackBufferRenderPasses.size(); ++RenderPassIndex)
				DestroyRenderPass(_BackBufferRenderPasses[RenderPassIndex]);

			delete _SwapChain;
			_SwapChain = nullptr;

			DestroySampler(_BilinearClampSampler);
			DestroySampler(_PointClampSampler);
			DestroyInputLayout(_EmptyInputLayout);
			DestroyViewport(_BackBufferViewportFullScreen);
			DestroyViewport(_MainViewportFullScreen);

			delete _ShaderFactory;
			_ShaderFactory = nullptr;

			for (int32_t FrameIndex = 0; FrameIndex < FrameBufferingCount; ++FrameIndex)
			{
				for (uint32_t CommandTypeIndex = 0; CommandTypeIndex < _CommandAllocators.size(); ++CommandTypeIndex)
				{
					vector<CommandList*>& CurrentCommandListPool = _CommandListPools[FrameIndex][CommandTypeIndex];
					for (uint32_t CommandListIndex = 0; CommandListIndex < CurrentCommandListPool.size(); ++CommandListIndex)
					{
						delete CurrentCommandListPool[CommandListIndex];
						CurrentCommandListPool[CommandListIndex] = nullptr;
					}

					delete _CommandAllocators[FrameIndex][CommandTypeIndex];
					_CommandAllocators[FrameIndex][CommandTypeIndex] = nullptr;
				}

				delete _FrameFences[FrameIndex];
				_FrameFences[FrameIndex] = nullptr;
			}

			delete _CopyQueue;
			_CopyQueue = nullptr;

			delete _ComputeQueue;
			_ComputeQueue = nullptr;

			delete _GraphicsQueue;
			_GraphicsQueue = nullptr;

			delete _Device;
			_Device = nullptr;
		}

		void GraphicsContext::Initialize()
		{
			_PointClampSampler = CreateSampler(*this, SamplerCreateInformation(
				/*InMINLinear =*/ false,
				/*InMAGLinear =*/ false,
				/*InMIPLinear =*/ false,
				AddressMode::ADDRESS_MODE_CLAMP,
				AddressMode::ADDRESS_MODE_CLAMP,
				AddressMode::ADDRESS_MODE_CLAMP
			));
			_BilinearClampSampler = CreateSampler(*this, SamplerCreateInformation(
				/*InMINLinear =*/ true,
				/*InMAGLinear =*/ true,
				/*InMIPLinear =*/ false,
				AddressMode::ADDRESS_MODE_CLAMP,
				AddressMode::ADDRESS_MODE_CLAMP,
				AddressMode::ADDRESS_MODE_CLAMP
			));

			_SwapChain = CreateSwapChain(*this);

			for (uint32_t RenderPassIndex = 0; RenderPassIndex < _BackBufferRenderPasses.size(); ++RenderPassIndex)
			{
				_BackBufferRenderPasses[RenderPassIndex] = CreateRenderPass(
					*this,
					RenderPassCreateInformation(
						GetBackBufferViewport(),
						{
							RenderTargetInformation(BlendStateNone, RenderTargetOperator::Load_Store, _SwapChain->GetBackBufferRenderTargetViews()[RenderPassIndex])
						}
					)
				);
			}
		}

		View& GraphicsContext::GetCurrentFrameBackBufferView()
		{
			return *_SwapChain->GetBackBufferRenderTargetViews()[GetCurrentFrameIndex()];
		}

		Resource& GraphicsContext::GetCurrentFrameBackBuffer()
		{
			return *_SwapChain->GetBackBuffers()[GetCurrentFrameIndex()];
		}

		RenderPass& GraphicsContext::GetCurrentFrameBackBufferRenderPass()
		{
			return *_BackBufferRenderPasses[GetCurrentFrameIndex()];
		}

		RenderPass* GraphicsContext::GetBackBufferRenderPass()
		{
			return _BackBufferRenderPasses[0];
		}

		void GraphicsContext::BeginFrame()
		{
			{
				ETERNAL_PROFILER(INFO)("Fences");
				GetNextFrameFence().Wait(GetDevice());
				GetNextFrameFence().Reset(GetDevice());
			}
			{
				ETERNAL_PROFILER(INFO)("Acquire SwapChain");
				GetSwapChain().Acquire(*this);
			}

			ResetFrameStates();

			{
				ETERNAL_PROFILER(INFO)("TransitionBackBufferToRenderTarget");
				const vector<View*>& BackBufferViews = GetSwapChain().GetBackBufferRenderTargetViews();
				CommandListScope TransitionToRenderTargetCommandList = CreateNewCommandList(CommandType::COMMAND_TYPE_GRAPHICS, "TransitionBackBufferToRenderTarget");
				ResourceTransition BackBufferPresentToRenderTarget(BackBufferViews[GetCurrentFrameIndex()], TransitionState::TRANSITION_RENDER_TARGET);
				TransitionToRenderTargetCommandList->Transition(&BackBufferPresentToRenderTarget, 1);
			}

			{
				ETERNAL_PROFILER(INFO)("GraphicsCommands");
				if (_GraphicsCommands)
				{
					for (uint32_t CommandIndex = 0, CommandCount = static_cast<uint32_t>(_GraphicsCommands->size()); CommandIndex < CommandCount; ++CommandIndex)
					{
						(*_GraphicsCommands)[CommandIndex]->Execute(*this);
						delete (*_GraphicsCommands)[CommandIndex];
						(*_GraphicsCommands)[CommandIndex] = nullptr;
					}
					_GraphicsCommands->clear();
					_GraphicsCommands = nullptr;
				}
			}
		}

		void GraphicsContext::EndFrame()
		{
			{
				ETERNAL_PROFILER(INFO)("TransitionBackBufferToPresent");
				const vector<View*>& BackBufferViews = GetSwapChain().GetBackBufferRenderTargetViews();
				CommandListScope TransitionToBackBufferCommandList = CreateNewCommandList(CommandType::COMMAND_TYPE_GRAPHICS, "TransitionBackBufferToPresent");
				ResourceTransition BackBufferRenderTargetToPresent(BackBufferViews[GetCurrentFrameIndex()], TransitionState::TRANSITION_PRESENT);
				TransitionToBackBufferCommandList->Transition(&BackBufferRenderTargetToPresent, 1);
			}

			uint32_t CopyQueueIndex = static_cast<uint32_t>(CommandType::COMMAND_TYPE_COPY);
			uint32_t ComputeQueueIndex = static_cast<uint32_t>(CommandType::COMMAND_TYPE_COMPUTE);
			uint32_t GraphicsQueueIndex = static_cast<uint32_t>(CommandType::COMMAND_TYPE_GRAPHICS);
			if (_CurrentFrameCommandListIndex[CopyQueueIndex] > 0)
			{
				ETERNAL_PROFILER(INFO)("Submit Copy Queue");
				GetCopyQueue().SubmitCommandLists(
					_CommandListPools[_CurrentFrameIndex][CopyQueueIndex].data(),
					_CurrentFrameCommandListIndex[CopyQueueIndex]
				);
			}
			
			if (_CurrentFrameCommandListIndex[ComputeQueueIndex] > 0)
			{
				ETERNAL_PROFILER(INFO)("Submit Compute Queue");
				GetComputeQueue().SubmitCommandLists(
					_CommandListPools[_CurrentFrameIndex][ComputeQueueIndex].data(),
					_CurrentFrameCommandListIndex[ComputeQueueIndex]
				);
			}

			if (_CurrentFrameCommandListIndex[GraphicsQueueIndex] > 0)
			{
				ETERNAL_PROFILER(INFO)("Submit Graphics Queue");
				GetGraphicsQueue().SubmitCommandLists(
					_CommandListPools[_CurrentFrameIndex][GraphicsQueueIndex].data(),
					_CurrentFrameCommandListIndex[GraphicsQueueIndex],
					this
				);
			}
			{
				ETERNAL_PROFILER(INFO)("Present");
				GetSwapChain().Present(*this);
			}
		}

		void GraphicsContext::ResetFrameStates()
		{
			ETERNAL_PROFILER(DETAIL)();
			{
				ETERNAL_PROFILER(INFO)("CommandAllocators");
				_CurrentFrameCommandListIndex.fill(0);
				for (uint32_t CommandTypeIndex = 0; CommandTypeIndex < _CommandAllocators.size(); ++CommandTypeIndex)
					_CommandAllocators[_CurrentFrameIndex][CommandTypeIndex]->Reset();
			}

			{
				ETERNAL_PROFILER(INFO)("Vectors");
				vector<View*>& ViewsToClear = _ViewsToClear[_CurrentFrameIndex];
				for (uint32_t ViewIndex = 0; ViewIndex < ViewsToClear.size(); ++ViewIndex)
					delete ViewsToClear[ViewIndex];
				ViewsToClear.clear();

				vector<Resource*>& ResourcesToClear = _ResourcesToClear[_CurrentFrameIndex];
				for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesToClear.size(); ++ResourceIndex)
					delete ResourcesToClear[ResourceIndex];
				ResourcesToClear.clear();
			}
		}

		CommandQueue& GraphicsContext::GetGraphicsQueue()
		{
			ETERNAL_ASSERT(_GraphicsQueue);
			return *_GraphicsQueue;
		}

		CommandList* GraphicsContext::_CreateNewCommandList(_In_ const CommandType& InType, _In_ const string& InName)
		{
			uint32_t TypeInt			= static_cast<int32_t>(InType);
			uint32_t CommandListIndex	= _CurrentFrameCommandListIndex[TypeInt]++;

			std::vector<CommandList*>& CurrentCommandListPool = _CommandListPools[_CurrentFrameIndex][TypeInt];

			CommandList* NewCommandList = nullptr;
			if (CommandListIndex < CurrentCommandListPool.size())
			{
				NewCommandList = CurrentCommandListPool[CommandListIndex];
			}
			else
			{
				NewCommandList = CreateCommandList(*_Device, *_CommandAllocators[_CurrentFrameIndex][TypeInt]);
				CurrentCommandListPool.push_back(NewCommandList);
			}

			NewCommandList->SetName(*this, InName);
			return NewCommandList;
		}

		CommandListScope GraphicsContext::CreateNewCommandList(_In_ const CommandType& InType, _In_ const string& InName)
		{
			return CommandListScope(_CreateNewCommandList(InType, InName), *this);
		}

		GraphicsCommandListScope GraphicsContext::CreateNewGraphicsCommandList(_In_ const RenderPass& InRenderPass, _In_ const string& InName)
		{
			return GraphicsCommandListScope(_CreateNewCommandList(CommandType::COMMAND_TYPE_GRAPHICS, InName), InRenderPass, *this);
		}

		Shader* GraphicsContext::GetShader(_In_ const ShaderCreateInformation& InShaderCreateInformation)
		{
			ETERNAL_ASSERT(_ShaderFactory);
			return _ShaderFactory->GetShader(*this, InShaderCreateInformation);
		}

		template<> void GraphicsContext::DelayedDelete<View>(_In_ View* InView)
		{
			_ViewsToClear[_CurrentFrameIndex].push_back(InView);
		}

		template<> void GraphicsContext::DelayedDelete<Resource>(_In_ Resource* InResource)
		{
			_ResourcesToClear[_CurrentFrameIndex].push_back(InResource);
		}

		void GraphicsContext::WaitForAllFences()
		{
			for (int32_t FrameIndex = 0; FrameIndex < FrameBufferingCount; ++FrameIndex)
				_FrameFences[FrameIndex]->Wait(GetDevice());
		}

		void GraphicsContext::RegisterGraphicsCommands(_In_ vector<GraphicsCommand *>* InCommands)
		{
			_GraphicsCommands = InCommands;
		}

		//////////////////////////////////////////////////////////////////////////

		CommandListScope::CommandListScope(_In_ CommandList* InCommandList, _In_ GraphicsContext& InContext)
			: _CommandList(InCommandList)
		{
			_CommandList->Begin(InContext);
		}

		CommandListScope::~CommandListScope()
		{
			_CommandList->End();
			_CommandList = nullptr;
		}


		GraphicsCommandListScope::GraphicsCommandListScope(_In_ CommandList* InCommandList, _In_ const RenderPass& InRenderPass, _In_ GraphicsContext& InContext)
			: CommandListScope(InCommandList, InContext)
		{
			_CommandList->BeginRenderPass(InRenderPass);
		}

		GraphicsCommandListScope::~GraphicsCommandListScope()
		{
			_CommandList->EndRenderPass();
		}
	}
}
