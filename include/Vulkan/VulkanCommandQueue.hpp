#pragma once

#include "Graphics/CommandQueue.hpp"
#include <vulkan/vulkan.hpp>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class CommandList;
		//class Fence;
		//class SwapChain;

		class VulkanCommandQueue : public CommandQueue
		{
		public:
			VulkanCommandQueue(_In_ Device& DeviceObj, _In_ const CommandType& Type);
			~VulkanCommandQueue();

			virtual void SubmitCommandLists(_In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) override;
			//VkQueue_T* GetVulkanCommandQueue();

		private:
			//Device&							_Device;
			//VkQueue_T*						_CommandQueue	= nullptr;
			vk::Queue	_CommandQueue;
		};
	}
}
