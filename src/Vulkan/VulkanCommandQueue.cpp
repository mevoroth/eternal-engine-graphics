#include "Vulkan/VulkanCommandQueue.hpp"
#include <vulkan/vulkan.hpp>
#include "Vulkan/VulkanUtils.hpp"

//#include "Vulkan_deprecated/VulkanDevice.hpp"
//#include "Vulkan_deprecated/VulkanCommandList.hpp"
//#include "Vulkan_deprecated/VulkanFence.hpp"
//#include "Vulkan_deprecated/VulkanSwapChain.hpp"
//#include "Vulkan_deprecated/VulkanCommandAllocator.hpp"
//#include "Vulkan_deprecated/VulkanContext.hpp"

//#define VC_EXTRALEAN
//#define WIN32_LEAN_AND_MEAN
//#define WIN32_EXTRA_LEAN
//#include <Windows.h>

using namespace Eternal::Graphics;
using namespace std;

VulkanCommandQueue::VulkanCommandQueue(_In_ Device& DeviceObj, _In_ const CommandType& Type)
	: CommandQueue(DeviceObj, Type)
{
}

VulkanCommandQueue::~VulkanCommandQueue()
{
}

void VulkanCommandQueue::SubmitCommandLists(_In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount)
{
	//vk::PipelineStageFlags WaitDestStageMask = vk::PipelineStageFlagBits::eAllGraphics;
	//vk::SubmitInfo SubmitInfo(
	//	1, &CurrentSemaphore,
	//	&WaitDestStageMask,
	//	HasRequests ? 2 : 1, HasRequests ? SubmitCmdList : &VulkanCommandBuffer,
	//	1, &ReleaseSemaphore
	//);
	//Vulkan::VerifySuccess(VulkanQueue.submit(1, &SubmitInfo, SubmitFence));

	//SubmitInfo(uint32_t                                         waitSemaphoreCount_ = {},
	//	const VULKAN_HPP_NAMESPACE::Semaphore * pWaitSemaphores_ = {},
	//	const VULKAN_HPP_NAMESPACE::PipelineStageFlags * pWaitDstStageMask_ = {},
	//	uint32_t                                         commandBufferCount_ = {},
	//	const VULKAN_HPP_NAMESPACE::CommandBuffer * pCommandBuffers_ = {},
	//	uint32_t                                         signalSemaphoreCount_ = {},
	//	const VULKAN_HPP_NAMESPACE::Semaphore * pSignalSemaphores_ = {}) VULKAN_HPP_NOEXCEPT

	//vk::SubmitInfo SubmitInfo(

	//);

	//Vulkan::VerifySuccess(_CommandQueue.submit());
}
