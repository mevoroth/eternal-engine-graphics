#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanSwapChain.hpp"

#include "Windows/WindowsOutputDevice.hpp"
#include "Graphics/RenderPassFactory.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanFormat.hpp"
#include "Vulkan/VulkanFence.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanView.hpp"

namespace Eternal
{
	namespace Graphics
	{
		namespace VulkanPrivate
		{
			EternalSwapChainDispatchLoader::EternalSwapChainDispatchLoader(_In_ VulkanDevice& InVulkanDevice)
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
				
			size_t EternalSwapChainDispatchLoader::getVkHeaderVersion() const
			{
				return VK_HEADER_VERSION;
			}

			VkResult EternalSwapChainDispatchLoader::vkGetPhysicalDeviceSurfaceSupportKHR(_In_ VkPhysicalDevice InPhysicalDevice, _In_ uint32_t InQueueFamilyIndex, _In_ VkSurfaceKHR InSurface, _Out_ VkBool32* OutSupported) const VULKAN_HPP_NOEXCEPT
			{
				return _vkGetPhysicalDeviceSurfaceSupportKHR(InPhysicalDevice, InQueueFamilyIndex, InSurface, OutSupported);
			}

			VkResult EternalSwapChainDispatchLoader::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_In_ VkPhysicalDevice InPhysicalDevice, _In_ VkSurfaceKHR InSurface, _Out_ VkSurfaceCapabilitiesKHR* OutSurfaceCapabilities) const VULKAN_HPP_NOEXCEPT
			{
				return _vkGetPhysicalDeviceSurfaceCapabilitiesKHR(InPhysicalDevice, InSurface, OutSurfaceCapabilities);
			}

			VkResult EternalSwapChainDispatchLoader::vkGetPhysicalDeviceSurfaceFormatsKHR(_In_ VkPhysicalDevice InPhysicalDevice, _In_ VkSurfaceKHR InSurface, _Out_ uint32_t* OutSurfaceFormatCount, _Out_ VkSurfaceFormatKHR* OutSurfaceFormats) const VULKAN_HPP_NOEXCEPT
			{
				return _vkGetPhysicalDeviceSurfaceFormatsKHR(InPhysicalDevice, InSurface, OutSurfaceFormatCount, OutSurfaceFormats);
			}

			VkResult EternalSwapChainDispatchLoader::vkGetPhysicalDeviceSurfacePresentModesKHR(_In_ VkPhysicalDevice InPhysicalDevice, _In_ VkSurfaceKHR InSurface, uint32_t* OutPresentModeCount, VkPresentModeKHR* OutPresentModes) const VULKAN_HPP_NOEXCEPT
			{
				return _vkGetPhysicalDeviceSurfacePresentModesKHR(InPhysicalDevice, InSurface, OutPresentModeCount, OutPresentModes);
			}

			VkResult EternalSwapChainDispatchLoader::vkGetSwapchainImagesKHR(_In_ VkDevice InDevice, _In_ VkSwapchainKHR InSwapchain, _Out_ uint32_t* OutSwapchainImageCount, _Out_ VkImage* OutSwapchainImages) const VULKAN_HPP_NOEXCEPT
			{
				return _vkGetSwapchainImagesKHR(InDevice, InSwapchain, OutSwapchainImageCount, OutSwapchainImages);
			}
		}

		VulkanSwapChain::VulkanSwapChain(_In_ GraphicsContext& InContext)
			: SwapChain()
		{
		}

		VulkanSwapChain::~VulkanSwapChain()
		{

		}

		void VulkanSwapChain::InitializeVulkanSwapChain(_In_ GraphicsContext& InContext)
		{
			using namespace Vulkan;

			VulkanDevice& InVulkanDevice = static_cast<VulkanDevice&>(InContext.GetDevice());
			VulkanPrivate::EternalSwapChainDispatchLoader EternalLoader(InVulkanDevice);

			CreateSurface(InContext);

			vk::PhysicalDevice& VulkanPhysicalDevice = InVulkanDevice.GetPhysicalDevice();
			std::vector<vk::Bool32> SupportPresents;
			SupportPresents.resize(InVulkanDevice.GetQueueFamilyPropertiesCount());
			for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < InVulkanDevice.GetQueueFamilyPropertiesCount(); ++QueueFamilyIndex)
			{
				VerifySuccess(VulkanPhysicalDevice.getSurfaceSupportKHR<VulkanPrivate::EternalSwapChainDispatchLoader>(QueueFamilyIndex, _Surface, &SupportPresents[QueueFamilyIndex], EternalLoader));
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
				if (InContext.GetOutputDevice().GetVSync())
					HasValidMode |= PresentModes[PresentIndex] == vk::PresentModeKHR::eMailbox;
				else
					HasValidMode |= PresentModes[PresentIndex] == vk::PresentModeKHR::eFifo;
			}
			ETERNAL_ASSERT(HasValidMode);
#endif

			vk::CompositeAlphaFlagBitsKHR CompositeAlphaMode = [&SurfaceCapabilities]() -> vk::CompositeAlphaFlagBitsKHR
			{
				uint32_t LastCompositeFlagIndex = Math::Log2Fast(static_cast<uint32_t>(vk::CompositeAlphaFlagBitsKHR::eInherit));

				for (uint32_t CompositeAlphaFlagIndex = 0; CompositeAlphaFlagIndex <= LastCompositeFlagIndex; ++CompositeAlphaFlagIndex)
				{
					vk::CompositeAlphaFlagBitsKHR CurrentCompositeAlphaFlagBit = static_cast<vk::CompositeAlphaFlagBitsKHR>(1 << CompositeAlphaFlagIndex);
					if (SurfaceCapabilities.supportedCompositeAlpha & CurrentCompositeAlphaFlagBit)
						return CurrentCompositeAlphaFlagBit;
				}
				return vk::CompositeAlphaFlagBitsKHR();
			}();

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
				CompositeAlphaMode,
				InContext.GetOutputDevice().GetVSync() ? vk::PresentModeKHR::eFifo : vk::PresentModeKHR::eMailbox,
				true
			);

			vk::Device& VulkanDevice = InVulkanDevice.GetVulkanDevice();
			VerifySuccess(VulkanDevice.createSwapchainKHR(&SwapChainInfo, nullptr, &_SwapChain));

			vk::PhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties = VulkanPhysicalDevice.getMemoryProperties();

			uint32_t BackBuffersCount = 0u;
			VerifySuccess(VulkanDevice.getSwapchainImagesKHR(_SwapChain, &BackBuffersCount, static_cast<vk::Image*>(nullptr)));

			ETERNAL_ASSERT(BackBuffersCount == GraphicsContext::FrameBufferingCount);

			BackBuffersCount = GraphicsContext::FrameBufferingCount;

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
						ConvertVulkanFormatToFormat(Formats[0].format),
						ViewRenderTargetType::VIEW_RENDER_TARGET_TEXTURE_2D
					);

					_BackBufferRenderTargetViews[BackBufferIndex] = new VulkanView(CreateInformation);
				}
			}
		}

		void VulkanSwapChain::Acquire(GraphicsContext& InContext)
		{
			VulkanGraphicsContext& InVulkanGraphicsContext = Vulkan::VulkanGraphicsContextCast(InContext);

			VulkanCommandQueue& VkCommandQueue = static_cast<VulkanCommandQueue&>(InContext.GetGraphicsQueue());

			Vulkan::VerifySuccess(
				static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice().acquireNextImageKHR(
					GetSwapChain(),
					UINT64_MAX,
					InVulkanGraphicsContext.GetNextFrameSemaphore(),
					nullptr,
					&InVulkanGraphicsContext.GetVulkanSwapChainIndex()
				)
			);

			InContext.GetCurrentFrameIndex() = (InContext.GetCurrentFrameIndex() + 1) % GraphicsContext::FrameBufferingCount;
		}

		void VulkanSwapChain::Present(GraphicsContext& InContext)
		{
			VulkanGraphicsContext& InVulkanGraphicsContext = Vulkan::VulkanGraphicsContextCast(InContext);

			VulkanCommandQueue& VkCommandQueue = static_cast<VulkanCommandQueue&>(InContext.GetGraphicsQueue());

			vk::PresentInfoKHR PresentInfo(
				1u, &InVulkanGraphicsContext.GetCurrentSubmitSemaphore(),
				1u, &GetSwapChain(),
				&InVulkanGraphicsContext.GetVulkanSwapChainIndex()
			);
			Vulkan::VerifySuccess(
				static_cast<VulkanCommandQueue&>(InContext.GetGraphicsQueue()).GetVulkanCommandQueue().presentKHR(
					&PresentInfo
				)
			);
		}
	}
}

#endif
