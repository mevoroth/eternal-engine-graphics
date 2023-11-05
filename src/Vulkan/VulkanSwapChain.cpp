#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanSwapChain.hpp"

#include "Windows/WindowsOutputDevice.hpp"
#include "Graphics/RenderPassFactory.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Graphics/Format.hpp"

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
				
				size_t getVkHeaderVersion() const
				{
					return VK_HEADER_VERSION;
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

		VulkanSwapChain::VulkanSwapChain(_In_ GraphicsContext& InContext)
			: SwapChain()
		{
			using namespace Vulkan;

			WindowsOutputDevice& InOutputDevice = static_cast<WindowsOutputDevice&>(InContext.GetOutputDevice());

			VulkanDevice& InVulkanDevice = static_cast<VulkanDevice&>(InContext.GetDevice());
			VulkanPrivate::EternalDispatchLoader EternalLoader(InVulkanDevice);

			vk::Win32SurfaceCreateInfoKHR Win32SurfaceInfo(
				vk::Win32SurfaceCreateFlagBitsKHR(),
				InOutputDevice.GetHInstance(),
				InOutputDevice.GetWindowHandler()
			);

			vk::Instance& VulkanInstance = InVulkanDevice.GetInstance();
			VerifySuccess(VulkanInstance.createWin32SurfaceKHR(&Win32SurfaceInfo, nullptr, &_Surface));

			vk::PhysicalDevice& VulkanPhysicalDevice = InVulkanDevice.GetPhysicalDevice();
			std::vector<vk::Bool32> SupportPresents;
			SupportPresents.resize(InVulkanDevice.GetQueueFamilyPropertiesCount());
			for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < InVulkanDevice.GetQueueFamilyPropertiesCount(); ++QueueFamilyIndex)
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

#if ETERNAL_DEBUG
			bool HasValidMode = false;
			for (uint32_t PresentIndex = 0; PresentIndex < PresentModesCount; ++PresentIndex)
			{
				if (InOutputDevice.GetVSync())
					HasValidMode |= PresentModes[PresentIndex] == vk::PresentModeKHR::eMailbox;
				else
					HasValidMode |= PresentModes[PresentIndex] == vk::PresentModeKHR::eFifo;
			}
			ETERNAL_ASSERT(HasValidMode);
#endif

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
				InOutputDevice.GetVSync() ? vk::PresentModeKHR::eFifo : vk::PresentModeKHR::eMailbox,
				true
			);

			vk::Device& VulkanDevice = InVulkanDevice.GetVulkanDevice();
			VerifySuccess(VulkanDevice.createSwapchainKHR(&SwapChainInfo, nullptr, &_SwapChain));

			vk::PhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties = VulkanPhysicalDevice.getMemoryProperties();

			uint32_t BackBuffersCount = 0u;
			VerifySuccess(VulkanDevice.getSwapchainImagesKHR(_SwapChain, &BackBuffersCount, static_cast<vk::Image*>(nullptr)));

			ETERNAL_ASSERT(BackBuffersCount == GraphicsContext::FrameBufferingCount);

			_BackBuffers.resize(BackBuffersCount);
			_BackBufferRenderTargetViews.resize(BackBuffersCount);
			vector<vk::Image> BackBufferImages;
			BackBufferImages.resize(BackBuffersCount);
			VerifySuccess(VulkanDevice.getSwapchainImagesKHR(_SwapChain, &BackBuffersCount, BackBufferImages.data()));

			for (uint32_t BackBufferIndex = 0; BackBufferIndex < BackBuffersCount; ++BackBufferIndex)
			{
				{
					std::string BackBufferName = "BackBuffer" + std::to_string(BackBufferIndex);
					VulkanResourceBackBufferCreateInformation CreateInformation(
						InContext.GetDevice(),
						BackBufferName,
						BackBufferImages[BackBufferIndex]
					);

					_BackBuffers[BackBufferIndex] = new VulkanResource(CreateInformation);
				}

				{
					ViewMetaData MetaData;
					RenderTargetViewCreateInformation CreateInformation(
						InContext,
						_BackBuffers[BackBufferIndex],
						MetaData,
						Format::FORMAT_BGRA8888_UNORM,
						ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D
					);

					_BackBufferRenderTargetViews[BackBufferIndex] = new VulkanView(CreateInformation);
				}
			}
		}

		VulkanSwapChain::~VulkanSwapChain()
		{

		}

		void VulkanSwapChain::Acquire(GraphicsContext& Context)
		{
			VulkanGraphicsContext& GfxContext = static_cast<VulkanGraphicsContext&>(Context);

			Vulkan::VerifySuccess(
				static_cast<VulkanDevice&>(Context.GetDevice()).GetVulkanDevice().acquireNextImageKHR(
					GetSwapChain(),
					UINT64_MAX,
					GfxContext.GetNextFrameSemaphore(),
					nullptr,
					&GfxContext.GetCurrentFrameIndex()
				)
			);
		}

		void VulkanSwapChain::Present(GraphicsContext& Context)
		{
			VulkanGraphicsContext& GfxContext = static_cast<VulkanGraphicsContext&>(Context);

			VulkanCommandQueue& VkCommandQueue = static_cast<VulkanCommandQueue&>(Context.GetGraphicsQueue());

			vk::Semaphore* SubmitCompletionSemaphores	= nullptr;
			uint32_t SubmitCompletionSemaphoresCount	= 0;
			VkCommandQueue.GetSubmitCompletionSemaphoresAndReset(SubmitCompletionSemaphores, SubmitCompletionSemaphoresCount);

			vk::PresentInfoKHR PresentInfo(
				SubmitCompletionSemaphoresCount, SubmitCompletionSemaphores,
				1, &GetSwapChain(),
				&GfxContext.GetCurrentFrameIndex()
			);
			Vulkan::VerifySuccess(
				static_cast<VulkanCommandQueue&>(Context.GetGraphicsQueue()).GetVulkanCommandQueue().presentKHR(
					&PresentInfo
				)
			);
		}
	}
}

#endif
