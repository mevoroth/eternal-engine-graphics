#include "Windows/Vulkan/WindowsVulkanSwapChain.hpp"

#if ETERNAL_PLATFORM_WINDOWS && ETERNAL_ENABLE_VULKAN

#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Windows/WindowsOutputDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		WindowsVulkanSwapChain::WindowsVulkanSwapChain(_In_ GraphicsContext& InContext)
			: VulkanSwapChain(InContext)
		{
		}

		void WindowsVulkanSwapChain::CreateSurface(_In_ GraphicsContext& InContext)
		{
			using namespace Vulkan;

			VulkanDevice& InVulkanDevice = static_cast<VulkanDevice&>(InContext.GetDevice());
			WindowsOutputDevice& InOutputDevice = static_cast<WindowsOutputDevice&>(InContext.GetOutputDevice());

			vk::Win32SurfaceCreateInfoKHR Win32SurfaceInfo(
				vk::Win32SurfaceCreateFlagBitsKHR(),
				InOutputDevice.GetHInstance(),
				InOutputDevice.GetWindowHandler()
			);

			vk::Instance& VulkanInstance = InVulkanDevice.GetInstance();
			VerifySuccess(VulkanInstance.createWin32SurfaceKHR(&Win32SurfaceInfo, nullptr, &_Surface));
		}
	}
}

#endif
