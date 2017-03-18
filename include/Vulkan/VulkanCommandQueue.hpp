#ifndef _VULKAN_COMMAND_QUEUE_HPP_
#define _VULKAN_COMMAND_QUEUE_HPP_

#include "Graphics/CommandQueue.hpp"
#include <vector>
#include <cstdint>

struct VkQueue_T;
struct VkCommandPool_T;
struct VkSemaphore_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class CommandList;
		class Fence;
		class SwapChain;
		class CommandAllocator;
		class VulkanCommandAllocator;

		class VulkanCommandQueue : public CommandQueue
		{
		public:
			VulkanCommandQueue(_In_ Device& DeviceObj, _In_ SwapChain& SwapChainObj);
			~VulkanCommandQueue();

			virtual void Reset(_In_ uint32_t FrameIndex) override;
			virtual void Submit(_In_ uint32_t FrameIndex, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount, _In_ Fence& FenceObj, _In_ SwapChain& SwapChainObj) override;
			virtual CommandAllocator* GetCommandAllocator(_In_ uint32_t FrameIndex) override;
			VkQueue_T* GetVulkanCommandQueue();

		private:
			vector<VulkanCommandAllocator*>	_CommandAllocators;
			Device&							_Device;
			VkQueue_T*						_CommandQueue	= nullptr;
		};
	}
}

#endif
