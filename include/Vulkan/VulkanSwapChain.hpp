#pragma once

#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanHeader.hpp"
#include "Graphics/SwapChain.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class OutputDevice;
		class Device;
		class CommandQueue;
		class GraphicsContext;
		class View;
		class VulkanDevice;

		namespace VulkanPrivate
		{
			class EternalSwapChainDispatchLoader
			{
			public:

				EternalSwapChainDispatchLoader(_In_ VulkanDevice& InVulkanDevice);
				
				size_t getVkHeaderVersion() const;
				VkResult vkGetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) const VULKAN_HPP_NOEXCEPT;
				VkResult vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) const VULKAN_HPP_NOEXCEPT;
				VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) const VULKAN_HPP_NOEXCEPT;
				VkResult vkGetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const VULKAN_HPP_NOEXCEPT;
				VkResult vkGetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) const VULKAN_HPP_NOEXCEPT;

			private:
				PFN_vkGetPhysicalDeviceSurfaceSupportKHR		_vkGetPhysicalDeviceSurfaceSupportKHR;
				PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
				PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		_vkGetPhysicalDeviceSurfaceFormatsKHR;
				PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	_vkGetPhysicalDeviceSurfacePresentModesKHR;
				PFN_vkGetSwapchainImagesKHR						_vkGetSwapchainImagesKHR;
			};
		}

		class VulkanSwapChain : public SwapChain
		{
		public:
			VulkanSwapChain(_In_ GraphicsContext& InContext);
			~VulkanSwapChain();

			void InitializeVulkanSwapChain(_In_ GraphicsContext& InContext);

			virtual void Acquire(GraphicsContext& InContext) override final;
			virtual void Present(GraphicsContext& InContext) override final;

			const vk::SwapchainKHR& GetSwapChain() const { return _SwapChain; }
			const vk::SurfaceKHR& GetSurface() const { return _Surface; }

		protected:
			virtual void CreateSurface(_In_ GraphicsContext& InContext) = 0;

			vk::SurfaceKHR				_Surface;

		private:
			vk::SwapchainKHR			_SwapChain;
		};
	}
}

namespace VULKAN_HPP_NAMESPACE
{
	namespace detail
	{
		template <>
		struct isDispatchLoader<Eternal::Graphics::VulkanPrivate::EternalSwapChainDispatchLoader>
		{
			static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = true;
		};
	}
}

#endif
