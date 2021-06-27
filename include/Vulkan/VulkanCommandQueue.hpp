#pragma once

#include "Graphics/CommandQueue.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include <vector>

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

			virtual void SubmitCommandLists(_In_ CommandList* InCommandLists[], _In_ uint32_t InCommandListsCount, _In_ GraphicsContext* InContext) override final;
			void GetSubmitCompletionSemaphoresAndReset(_Out_ vk::Semaphore*& OutSemaphores, uint32_t& OutSemaphoresCount);

			vk::Queue& GetVulkanCommandQueue() { return _CommandQueue; }
			uint32_t GetQueueFamilyIndex() const { return _QueueFamilyIndex; }
			uint32_t GetQueueIndex() const { return _QueueIndex; }

		private:
			Device&					_Device;
			vk::Queue				_CommandQueue;
			vector<vk::Semaphore>	_SubmitCompletionSemaphores;
			uint32_t				_SubmitCount		= 0;
			uint32_t				_QueueFamilyIndex	= Vulkan::InvalidQueueFamilyIndex;
			uint32_t				_QueueIndex			= Vulkan::InvalidQueueIndex;
		};
	}
}
