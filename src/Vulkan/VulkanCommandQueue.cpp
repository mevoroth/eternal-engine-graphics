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

VulkanCommandQueue::VulkanCommandQueue(_In_ Device& DeviceObj)
	: _Device(DeviceObj)
{
	_CommandQueue = static_cast<VulkanDevice&>(DeviceObj).PopVulkanQueue();
}

VulkanCommandQueue::~VulkanCommandQueue()
{
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

	// ACQUIRE FRAME SEMAPHORE
	//SubmitInfo.waitSemaphoreCount		= 1;
	//SubmitInfo.pWaitSemaphores			= &static_cast<VulkanSwapChain&>(SwapChainObj).GetAcquireSemaphore(FrameIndex);
	SubmitInfo.pWaitDstStageMask		= &PipelineStageFlags;
	SubmitInfo.commandBufferCount		= (uint32_t)VulkanCommandLists.size();
	SubmitInfo.pCommandBuffers			= VulkanCommandLists.data();

	// PREVIOUS FRAME SEMAPHORE
	//SubmitInfo.signalSemaphoreCount		= 1;
	//SubmitInfo.pSignalSemaphores		= &_CommandAllocators[FrameIndex]->GetSemaphore();

	//char test[256];
	//sprintf_s(test, "[VulkanFence::Signal] FENCE: %d\n", FrameIndex);
	//OutputDebugString(test);
	
	VkResult Result = vkQueueSubmit(GetVulkanCommandQueue(), 1, &SubmitInfo, static_cast<VulkanFence&>(FenceObj).GetFence());
	ETERNAL_ASSERT(!Result);
}

VkQueue_T* VulkanCommandQueue::GetVulkanCommandQueue()
{
	return _CommandQueue;
}
