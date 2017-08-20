#ifndef _VULKAN_COMMAND_QUEUE_HPP_
#define _VULKAN_COMMAND_QUEUE_HPP_

#include "Graphics/CommandQueue.hpp"
#include <vector>
#include <cstdint>

struct VkQueue_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class CommandList;
		class Fence;
		class SwapChain;

		class VulkanCommandQueue : public CommandQueue
		{
		public:
			VulkanCommandQueue(_In_ Device& DeviceObj);
			~VulkanCommandQueue();

			virtual void Submit(_In_ Context& GfxContext, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) override;
			VkQueue_T* GetVulkanCommandQueue();

		private:
			Device&							_Device;
			VkQueue_T*						_CommandQueue	= nullptr;
		};
	}
}

#endif
