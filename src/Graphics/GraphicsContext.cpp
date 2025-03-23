#include "Graphics/GraphicsContext.hpp"

#include "d3d12/D3D12GraphicsContext.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Graphics/AccelerationStructureFactory.hpp"
#include "Graphics/CommandAllocator.hpp"
#include "Graphics/CommandAllocatorFactory.hpp"
#include "Graphics/CommandList.hpp"
#include "Graphics/CommandListFactory.hpp"
#include "Graphics/Device.hpp"
#include "Graphics/DeviceFactory.hpp"
#include "Graphics/CommandUtils.hpp"
#include "Graphics/CommandQueueFactory.hpp"
#include "Graphics/CommandQueue.hpp"
#include "Graphics/Fence.hpp"
#include "Graphics/FenceFactory.hpp"
#include "Graphics/InputLayoutFactory.hpp"
#include "Graphics/Pipeline.hpp"
#include "Graphics/PipelineFactory.hpp"
#include "Graphics/SamplerFactory.hpp"
#include "Graphics/RenderPass.hpp"
#include "Graphics/RenderPassFactory.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "Graphics/RootSignatureFactory.hpp"
#include "Graphics/Sampler.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/ShaderFactory.hpp"
#include "Graphics/ShaderType.hpp"
#include "Graphics/SwapChain.hpp"
#include "Graphics/SwapChainFactory.hpp"
#include "Graphics/View.hpp"
#include "Graphics/ViewportFactory.hpp"
#include "Proxy/ProxyGraphicsContext.hpp"
#include "File/FileFactory.hpp"
#include "File/File.hpp"
#include "File/FilePath.hpp"
#include <utility>

namespace Eternal
{
	namespace Graphics
	{
		namespace GraphicsPrivate
		{
			struct GraphicsCommandInitialization final : public GraphicsCommand
			{
				virtual void Execute(_In_ GraphicsContext& InContext) override final
				{
					//CommandListScope InitializationCommandList = InContext.CreateNewCommandList(CommandType::COMMAND_TYPE_GRAPHICS, "GraphicsCommandInitialization");

					//ResourceTransition TransitionScratchAccelerationStructure(InContext.GetScratchAccelerationStructureBuffer(), TransitionState::TRANSITION_SHADER_WRITE);
					//InitializationCommandList->Transition(TransitionScratchAccelerationStructure);
				}
			};

			static void OnStencilWriteFunctorNull(_In_ uint32_t InStencilBit, _In_ const string& InOwner)
			{
				ETERNAL_BREAK();
			}
		}

		OnStencilWriteFunction GraphicsContext::OnStencilWriteFunctor = GraphicsPrivate::OnStencilWriteFunctorNull;

		OnStencilWriteFunction GraphicsContext::GetOnStencilWriteFunctor()
		{
			return OnStencilWriteFunctor;
		}

		void GraphicsContext::SetOnStencilWriteFunctor(_In_ const OnStencilWriteFunction InOnStencilWriteFunctor)
		{
			OnStencilWriteFunctor = InOnStencilWriteFunctor;
		}

		GraphicsContext::GraphicsContext(_In_ const GraphicsContextCreateInformation& InGraphicsContextCreateInformation, _In_ OutputDevice& InOutputDevice)
			: _GraphicsContextCreateInformation(InGraphicsContextCreateInformation)
			, _OutputDevice(InOutputDevice)
		{
			static constexpr uint32_t ResourcesToClearInitialCount = 256;

			_Device							= CreateDevice(*this, InGraphicsContextCreateInformation.Settings.Driver);

			_GraphicsQueue					= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_GRAPHICS);
			_ComputeQueue					= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_COMPUTE);
			_CopyQueue						= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_COPY);

			for (int32_t FrameIndex = 0; FrameIndex < FrameBufferingCount; ++FrameIndex)
			{
				_FrameFences[FrameIndex] = CreateFence(*_Device);

				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_GRAPHICS)]	= CreateCommandAllocator(*_Device, *_GraphicsQueue);
				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_COMPUTE)]		= CreateCommandAllocator(*_Device, *_ComputeQueue);
				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_COPY)]		= CreateCommandAllocator(*_Device, *_CopyQueue);

				for (int32_t CommandTypeIndex = 0; CommandTypeIndex < static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT); ++CommandTypeIndex)
					_CommandListPools[FrameIndex][CommandTypeIndex].reserve(CommandListUsage[CommandTypeIndex]);

				_ViewsToClear[FrameIndex].reserve(ResourcesToClearInitialCount);
				_ResourcesToClear[FrameIndex].reserve(ResourcesToClearInitialCount);
				_AccelerationStructuresToClear[FrameIndex].reserve(ResourcesToClearInitialCount);
			}

			_CurrentFrameCommandListIndex.fill(0);

			_ShaderFactory						= new ShaderFactory();
			
			_MainViewportFullScreen				= CreateInvertedViewport(*this, InGraphicsContextCreateInformation.Settings.Width, InGraphicsContextCreateInformation.Settings.Height);
			_BackBufferViewportFullScreen		= CreateInvertedViewport(*this, InGraphicsContextCreateInformation.Settings.Width, InGraphicsContextCreateInformation.Settings.Height);
			_EmptyRootSignature					= CreateRootSignature(*this);
			_EmptyLocalRootSignature			= CreateRootSignature(*this, /* InIsLocalRootSignature = */ true);
			_EmptyInputLayout					= CreateInputLayout(*this);

			_ScratchAccelerationStructureBuffer	= CreateBuffer(
				BufferResourceCreateInformation(
					*_Device,
					"ScratchAccelerationStructureBuffer",
					AccelerationStructureBufferCreateInformation(ScratchAccelerationStructureBufferSize),
					ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_MEMORY,
					TransitionState::TRANSITION_SHADER_WRITE
				)
			);

			_GraphicsCommands.push_back(new GraphicsPrivate::GraphicsCommandInitialization());
		}

		GraphicsContext::~GraphicsContext()
		{
			for (uint32_t RenderPassIndex = 0; RenderPassIndex < _BackBufferRenderPasses.size(); ++RenderPassIndex)
				DestroyRenderPass(_BackBufferRenderPasses[RenderPassIndex]);

			delete _SwapChain;
			_SwapChain = nullptr;

			DestroySampler(_BilinearWrapSampler);
			DestroySampler(_BilinearClampSampler);
			DestroySampler(_PointClampSampler);
			DestroyResource(_ScratchAccelerationStructureBuffer);
			DestroyInputLayout(_EmptyInputLayout);
			DestroyRootSignature(_EmptyLocalRootSignature);
			DestroyRootSignature(_EmptyRootSignature);
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

		void GraphicsContext::InitializeGraphicsContext()
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
			_BilinearWrapSampler = CreateSampler(*this, SamplerCreateInformation(
				/*InMINLinear =*/ true,
				/*InMAGLinear =*/ true,
				/*InMIPLinear =*/ false,
				AddressMode::ADDRESS_MODE_WRAP,
				AddressMode::ADDRESS_MODE_WRAP,
				AddressMode::ADDRESS_MODE_WRAP
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

			SerializePipelineLibrary(PipelineSerializationMode::PIPELINE_SERIALIZATION_MODE_READ);
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
				ETERNAL_PROFILER(INFO)("PipelineRecompile");
				for (auto ShaderIterator = _PipelineRecompile.Shaders.begin(); ShaderIterator != _PipelineRecompile.Shaders.end(); ++ShaderIterator)
				{
					Shader* CurrentShader = *ShaderIterator;
					ShaderCreateInformation CreateInformation = CurrentShader->GetShaderCreateInformation();
					CurrentShader->~Shader();
					_ShaderFactory->Create(*this, CreateInformation, CurrentShader);
				}

				_PipelinesToClear[_CurrentFrameIndex].reserve(_PipelineRecompile.Pipelines.size());
				for (uint32_t PipelineIndex = 0; PipelineIndex < _PipelineRecompile.Pipelines.size(); ++PipelineIndex)
				{
					Pipeline* NewPipeline = nullptr;
					if (_PipelineRecompile.Pipelines[PipelineIndex]->GetShaderTypes() == ShaderTypeFlags::SHADER_TYPE_FLAGS_COMPUTE)
						NewPipeline = CreatePipeline(*this, ComputePipelineCreateInformation(_PipelineRecompile.Pipelines[PipelineIndex]->GetPipelineCreateInformation()));
					else if ((_PipelineRecompile.Pipelines[PipelineIndex]->GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX) == ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX)
						NewPipeline = CreatePipeline(*this, GraphicsPipelineCreateInformation(_PipelineRecompile.Pipelines[PipelineIndex]->GetPipelineCreateInformation()));
					//else if ((_PipelineRecompile.Pipelines[PipelineIndex]->GetShaderTypes() & ShaderTypeFlags::MS) == ShaderTypeFlags::MS)
					//	CreatePipeline(*this, MeshPipelineCreateInformation(_PipelineRecompile.Pipelines[PipelineIndex]->GetPipelineCreateInformation()), _PipelineRecompile.Pipelines[PipelineIndex]);
					else if ((_PipelineRecompile.Pipelines[PipelineIndex]->GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION) == ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_RAY_GENERATION)
						NewPipeline = CreatePipeline(*this, RayTracingPipelineCreateInformation(_PipelineRecompile.Pipelines[PipelineIndex]->GetPipelineCreateInformation()));
					else
					{
						ETERNAL_BREAK(); // Incorrect pipeline
					}
					SwapPipelines(*this, _PipelineRecompile.Pipelines[PipelineIndex], NewPipeline);
					_PipelinesToClear[_CurrentFrameIndex].push_back(NewPipeline);
				}

				_PipelineRecompile.Shaders.clear();
				_PipelineRecompile.Pipelines.clear();
			}

			{
				ETERNAL_PROFILER(INFO)("GraphicsCommands");
				if (_NewGraphicsCommands)
				{
					ETERNAL_ASSERT(_NewGraphicsCommands->size() > 0);
					_GraphicsCommands.insert(_GraphicsCommands.end(), _NewGraphicsCommands->begin(), _NewGraphicsCommands->end());
					_NewGraphicsCommands->clear();
					_NewGraphicsCommands = nullptr;
				}

				for (uint32_t CommandCount = min(static_cast<uint32_t>(_GraphicsCommands.size()), _CurrentGraphicsCommandIndex + GraphicsCommandBudgetPerFrame);
					_CurrentGraphicsCommandIndex < CommandCount;
					++_CurrentGraphicsCommandIndex)
				{
					_GraphicsCommands[_CurrentGraphicsCommandIndex]->Execute(*this);
				}

				if (_CurrentGraphicsCommandIndex >= _GraphicsCommands.size())
				{
					for (uint32_t CommandIndex = 0; CommandIndex < _GraphicsCommands.size(); ++CommandIndex)
					{
						delete _GraphicsCommands[CommandIndex];
						_GraphicsCommands[CommandIndex] = nullptr;
					}

					_GraphicsCommands.clear();
					_CurrentGraphicsCommandIndex = 0;
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

				vector<Pipeline*>& PipelinesToClear = _PipelinesToClear[_CurrentFrameIndex];
				for (uint32_t PipelineIndex = 0; PipelineIndex < PipelinesToClear.size(); ++PipelineIndex)
					DestroyPipeline(PipelinesToClear[PipelineIndex]);
				PipelinesToClear.clear();

				vector<AccelerationStructure*>& AccelerationStructuresToClear = _AccelerationStructuresToClear[_CurrentFrameIndex];
				for (uint32_t AccelerationStructureIndex = 0; AccelerationStructureIndex < AccelerationStructuresToClear.size(); ++AccelerationStructureIndex)
					DestroyAccelerationStructure(AccelerationStructuresToClear[AccelerationStructureIndex]);
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
			return CommandListScope(*this, _CreateNewCommandList(InType, InName), InName);
		}

		CommandList* GraphicsContext::CreateNewCommandListUnsafe(_In_ const CommandType& InType, _In_ const string& InName)
		{
			return _CreateNewCommandList(InType, InName);
		}

		GraphicsCommandListScope GraphicsContext::CreateNewGraphicsCommandList(_In_ const RenderPass& InRenderPass, _In_ const string& InName)
		{
			return GraphicsCommandListScope(*this, _CreateNewCommandList(CommandType::COMMAND_TYPE_GRAPHICS, InName), InName, InRenderPass);
		}

		Shader* GraphicsContext::GetShader(_In_ const ShaderCreateInformation& InShaderCreateInformation)
		{
			ETERNAL_ASSERT(_ShaderFactory);
			return _ShaderFactory->GetShader(*this, InShaderCreateInformation);
		}

		Shader* GraphicsContext::GetShader()
		{
			ETERNAL_ASSERT(_ShaderFactory);
			return _ShaderFactory->GetShader(*this);
		}

		template<> void GraphicsContext::DelayedDelete<View>(_In_ View* InView)
		{
			_ViewsToClear[_CurrentFrameIndex].push_back(InView);
		}

		template<> void GraphicsContext::DelayedDelete<Resource>(_In_ Resource* InResource)
		{
			_ResourcesToClear[_CurrentFrameIndex].push_back(InResource);
		}

		template<> void GraphicsContext::DelayedDelete<AccelerationStructure>(_In_ AccelerationStructure* InAccelerationStructure)
		{
			_AccelerationStructuresToClear[_CurrentFrameIndex].push_back(InAccelerationStructure);
		}

		void GraphicsContext::WaitForAllFences()
		{
			for (int32_t FrameIndex = 0; FrameIndex < FrameBufferingCount; ++FrameIndex)
				_FrameFences[FrameIndex]->Wait(GetDevice());
		}

		ShaderFactory* GraphicsContext::GetShaderFactory()
		{
			return _ShaderFactory;
		}

		void GraphicsContext::RegisterGraphicsCommands(_In_ vector<GraphicsCommand *>* InCommands)
		{
			_NewGraphicsCommands = InCommands;
		}

		void GraphicsContext::RegisterPipelineRecompile(_In_ const ResolvedPipelineDependency& InPipelineDependencies)
		{
			_PipelineRecompile = InPipelineDependencies;
		}

		void GraphicsContext::SerializePipelineLibrary(_In_ PipelineSerializationMode InPipelineSerializationMode)
		{
			DeviceType CurrentDeviceType = _Device->GetDeviceType();
			if (CurrentDeviceType == DeviceType::DEVICE_TYPE_PROXY)
			{
				if (InPipelineSerializationMode == PipelineSerializationMode::PIPELINE_SERIALIZATION_MODE_READ)
					return;

				CurrentDeviceType = static_cast<ProxyGraphicsContext*>(this)->GetProxyDeviceType();
			}

			string DeviceTypeString = ConvertDeviceTypeToString(CurrentDeviceType);
			char PipelineFileName[256];
			sprintf_s(PipelineFileName, "pipelines.%s.library", DeviceTypeString.c_str());

			string PipelinesLibraryFileName = [&InPipelineSerializationMode, &PipelineFileName]() -> string
			{
				if (InPipelineSerializationMode == PipelineSerializationMode::PIPELINE_SERIALIZATION_MODE_WRITE)
					return FilePath::FindOrCreate(PipelineFileName, FileType::FILE_TYPE_CACHED_PIPELINES);
				else
					return FilePath::Find(PipelineFileName, FileType::FILE_TYPE_CACHED_PIPELINES);
			}();

			if (InPipelineSerializationMode == PipelineSerializationMode::PIPELINE_SERIALIZATION_MODE_READ)
			{
				if (PipelinesLibraryFileName.length() == 0)
					return;
			}

			File* PipelinesLibraryFile = CreateFileHandle(PipelinesLibraryFileName);
			PipelinesLibraryFile->Open(InPipelineSerializationMode == PipelineSerializationMode::PIPELINE_SERIALIZATION_MODE_WRITE ? FileOpenMode::FILE_OPEN_MODE_WRITE : FileOpenMode::FILE_OPEN_MODE_READ);

			_PipelineLibrary.SerializePipelineLibrary(*this, PipelinesLibraryFile);

			PipelinesLibraryFile->Close();
			DestroyFileHandle(PipelinesLibraryFile);
		}

		//////////////////////////////////////////////////////////////////////////

		CommandListScope::CommandListScope(_In_ GraphicsContext& InContext, _In_ CommandList* InCommandList, _In_ const string& InName)
			: _Context(InContext)
			, _CommandList(InCommandList)
		{
			_CommandList->Begin(InContext);
			_CommandList->BeginEvent(InContext, InName.c_str());
		}

		CommandListScope::~CommandListScope()
		{
			_CommandList->EndEvent(_Context);
			_CommandList->End();
			_CommandList = nullptr;
		}


		GraphicsCommandListScope::GraphicsCommandListScope(_In_ GraphicsContext& InContext, _In_ CommandList* InCommandList, _In_ const string& InName, _In_ const RenderPass& InRenderPass)
			: CommandListScope(InContext, InCommandList, InName)
		{
			_CommandList->BeginRenderPass(InRenderPass);
		}

		GraphicsCommandListScope::~GraphicsCommandListScope()
		{
			_CommandList->EndRenderPass();
		}
	}
}
