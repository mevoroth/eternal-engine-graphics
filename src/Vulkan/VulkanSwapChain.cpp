#include "Vulkan/VulkanSwapChain.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Window/Window.hpp"
#include "Graphics/View.hpp"
#include "Graphics/Format.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanRenderTarget.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"

using namespace Eternal::Graphics;

VulkanSwapChain::VulkanSwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj)
{
	VkResult Result;

	VulkanDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(DeviceObj);

	PFN_vkGetPhysicalDeviceSurfaceSupportKHR		vkGetPhysicalDeviceSurfaceSupportKHR		= (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)		vkGetInstanceProcAddr(VulkanDeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceSupportKHR");
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	vkGetPhysicalDeviceSurfaceCapabilitiesKHR	= (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)	vkGetInstanceProcAddr(VulkanDeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		vkGetPhysicalDeviceSurfaceFormatsKHR		= (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)		vkGetInstanceProcAddr(VulkanDeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceFormatsKHR");
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	vkGetPhysicalDeviceSurfacePresentModesKHR	= (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)	vkGetInstanceProcAddr(VulkanDeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfacePresentModesKHR");
	PFN_vkGetSwapchainImagesKHR						vkGetSwapchainImagesKHR						= (PFN_vkGetSwapchainImagesKHR)						vkGetInstanceProcAddr(VulkanDeviceObj.GetInstance(), "vkGetSwapchainImagesKHR");
	ETERNAL_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR);
	ETERNAL_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	ETERNAL_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR);
	ETERNAL_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR);
	ETERNAL_ASSERT(vkGetSwapchainImagesKHR);

	VkWin32SurfaceCreateInfoKHR Win32SurfaceInfo;
	Win32SurfaceInfo.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	Win32SurfaceInfo.pNext		= nullptr;
	Win32SurfaceInfo.flags		= 0;
	Win32SurfaceInfo.hinstance	= WindowObj.GetHInstance();
	Win32SurfaceInfo.hwnd		= WindowObj.GetWindowHandler();

	Result = vkCreateWin32SurfaceKHR(VulkanDeviceObj.GetInstance(), &Win32SurfaceInfo, nullptr, &_Surface);
	ETERNAL_ASSERT(!Result);

	std::vector<VkBool32> SupportPresents;
	SupportPresents.resize(VulkanDeviceObj.GetQueueFamilyPropertiesCount());
	for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < VulkanDeviceObj.GetQueueFamilyPropertiesCount(); ++QueueFamilyIndex)
	{
		Result = vkGetPhysicalDeviceSurfaceSupportKHR(VulkanDeviceObj.GetPhysicalDevice(), QueueFamilyIndex, _Surface, &SupportPresents[QueueFamilyIndex]);
		ETERNAL_ASSERT(!Result);
	}

	uint32_t FormatsCount = 0u;
	Result = vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanDeviceObj.GetPhysicalDevice(), _Surface, &FormatsCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkSurfaceFormatKHR> Formats;
	Formats.resize(FormatsCount);
	Result = vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanDeviceObj.GetPhysicalDevice(), _Surface, &FormatsCount, Formats.data());
	ETERNAL_ASSERT(!Result);

	VkSurfaceCapabilitiesKHR SurfaceCapabilities;
	Result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanDeviceObj.GetPhysicalDevice(), _Surface, &SurfaceCapabilities);
	ETERNAL_ASSERT(!Result);

	uint32_t PresentModesCount = 0u;
	Result = vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanDeviceObj.GetPhysicalDevice(), _Surface, &PresentModesCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkPresentModeKHR> PresentModes;
	PresentModes.resize(PresentModesCount);
	Result = vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanDeviceObj.GetPhysicalDevice(), _Surface, &PresentModesCount, PresentModes.data());
	ETERNAL_ASSERT(!Result);

	VkSwapchainCreateInfoKHR SwapChainInfo;
	SwapChainInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChainInfo.pNext					= nullptr;
	SwapChainInfo.flags					= 0;
	SwapChainInfo.surface				= _Surface;
	SwapChainInfo.minImageCount			= SurfaceCapabilities.minImageCount;
	SwapChainInfo.imageFormat			= Formats[0].format;
	SwapChainInfo.imageColorSpace		= Formats[0].colorSpace;
	SwapChainInfo.imageExtent			= SurfaceCapabilities.currentExtent;
	SwapChainInfo.imageArrayLayers		= 1;
	SwapChainInfo.imageUsage			= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	SwapChainInfo.queueFamilyIndexCount	= 0;
	SwapChainInfo.pQueueFamilyIndices	= nullptr;
	SwapChainInfo.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
	SwapChainInfo.preTransform			= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	SwapChainInfo.compositeAlpha		= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapChainInfo.presentMode			= PresentModes[0];
	SwapChainInfo.clipped				= true;
	SwapChainInfo.oldSwapchain			= VK_NULL_HANDLE;

	Result = vkCreateSwapchainKHR(VulkanDeviceObj.GetVulkanDevice(), &SwapChainInfo, nullptr, &_SwapChain);
	ETERNAL_ASSERT(!Result);

	VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(VulkanDeviceObj.GetPhysicalDevice(), &PhysicalDeviceMemoryProperties);

	uint32_t BackBuffersCount = 0u;
	Result =  vkGetSwapchainImagesKHR(VulkanDeviceObj.GetVulkanDevice(), _SwapChain, &BackBuffersCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkImage> BackBuffers;
	BackBuffers.resize(BackBuffersCount);
	Result = vkGetSwapchainImagesKHR(VulkanDeviceObj.GetVulkanDevice(), _SwapChain, &BackBuffersCount, BackBuffers.data());
	ETERNAL_ASSERT(!Result);

	_BackBuffers.resize(BackBuffersCount);
	_BackBufferViews.resize(BackBuffersCount);
	_AcquireSemaphores.resize(BackBuffersCount);

	std::vector<View*> VkViews;
	VkViews.resize(BackBuffersCount);

	for (uint32_t BackBufferIndex = 0; BackBufferIndex < BackBuffers.size(); ++BackBufferIndex)
	{
		VkViews[BackBufferIndex] = new VulkanView(VulkanDeviceObj, BackBuffers[BackBufferIndex], TEXTURE_VIEW_TYPE_2D, FORMAT_BGRA8888);
		//_BackBufferViews[BackBufferIndex] = new VulkanRenderTarget(DeviceObj, ;
		_BackBuffers[BackBufferIndex] = new VulkanResource(BackBuffers[BackBufferIndex]);
	}

	vector<View*> RenderTargets;
	RenderTargets.push_back(VkViews[0]);
	_RenderPass = new VulkanRenderPass(VulkanDeviceObj, RenderTargets);

	for (uint32_t BackBufferIndex = 0; BackBufferIndex < BackBuffers.size(); ++BackBufferIndex)
	{
		_BackBufferViews[BackBufferIndex] = new VulkanRenderTarget(DeviceObj, *_RenderPass, &VkViews[BackBufferIndex], 1u, (uint32_t)WindowObj.GetWidth(), (uint32_t)WindowObj.GetHeight());
		
		VkSemaphoreCreateInfo SemaphoreInfo;
		SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		SemaphoreInfo.pNext = nullptr;
		SemaphoreInfo.flags = 0;

		Result = vkCreateSemaphore(VulkanDeviceObj.GetVulkanDevice(), &SemaphoreInfo, nullptr, &_AcquireSemaphores[BackBufferIndex]);
		ETERNAL_ASSERT(!Result);
	}
}

Resource& VulkanSwapChain::GetBackBuffer(_In_ uint32_t BackBufferIndex)
{
	ETERNAL_ASSERT(BackBufferIndex < _BackBuffers.size());
	return *_BackBuffers[BackBufferIndex];
}

View& VulkanSwapChain::GetBackBufferView(_In_ uint32_t BackBufferIndex)
{
	ETERNAL_ASSERT(BackBufferIndex < _BackBufferViews.size());
	return *_BackBufferViews[BackBufferIndex];
}

uint32_t VulkanSwapChain::AcquireFrame(_In_ Device& DeviceObj, _In_ Fence& FenceObj)
{
	uint32_t FrameIndex;
	//FenceObj.Reset(DeviceObj);
	//VkResult Result = vkAcquireNextImageKHR(DeviceObj.GetDevice(), _SwapChain, UINT64_MAX, _AcquireSemaphores[_FrameIndex], nullptr, &FrameIndex);

	VkResult Result = vkAcquireNextImageKHR(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), _SwapChain, UINT64_MAX, _AcquireSemaphores[_FrameIndex], static_cast<VulkanFence&>(FenceObj).GetFence(), &FrameIndex);
	ETERNAL_ASSERT(!Result);
	ETERNAL_ASSERT(_FrameIndex == FrameIndex);
	
	char test[256];
	sprintf_s(test, "[VulkanSwapChain::AcquireFrame] FENCE: %d:%d\n", _FrameIndex, FrameIndex);
	OutputDebugString(test);

	return FrameIndex;
}

uint32_t VulkanSwapChain::GetBackBuffersFrameCount() const
{
	return (uint32_t)_BackBuffers.size();
}

VkSemaphore_T*& VulkanSwapChain::GetAcquireSemaphore(_In_ uint32_t ResourceIndex)
{
	ETERNAL_ASSERT(ResourceIndex < _AcquireSemaphores.size());
	return _AcquireSemaphores[ResourceIndex];
}

RenderPass& VulkanSwapChain::GetMainRenderPass()
{
	return *_RenderPass;
}

void VulkanSwapChain::Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _In_ uint32_t ResourceIndex)
{
	VulkanCommandQueue& VulkanCommandQueueObj			= static_cast<VulkanCommandQueue&>(CommandQueueObj);
	VulkanCommandAllocator* VulkanCommandAllocatorObj	= static_cast<VulkanCommandAllocator*>(VulkanCommandQueueObj.GetCommandAllocator(ResourceIndex));
	uint32_t ImageIndice = ResourceIndex;
	VkResult PresentInfoResult;
	
	VkPresentInfoKHR PresentInfo;
	PresentInfo.sType					= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	PresentInfo.pNext					= nullptr;
	PresentInfo.waitSemaphoreCount		= 1;
	PresentInfo.pWaitSemaphores			= &VulkanCommandAllocatorObj->GetSemaphore();
	PresentInfo.swapchainCount			= 1;
	PresentInfo.pSwapchains				= &_SwapChain;
	PresentInfo.pImageIndices			= &ImageIndice;
	PresentInfo.pResults				= &PresentInfoResult;

	char test[256];
	sprintf_s(test, "[VulkanSwapChain::Present] FENCE: %d\n", ResourceIndex);
	OutputDebugString(test);

	VkResult Result = vkQueuePresentKHR(VulkanCommandQueueObj.GetVulkanCommandQueue(), &PresentInfo);
	ETERNAL_ASSERT(!PresentInfoResult);
	ETERNAL_ASSERT(!Result);

	_FrameIndex = (_FrameIndex + 1) % _BackBuffers.size();
}
