<<<<<<< HEAD
#include "Vulkan/VulkanSwapChain.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Window/Window.hpp"
#include "Graphics/View.hpp"
#include "Graphics/Format.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanFrameBuffer.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanRenderPass.hpp"

using namespace Eternal::Graphics;

VulkanSwapChain::VulkanSwapChain(_In_ VulkanDevice& DeviceObj, _In_ Window& WindowObj)
{
	VkResult Result;

	PFN_vkGetPhysicalDeviceSurfaceSupportKHR		vkGetPhysicalDeviceSurfaceSupportKHR		= (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)		vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceSupportKHR");
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	vkGetPhysicalDeviceSurfaceCapabilitiesKHR	= (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)	vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		vkGetPhysicalDeviceSurfaceFormatsKHR		= (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)		vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceFormatsKHR");
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	vkGetPhysicalDeviceSurfacePresentModesKHR	= (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)	vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfacePresentModesKHR");
	PFN_vkGetSwapchainImagesKHR						vkGetSwapchainImagesKHR						= (PFN_vkGetSwapchainImagesKHR)						vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetSwapchainImagesKHR");
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

	Result = vkCreateWin32SurfaceKHR(DeviceObj.GetInstance(), &Win32SurfaceInfo, nullptr, &_Surface);
	ETERNAL_ASSERT(!Result);

	std::vector<VkBool32> SupportPresents;
	SupportPresents.resize(DeviceObj.GetQueueFamilyPropertiesCount());
	for (int QueueFamilyIndex = 0; QueueFamilyIndex < DeviceObj.GetQueueFamilyPropertiesCount(); ++QueueFamilyIndex)
	{
		Result = vkGetPhysicalDeviceSurfaceSupportKHR(DeviceObj.GetPhysicalDevice(), QueueFamilyIndex, _Surface, &SupportPresents[QueueFamilyIndex]);
		ETERNAL_ASSERT(!Result);
	}

	uint32_t FormatsCount = 0u;
	Result = vkGetPhysicalDeviceSurfaceFormatsKHR(DeviceObj.GetPhysicalDevice(), _Surface, &FormatsCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkSurfaceFormatKHR> Formats;
	Formats.resize(FormatsCount);
	Result = vkGetPhysicalDeviceSurfaceFormatsKHR(DeviceObj.GetPhysicalDevice(), _Surface, &FormatsCount, Formats.data());
	ETERNAL_ASSERT(!Result);

	VkSurfaceCapabilitiesKHR SurfaceCapabilities;
	Result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(DeviceObj.GetPhysicalDevice(), _Surface, &SurfaceCapabilities);
	ETERNAL_ASSERT(!Result);

	uint32_t PresentModesCount = 0u;
	Result = vkGetPhysicalDeviceSurfacePresentModesKHR(DeviceObj.GetPhysicalDevice(), _Surface, &PresentModesCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkPresentModeKHR> PresentModes;
	PresentModes.resize(PresentModesCount);
	Result = vkGetPhysicalDeviceSurfacePresentModesKHR(DeviceObj.GetPhysicalDevice(), _Surface, &PresentModesCount, PresentModes.data());
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

	Result = vkCreateSwapchainKHR(DeviceObj.GetDevice(), &SwapChainInfo, nullptr, &_SwapChain);
	ETERNAL_ASSERT(!Result);

	VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(DeviceObj.GetPhysicalDevice(), &PhysicalDeviceMemoryProperties);

	uint32_t BackBuffersCount = 0u;
	Result =  vkGetSwapchainImagesKHR(DeviceObj.GetDevice(), _SwapChain, &BackBuffersCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkImage> BackBuffers;
	BackBuffers.resize(BackBuffersCount);
	Result = vkGetSwapchainImagesKHR(DeviceObj.GetDevice(), _SwapChain, &BackBuffersCount, BackBuffers.data());
	ETERNAL_ASSERT(!Result);

	_BackBuffers.resize(BackBuffersCount);
	_BackBufferViews.resize(BackBuffersCount);

	for (uint32_t BackBufferIndex = 0; BackBufferIndex < BackBuffers.size(); ++BackBufferIndex)
	{
		_BackBufferViews[BackBufferIndex] = new VulkanView(DeviceObj, BackBuffers[BackBufferIndex], TEXTURE_VIEW_TYPE_2D, BGRA8888);
	}

	vector<VulkanView*> RenderTargets;
	RenderTargets.push_back(_BackBufferViews[0]);
	_RenderPass = new VulkanRenderPass(DeviceObj, RenderTargets);
	_RenderPass->Initialize();

	for (uint32_t BackBufferIndex = 0; BackBufferIndex < BackBuffers.size(); ++BackBufferIndex)
	{
		_BackBuffers[BackBufferIndex] = new VulkanFrameBuffer(DeviceObj, *_RenderPass, BackBuffers[BackBufferIndex], *_BackBufferViews[BackBufferIndex], WindowObj.GetWidth(), WindowObj.GetHeight());
	}
}

VulkanFrameBuffer& VulkanSwapChain::GetBackBuffer(_In_ uint32_t BackBufferIndex)
{
	ETERNAL_ASSERT(BackBufferIndex < _BackBuffers.size());
	return *_BackBuffers[BackBufferIndex];
}
=======
#include "Vulkan/VulkanSwapChain.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "Macros/Macros.hpp"
#include "Window/Window.hpp"
#include "Graphics/View.hpp"
#include "Graphics/Format.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanFrameBuffer.hpp"
#include "Vulkan/VulkanView.hpp"

using namespace Eternal::Graphics;

VulkanSwapChain::VulkanSwapChain(_In_ VulkanDevice& DeviceObj, _In_ Window& WindowObj)
{
	VkResult Result;

	PFN_vkGetPhysicalDeviceSurfaceSupportKHR		vkGetPhysicalDeviceSurfaceSupportKHR		= (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)		vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceSupportKHR");
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	vkGetPhysicalDeviceSurfaceCapabilitiesKHR	= (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)	vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		vkGetPhysicalDeviceSurfaceFormatsKHR		= (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)		vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfaceFormatsKHR");
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	vkGetPhysicalDeviceSurfacePresentModesKHR	= (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)	vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetPhysicalDeviceSurfacePresentModesKHR");
	PFN_vkGetSwapchainImagesKHR						vkGetSwapchainImagesKHR						= (PFN_vkGetSwapchainImagesKHR)						vkGetInstanceProcAddr(DeviceObj.GetInstance(), "vkGetSwapchainImagesKHR");
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

	Result = vkCreateWin32SurfaceKHR(DeviceObj.GetInstance(), &Win32SurfaceInfo, nullptr, &_Surface);
	ETERNAL_ASSERT(!Result);

	std::vector<VkBool32> SupportPresents;
	SupportPresents.resize(DeviceObj.GetQueueFamilyPropertiesCount());
	for (int QueueFamilyIndex = 0; QueueFamilyIndex < DeviceObj.GetQueueFamilyPropertiesCount(); ++QueueFamilyIndex)
	{
		Result = vkGetPhysicalDeviceSurfaceSupportKHR(DeviceObj.GetPhysicalDevice(), QueueFamilyIndex, _Surface, &SupportPresents[QueueFamilyIndex]);
		ETERNAL_ASSERT(!Result);
	}

	uint32_t FormatsCount = 0u;
	Result = vkGetPhysicalDeviceSurfaceFormatsKHR(DeviceObj.GetPhysicalDevice(), _Surface, &FormatsCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkSurfaceFormatKHR> Formats;
	Formats.resize(FormatsCount);
	Result = vkGetPhysicalDeviceSurfaceFormatsKHR(DeviceObj.GetPhysicalDevice(), _Surface, &FormatsCount, Formats.data());
	ETERNAL_ASSERT(!Result);

	VkSurfaceCapabilitiesKHR SurfaceCapabilities;
	Result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(DeviceObj.GetPhysicalDevice(), _Surface, &SurfaceCapabilities);
	ETERNAL_ASSERT(!Result);

	uint32_t PresentModesCount = 0u;
	Result = vkGetPhysicalDeviceSurfacePresentModesKHR(DeviceObj.GetPhysicalDevice(), _Surface, &PresentModesCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkPresentModeKHR> PresentModes;
	PresentModes.resize(PresentModesCount);
	Result = vkGetPhysicalDeviceSurfacePresentModesKHR(DeviceObj.GetPhysicalDevice(), _Surface, &PresentModesCount, PresentModes.data());
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

	Result = vkCreateSwapchainKHR(DeviceObj.GetDevice(), &SwapChainInfo, nullptr, &_SwapChain);
	ETERNAL_ASSERT(!Result);

	VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(DeviceObj.GetPhysicalDevice(), &PhysicalDeviceMemoryProperties);

	uint32_t BackBuffersCount = 0u;
	Result =  vkGetSwapchainImagesKHR(DeviceObj.GetDevice(), _SwapChain, &BackBuffersCount, nullptr);
	ETERNAL_ASSERT(!Result);

	std::vector<VkImage> BackBuffers;
	BackBuffers.resize(BackBuffersCount);
	Result = vkGetSwapchainImagesKHR(DeviceObj.GetDevice(), _SwapChain, &BackBuffersCount, BackBuffers.data());
	ETERNAL_ASSERT(!Result);

	_BackBuffers.resize(BackBuffersCount);
	_BackBufferViews.resize(BackBuffersCount);

	for (uint32_t BackBufferIndex = 0; BackBufferIndex < BackBuffers.size(); ++BackBufferIndex)
	{
		_BackBufferViews[BackBufferIndex] = new VulkanView(DeviceObj, BackBuffers[BackBufferIndex], TEXTURE_VIEW_TYPE_2D, BGRA8888);
		_BackBuffers[BackBufferIndex] = new VulkanFrameBuffer(DeviceObj, BackBuffers[BackBufferIndex], *_BackBufferViews[BackBufferIndex], WindowObj.GetWidth(), WindowObj.GetHeight());
	}
}

VulkanFrameBuffer& VulkanSwapChain::GetBackBuffer(_In_ uint32_t BackBufferIndex)
{
	ETERNAL_ASSERT(BackBufferIndex < _BackBuffers.size());
	return *_BackBuffers[BackBufferIndex];
}
>>>>>>> 4a6e8cf3caa9bc469d4cbe28b1c95167a2572cce