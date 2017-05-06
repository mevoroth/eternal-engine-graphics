#include "Vulkan/VulkanCommandQueue.hpp"

#include <vector>
#include "Macros/Macros.hpp"
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandList.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanSwapChain.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

using namespace Eternal::Graphics;
using namespace std;

VulkanCommandQueue::VulkanCommandQueue(_In_ Device& DeviceObj, _In_ uint32_t FrameCount)
	: _Device(DeviceObj)
{
	_CommandQueue = static_cast<VulkanDevice&>(DeviceObj).PopVulkanQueue();

	_CommandAllocators.resize(FrameCount);
	_CommandLists.resize(FrameCount);
	for (uint32_t CommandBufferIndex = 0; CommandBufferIndex < _CommandAllocators.size(); ++CommandBufferIndex)
	{
		_CommandAllocators[CommandBufferIndex]	= new VulkanCommandAllocator(DeviceObj);
		_CommandLists[CommandBufferIndex]		= new VulkanCommandList(DeviceObj, *_CommandAllocators[CommandBufferIndex]);
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
	VkResult Result = vkResetCommandPool(static_cast<VulkanDevice&>(_Device).GetVulkanDevice(), _CommandAllocators[FrameIndex]->GetVulkanCommandPool(), VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	ETERNAL_ASSERT(!Result);
}

void VulkanCommandQueue::Submit(_In_ uint32_t FrameIndex, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount, _In_ Fence& FenceObj, _In_ SwapChain& SwapChainObj)
{
	vector<VkCommandBuffer> VulkanCommandLists;
	VulkanCommandLists.resize(CommandListsCount);
	for (uint32_t CommandListIndex = 0; CommandListIndex < CommandListsCount; ++CommandListIndex)
	{
		VulkanCommandLists[CommandListIndex] = static_cast<VulkanCommandList*>(CommandLists[CommandListIndex])->GetVulkanCommandList();
	}
	
	VkPipelineStageFlags PipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	
	VkSubmitInfo SubmitInfo;
	SubmitInfo.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.pNext					= nullptr;
	SubmitInfo.waitSemaphoreCount		= 1;
	SubmitInfo.pWaitSemaphores			= &static_cast<VulkanSwapChain&>(SwapChainObj).GetAcquireSemaphore(FrameIndex);
	SubmitInfo.pWaitDstStageMask		= &PipelineStageFlags;
	SubmitInfo.commandBufferCount		= (uint32_t)VulkanCommandLists.size();
	SubmitInfo.pCommandBuffers			= VulkanCommandLists.data();
	SubmitInfo.signalSemaphoreCount		= 1;
	SubmitInfo.pSignalSemaphores		= &_CommandAllocators[FrameIndex]->GetSemaphore();

	char test[256];
	sprintf_s(test, "[VulkanFence::Signal] FENCE: %d\n", FrameIndex);
	OutputDebugString(test);
	
	VkResult Result = vkQueueSubmit(GetVulkanCommandQueue(), 1, &SubmitInfo, static_cast<VulkanFence&>(FenceObj).GetFence());
	ETERNAL_ASSERT(!Result);
}

VkQueue_T* VulkanCommandQueue::GetVulkanCommandQueue()
{
	return _CommandQueue;
}

CommandAllocator* VulkanCommandQueue::GetCommandAllocator(_In_ uint32_t FrameIndex)
{
	ETERNAL_ASSERT(FrameIndex < _CommandAllocators.size());
	return _CommandAllocators[FrameIndex];
}

CommandList* VulkanCommandQueue::Pop()
{
	VulkanCommandList* CommandListObj = _CommandLists[_FrameIndex];
	_FrameIndex = (_FrameIndex + 1) % _CommandLists.size();
	return CommandListObj;
}
