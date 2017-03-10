#include "Vulkan/VulkanCommandQueue.hpp"

#include <vector>
#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandList.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanSwapChain.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"

using namespace Eternal::Graphics;
using namespace std;

VulkanCommandQueue::VulkanCommandQueue(_In_ VulkanDevice& DeviceObj, _In_ VulkanSwapChain& SwapChainObj)
	: _Device(DeviceObj)
{
	vkGetDeviceQueue(_Device.GetVulkanDevice(), 0, 0, &_CommandQueue);

	_CommandAllocators.resize(SwapChainObj.GetBackBuffersFrameCount());
	for (uint32_t CommandAllocatorIndex = 0; CommandAllocatorIndex < _CommandAllocators.size(); ++CommandAllocatorIndex)
	{
		_CommandAllocators[CommandAllocatorIndex] = new VulkanCommandAllocator(DeviceObj);
	}
}

VulkanCommandQueue::~VulkanCommandQueue()
{
	for (uint32_t CommandAllocatorIndex = 0; CommandAllocatorIndex < _CommandAllocators.size(); ++CommandAllocatorIndex)
	{
		delete _CommandAllocators[CommandAllocatorIndex];
	}
	_CommandAllocators.clear();
}

void VulkanCommandQueue::Reset(_In_ uint32_t FrameIndex)
{
	VkResult Result = vkResetCommandPool(_Device.GetVulkanDevice(), _CommandAllocators[FrameIndex]->GetVulkanCommandPool(), VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	ETERNAL_ASSERT(!Result);
}

VkQueue_T* VulkanCommandQueue::GetCommandQueue()
{
	return _CommandQueue;
}

CommandAllocator* VulkanCommandQueue::GetCommandAllocator(_In_ uint32_t FrameIndex)
{
	ETERNAL_ASSERT(FrameIndex < _CommandAllocators.size());
	return _CommandAllocators[FrameIndex];
}
