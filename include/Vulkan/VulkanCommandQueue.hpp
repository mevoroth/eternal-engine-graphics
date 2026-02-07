#pragma once

#if ETERNAL_ENABLE_VULKAN

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
		class VulkanDevice;
		class CommandList;

		class VulkanCommandQueue final : public CommandQueue
		{
		public:
			VulkanCommandQueue(_In_ Device& InDevice, _In_ const CommandType& Type);
			~VulkanCommandQueue();

			virtual void SubmitCommandLists(_In_ CommandList* InCommandLists[], _In_ uint32_t InCommandListsCount, _In_ GraphicsContext* InContext) override final;

			vk::Queue& GetVulkanCommandQueue() { return _CommandQueue; }
			uint32_t GetQueueFamilyIndex() const { return _QueueFamilyIndex; }
			uint32_t GetQueueIndex() const { return _QueueIndex; }

		private:
			vk::Queue				_CommandQueue;
			uint32_t				_QueueFamilyIndex	= Vulkan::InvalidQueueFamilyIndex;
			uint32_t				_QueueIndex			= Vulkan::InvalidQueueIndex;
		};
	}
}

#endif
