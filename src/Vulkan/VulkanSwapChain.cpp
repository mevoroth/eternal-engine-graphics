#include "Vulkan/VulkanSwapChain.hpp"

#include "Macros/Macros.hpp"
#include "Window/Window.hpp"
#include "Graphics/View.hpp"
#include "Graphics/Format.hpp"
#include "Graphics/BlendState.hpp"
#include "Graphics/BlendStateFactory.hpp"
#include "Graphics/RenderPassFactory.hpp"
#include "NextGenGraphics/Context.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanRenderTarget.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanCommandAllocator.hpp"
#include "Vulkan/VulkanContext.hpp"

using namespace Eternal::Graphics;

namespace Eternal
{
	namespace Graphics
	{
		namespace VulkanPrivate
		{
			class EternalDispatchLoader
			{
			public:
				EternalDispatchLoader(VulkanDevice& InVulkanDevice)
				{
					_vkGetPhysicalDeviceSurfaceSupportKHR		= (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)		InVulkanDevice.GetInstance().getProcAddr("vkGetPhysicalDeviceSurfaceSupportKHR");
					_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	= (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)	InVulkanDevice.GetInstance().getProcAddr("vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
					_vkGetPhysicalDeviceSurfaceFormatsKHR		= (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)		InVulkanDevice.GetInstance().getProcAddr("vkGetPhysicalDeviceSurfaceFormatsKHR");
					_vkGetPhysicalDeviceSurfacePresentModesKHR	= (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)	InVulkanDevice.GetInstance().getProcAddr("vkGetPhysicalDeviceSurfacePresentModesKHR");
					_vkGetSwapchainImagesKHR					= (PFN_vkGetSwapchainImagesKHR)						InVulkanDevice.GetInstance().getProcAddr("vkGetSwapchainImagesKHR");
					ETERNAL_ASSERT(_vkGetPhysicalDeviceSurfaceSupportKHR);
					ETERNAL_ASSERT(_vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
					ETERNAL_ASSERT(_vkGetPhysicalDeviceSurfaceFormatsKHR);
					ETERNAL_ASSERT(_vkGetPhysicalDeviceSurfacePresentModesKHR);
					ETERNAL_ASSERT(_vkGetSwapchainImagesKHR);
				}

				VkResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) const VULKAN_HPP_NOEXCEPT
				{
					return _vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
				}

				VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) const VULKAN_HPP_NOEXCEPT
				{
					return _vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
				}

				VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) const VULKAN_HPP_NOEXCEPT
				{
					return _vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
				}

				VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const VULKAN_HPP_NOEXCEPT
				{
					return _vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
				}

				VkResult vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) const VULKAN_HPP_NOEXCEPT
				{
					return _vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
				}

			private:
				PFN_vkGetPhysicalDeviceSurfaceSupportKHR		_vkGetPhysicalDeviceSurfaceSupportKHR;
				PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
				PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		_vkGetPhysicalDeviceSurfaceFormatsKHR;
				PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	_vkGetPhysicalDeviceSurfacePresentModesKHR;
				PFN_vkGetSwapchainImagesKHR						_vkGetSwapchainImagesKHR;
			};
		}
	}
}

VulkanSwapChain::VulkanSwapChain(_In_ Device& DeviceObj, _In_ Window& WindowObj)
	: SwapChain(WindowObj)
{
	using namespace Vulkan;

	VulkanDevice& VulkanDeviceObj = static_cast<VulkanDevice&>(DeviceObj);
	VulkanPrivate::EternalDispatchLoader EternalLoader(VulkanDeviceObj);

	vk::Win32SurfaceCreateInfoKHR Win32SurfaceInfo(
		vk::Win32SurfaceCreateFlagBitsKHR(),
		WindowObj.GetHInstance(),
		WindowObj.GetWindowHandler()
	);

	vk::Instance& VulkanInstance = VulkanDeviceObj.GetInstance();
	VerifySuccess(VulkanInstance.createWin32SurfaceKHR(&Win32SurfaceInfo, nullptr, &_Surface));

	vk::PhysicalDevice& VulkanPhysicalDevice = VulkanDeviceObj.GetPhysicalDevice();
	std::vector<vk::Bool32> SupportPresents;
	SupportPresents.resize(VulkanDeviceObj.GetQueueFamilyPropertiesCount());
	for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < VulkanDeviceObj.GetQueueFamilyPropertiesCount(); ++QueueFamilyIndex)
	{
		VerifySuccess(VulkanPhysicalDevice.getSurfaceSupportKHR(QueueFamilyIndex, _Surface, &SupportPresents[QueueFamilyIndex], EternalLoader));
	}

	uint32_t FormatsCount = 0u;
	VerifySuccess(VulkanPhysicalDevice.getSurfaceFormatsKHR(_Surface, &FormatsCount, static_cast<vk::SurfaceFormatKHR*>(nullptr), EternalLoader));

	std::vector<vk::SurfaceFormatKHR> Formats;
	Formats.resize(FormatsCount);
	VerifySuccess(VulkanPhysicalDevice.getSurfaceFormatsKHR(_Surface, &FormatsCount, Formats.data(), EternalLoader));

	vk::SurfaceCapabilitiesKHR SurfaceCapabilities;
	VerifySuccess(VulkanPhysicalDevice.getSurfaceCapabilitiesKHR(_Surface, &SurfaceCapabilities, EternalLoader));

	uint32_t PresentModesCount = 0u;
	VerifySuccess(VulkanPhysicalDevice.getSurfacePresentModesKHR(_Surface, &PresentModesCount, static_cast<vk::PresentModeKHR*>(nullptr), EternalLoader));

	std::vector<vk::PresentModeKHR> PresentModes;
	PresentModes.resize(PresentModesCount);
	VerifySuccess(VulkanPhysicalDevice.getSurfacePresentModesKHR(_Surface, &PresentModesCount, PresentModes.data(), EternalLoader));

	vk::SwapchainCreateInfoKHR SwapChainInfo(
		vk::SwapchainCreateFlagBitsKHR(),
		_Surface,
		SurfaceCapabilities.minImageCount,
		Formats[0].format,
		Formats[0].colorSpace,
		SurfaceCapabilities.currentExtent,
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive,
		0, nullptr,
		vk::SurfaceTransformFlagBitsKHR::eIdentity,
		vk::CompositeAlphaFlagBitsKHR::eOpaque,
		PresentModes[0],
		true
	);

	vk::Device& VulkanDevice = VulkanDeviceObj.GetVulkanDevice();
	VerifySuccess(VulkanDevice.createSwapchainKHR(&SwapChainInfo, nullptr, &_SwapChain));

	vk::PhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties = VulkanPhysicalDevice.getMemoryProperties();

	uint32_t BackBuffersCount = 0u;
	VerifySuccess(VulkanDevice.getSwapchainImagesKHR(_SwapChain, &BackBuffersCount, static_cast<vk::Image*>(nullptr)));

	//std::vector<vk::Image> BackBuffers;
	//BackBuffers.resize(BackBuffersCount);
	_BackBufferImages.resize(BackBuffersCount);
	VerifySuccess(VulkanDevice.getSwapchainImagesKHR(_SwapChain, &BackBuffersCount, _BackBufferImages.data()));

	//_BackBuffers.resize(BackBuffersCount);
	_BackBufferViews.resize(BackBuffersCount);
	//_AcquireSemaphores.resize(BackBuffersCount);

	for (uint32_t BackBufferIndex = 0; BackBufferIndex < _BackBufferImages.size(); ++BackBufferIndex)
	{
		_BackBufferViews[BackBufferIndex] = new VulkanView(VulkanDeviceObj, _BackBufferImages[BackBufferIndex], TEXTURE_VIEW_TYPE_2D, FORMAT_BGRA8888);
		//_BackBuffers[BackBufferIndex] = new VulkanResource(BackBuffers[BackBufferIndex]);
		
		//VkSemaphoreCreateInfo SemaphoreInfo;
		//SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		//SemaphoreInfo.pNext = nullptr;
		//SemaphoreInfo.flags = 0;

		//Result = vkCreateSemaphore(VulkanDeviceObj.GetVulkanDevice(), &SemaphoreInfo, nullptr, &_AcquireSemaphores[BackBufferIndex]);
		//ETERNAL_ASSERT(!Result);
	}

	//vector<View*> RenderTargets;
	//RenderTargets.resize(1);

	//vector<BlendState*> BlendStates;
	//BlendStates.push_back(&GetBackBufferBlendState());

	//_RenderPasses.resize(GetBackBuffersFrameCount());
	//for (uint32_t RenderPassIndex = 0; RenderPassIndex < GetBackBuffersFrameCount(); ++RenderPassIndex)
	//{
	//	RenderTargets[0]				= _BackBufferViews[RenderPassIndex];
	//	_RenderPasses[RenderPassIndex]	= static_cast<VulkanRenderPass*>(CreateRenderPass(DeviceObj, GetMainViewport(), RenderTargets, BlendStates));
	//}
}

void VulkanSwapChain::AcquireFrame(_In_ Device& DeviceObj, _Inout_ Context& GfxContext)
{
	//uint32_t FrameIndex;

	//VkFence		FrameFence		= static_cast<VulkanFence*>(GfxContext.GetFrameFence())->GetVulkanFence();
	//VkSemaphore	FrameSemaphore	= static_cast<VulkanContext&>(GfxContext).GetFrameSemaphore();

	//VkResult Result = vkAcquireNextImageKHR(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), _SwapChain, UINT64_MAX, FrameSemaphore, VK_NULL_HANDLE, &FrameIndex);
	//ETERNAL_ASSERT(!Result);
	//ETERNAL_ASSERT(_FrameIndex == FrameIndex);

	//GfxContext.SetFrameIndex(FrameIndex);
	//GfxContext.SetFrameRenderPass(_RenderPasses[FrameIndex]);
	//GfxContext.SetFrameBackBuffer(_BackBuffers[FrameIndex]);
}

uint32_t VulkanSwapChain::GetBackBuffersFrameCount() const
{
	return (uint32_t)_BackBuffers.size();
}

//VkSemaphore_T*& VulkanSwapChain::GetAcquireSemaphore(_In_ uint32_t ResourceIndex)
//{
//	ETERNAL_ASSERT(ResourceIndex < _AcquireSemaphores.size());
//	return _AcquireSemaphores[ResourceIndex];
//}

RenderPass& VulkanSwapChain::GetMainRenderPass()
{
	return *_RenderPasses[0];
}

void VulkanSwapChain::Present(_In_ Device& DeviceObj, _In_ CommandQueue& CommandQueueObj, _Inout_ Context& GfxContext)
{
	//uint32_t ImageIndex			= GfxContext.GetFrameIndex();
	//VulkanContext& VkContext	= static_cast<VulkanContext&>(GfxContext);
	//VkResult PresentInfoResult;

	//VkPresentInfoKHR PresentInfo;
	//PresentInfo.sType					= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//PresentInfo.pNext					= nullptr;
	//PresentInfo.waitSemaphoreCount		= (uint32_t)VkContext.GetFrameCommandListSemaphores().size();
	//PresentInfo.pWaitSemaphores			= VkContext.GetFrameCommandListSemaphores().data();
	//PresentInfo.swapchainCount			= 1;
	//PresentInfo.pSwapchains				= &_SwapChain;
	//PresentInfo.pImageIndices			= &ImageIndex;
	//PresentInfo.pResults				= &PresentInfoResult;

	////char test[256];
	////sprintf_s(test, "[VulkanSwapChain::Present] FENCE: %d\n", ResourceIndex);
	////OutputDebugString(test);

	//VkResult Result = vkQueuePresentKHR(static_cast<VulkanCommandQueue&>(CommandQueueObj).GetVulkanCommandQueue(), &PresentInfo);
	//ETERNAL_ASSERT(!PresentInfoResult);
	//ETERNAL_ASSERT(!Result);

	//_FrameIndex = (_FrameIndex + 1) % _BackBuffers.size();
}
