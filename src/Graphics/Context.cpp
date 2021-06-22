#include "NextGenGraphics/Context.hpp"

#include "NextGenGraphics/DeviceFactory.hpp"
#include "NextGenGraphics/Device.hpp"
#include "NextGenGraphics/Context.hpp"
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

using namespace Eternal::Graphics;

namespace Eternal
{
	namespace Graphics
	{
		GraphicsContext* CreateGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
		{
			switch (CreateInformation.Settings.Driver)
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new GraphicsContext(CreateInformation);
#endif
			case DeviceType::VULKAN:
				return new VulkanGraphicsContext(CreateInformation);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}

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
			, _MainViewportFullScreen(0, 0,
									  CreateInformation.Settings.Width, CreateInformation.Settings.Height)
		{
			_Window.Create(CreateInformation.Arguments.WindowEventsHandler);

			_Device			= CreateDevice(*this, CreateInformation.Settings.Driver);

			_GraphicsQueue	= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_GRAPHIC);
			_ComputeQueue	= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_COMPUTE);
			_CopyQueue		= CreateCommandQueue(*_Device, CommandType::COMMAND_TYPE_COPY);

			_SwapChain		= CreateSwapChain(*this);

			_SubmitFence	= CreateFence(*_Device);

			for (int32_t CommandAllocatorFrameIndex = 0; CommandAllocatorFrameIndex < FrameBufferingCount; ++CommandAllocatorFrameIndex)
			{
				_CommandAllocators[static_cast<int32_t>(CommandType::COMMAND_TYPE_GRAPHIC)][CommandAllocatorFrameIndex]	= CreateCommandAllocator(*_Device, *_GraphicsQueue);
				_CommandAllocators[static_cast<int32_t>(CommandType::COMMAND_TYPE_COMPUTE)][CommandAllocatorFrameIndex]	= CreateCommandAllocator(*_Device, *_ComputeQueue);
				_CommandAllocators[static_cast<int32_t>(CommandType::COMMAND_TYPE_COPY)][CommandAllocatorFrameIndex]	= CreateCommandAllocator(*_Device, *_CopyQueue);
			}
		}

		GraphicsContext::~GraphicsContext()
		{
			for (int32_t CommandTypeIndex = 0; CommandTypeIndex < _CommandAllocators.size(); ++CommandTypeIndex)
			{
				for (int32_t CommandAllocatorFrameIndex = 0; CommandAllocatorFrameIndex < FrameBufferingCount; ++CommandAllocatorFrameIndex)
				{
					delete _CommandAllocators[CommandTypeIndex][CommandAllocatorFrameIndex];
					_CommandAllocators[CommandTypeIndex][CommandAllocatorFrameIndex] = nullptr;
				}
			}

			delete _SubmitFence;
			_SubmitFence = nullptr;

			delete _SwapChain;
			_SwapChain = nullptr;

			delete _CopyQueue;
			_CopyQueue = nullptr;

			delete _ComputeQueue;
			_ComputeQueue = nullptr;

			delete _GraphicsQueue;
			_GraphicsQueue = nullptr;

			delete _Device;
			_Device = nullptr;
		}

		CommandQueue& GraphicsContext::GetGraphicsQueue()
		{
			ETERNAL_ASSERT(_GraphicsQueue);
			return *_GraphicsQueue;
		}

		CommandList* GraphicsContext::CreateNewCommandList(const CommandType& Type)
		{
			return CreateCommandList(*_Device, *_CommandAllocators[static_cast<int32_t>(Type)][CurrentFrameIndex]);
		}
	}
}
