#include "Vulkan/VulkanCommandQueue.hpp"

#include <vector>
#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandList.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanSwapChain.hpp"

using namespace Eternal::Graphics;
using namespace std;

VulkanCommandQueue::VulkanCommandQueue(_In_ VulkanDevice& DeviceObj, _In_ VulkanSwapChain& SwapChainObj)
	: _Device(DeviceObj)
{
	vkGetDeviceQueue(_Device.GetDevice(), 0, 0, &_CommandQueue);

	VkCommandPoolCreateInfo CommandPoolInfo;
	CommandPoolInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolInfo.pNext				= nullptr;
	CommandPoolInfo.flags				= 0;
	CommandPoolInfo.queueFamilyIndex	= DeviceObj.GetQueueFamilyIndex();

	VkResult Result = vkCreateCommandPool(DeviceObj.GetDevice(), &CommandPoolInfo, nullptr, &_CommandPool);
	ETERNAL_ASSERT(!Result);

	_CompletedSemaphores.resize(SwapChainObj.GetBackBuffersFrameCount());

	for (uint32_t SemaphoreIndex = 0; SemaphoreIndex < _CompletedSemaphores.size(); ++SemaphoreIndex)
	{
		VkSemaphoreCreateInfo SemaphoreInfo;

		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		SemaphoreInfo.pNext = nullptr;
		SemaphoreInfo.flags = 0;

		Result = vkCreateSemaphore(DeviceObj.GetDevice(), &SemaphoreInfo, nullptr, &_CompletedSemaphores[SemaphoreIndex]);
		ETERNAL_ASSERT(!Result);
	}
}

VulkanCommandQueue::~VulkanCommandQueue()
{
	vkDestroyCommandPool(_Device.GetDevice(), _CommandPool, nullptr);
	_CommandPool = nullptr;
}

void VulkanCommandQueue::Reset(_In_ uint32_t FrameIndex)
{
	VkResult Result = vkResetCommandPool(_Device.GetDevice(), _CommandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	ETERNAL_ASSERT(!Result);
}

VkSemaphore_T*& VulkanCommandQueue::GetCompletedSemaphore(_In_ uint32_t ResourceIndex)
{
	return _CompletedSemaphores[ResourceIndex];
}

VkQueue_T* VulkanCommandQueue::GetCommandQueue()
{
	return _CommandQueue;
}
