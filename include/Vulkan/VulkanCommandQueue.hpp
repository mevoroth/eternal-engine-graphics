#pragma once

#include "Graphics/CommandQueue.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include "Vulkan/VulkanUtils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class CommandList;

		class VulkanCommandQueue : public CommandQueue
		{
		public:
			VulkanCommandQueue(_In_ Device& InDevice, _In_ const CommandType& Type);
			~VulkanCommandQueue();

			virtual void SubmitCommandLists(_In_ GraphicsContext& GfxContext, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) override;
			vk::Semaphore* GetSubmitCompletionSemaphoreAndReset();

			vk::Queue& GetVulkanCommandQueue() { return _CommandQueue; }
			uint32_t GetQueueFamilyIndex() const { return _QueueFamilyIndex; }
			uint32_t GetQueueIndex() const { return _QueueIndex; }

		private:
			Device&			_Device;
			vk::Queue		_CommandQueue;
			vk::Semaphore	_SubmitCompletionSemaphore;
			bool			_HasSubmittedCommandLists = false;
			uint32_t		_QueueFamilyIndex	= Vulkan::InvalidQueueFamilyIndex;
			uint32_t		_QueueIndex			= Vulkan::InvalidQueueIndex;
		};
	}
}
