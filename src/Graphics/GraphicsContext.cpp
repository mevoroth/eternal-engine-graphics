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
			case DeviceType::VULKAN:
				Context = new VulkanGraphicsContext(CreateInformation);
				break;

			default:
				ETERNAL_BREAK();
			}

			Context->Initialize();

			return Context;
		}

		void DestroyGraphicsContext(GraphicsContext*& Context)
		{
			delete Context;
			Context = nullptr;
		}

		GraphicsContext::GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
			: _Window(CreateInformation.Arguments.hInstance,
					  CreateInformation.Arguments.nCmdShow,
					  CreateInformation.Arguments.Name,
					  CreateInformation.Arguments.ClassName,
					  CreateInformation.Settings.Width,
					  CreateInformation.Settings.Height)
		{
			_Window.Create(CreateInformation.Arguments.WindowEventsHandler);

			_Device					= CreateDevice(*this, CreateInformation.Settings.Driver);

			_MainViewportFullScreen	= CreateInvertedViewport(*this, 0, 0, CreateInformation.Settings.Width, CreateInformation.Settings.Height);

			_GraphicsQueue			= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_GRAPHIC);
			_ComputeQueue			= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_COMPUTE);
			_CopyQueue				= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_COPY);


			for (int32_t FrameIndex = 0; FrameIndex < FrameBufferingCount; ++FrameIndex)
			{
				_FrameFences[FrameIndex] = CreateFence(*_Device);

				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_GRAPHIC)]	= CreateCommandAllocator(*_Device, *_GraphicsQueue);
				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_COMPUTE)]	= CreateCommandAllocator(*_Device, *_ComputeQueue);
				_CommandAllocators[FrameIndex][static_cast<int32_t>(CommandType::COMMAND_TYPE_COPY)]	= CreateCommandAllocator(*_Device, *_CopyQueue);

				for (int32_t CommandTypeIndex = 0; CommandTypeIndex < static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT); ++CommandTypeIndex)
					_CommandListPools[FrameIndex][CommandTypeIndex].reserve(CommandListUsage[CommandTypeIndex]);
			}

			_CurrentFrameCommandListIndex.fill(0);

			_ShaderFactory	= new ShaderFactory();
		}

		GraphicsContext::~GraphicsContext()
		{
			delete _ShaderFactory;
			_ShaderFactory = nullptr;

			delete _SwapChain;
			_SwapChain = nullptr;

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
			_SwapChain = CreateSwapChain(*this);
		}

		void GraphicsContext::BeginFrame()
		{
			GetNextFrameFence().Wait(GetDevice());
			GetNextFrameFence().Reset(GetDevice());
			GetSwapChain().Acquire(*this);
			ResetFrameStates();
		}

		void GraphicsContext::EndFrame()
		{
			GetSwapChain().Present(*this);
		}

		void GraphicsContext::ResetFrameStates()
		{
			_CurrentFrameCommandListIndex.fill(0);
			for (uint32_t CommandTypeIndex = 0; CommandTypeIndex < _CommandAllocators.size(); ++CommandTypeIndex)
				_CommandAllocators[_CurrentFrameIndex][CommandTypeIndex]->Reset();
		}

		CommandQueue& GraphicsContext::GetGraphicsQueue()
		{
			ETERNAL_ASSERT(_GraphicsQueue);
			return *_GraphicsQueue;
		}

		CommandList* GraphicsContext::CreateNewCommandList(const CommandType& Type)
		{
			uint32_t TypeInt			= static_cast<int32_t>(Type);
			uint32_t CommandListIndex	= _CurrentFrameCommandListIndex[TypeInt]++;

			std::vector<CommandList*>& CurrentCommandListPool = _CommandListPools[_CurrentFrameIndex][TypeInt];

			if (CommandListIndex < CurrentCommandListPool.size())
			{
				return CurrentCommandListPool[CommandListIndex];
			}

			CurrentCommandListPool.push_back(
				CreateCommandList(*_Device, *_CommandAllocators[_CurrentFrameIndex][TypeInt])
			);
			return CurrentCommandListPool.back();
		}
	}
}
