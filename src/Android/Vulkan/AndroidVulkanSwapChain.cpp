#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN

#include "Android/Vulkan/AndroidVulkanSwapChain.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanDevice.hpp"

namespace Eternal
{
	namespace Graphics
	{
		AndroidVulkanSwapChain::AndroidVulkanSwapChain(_In_ GraphicsContext& InContext)
			: VulkanSwapChain(InContext)
		{
		}

		void AndroidVulkanSwapChain::CreateSurface(_In_ GraphicsContext& InContext)
		{
			using namespace Vulkan;

			VulkanDevice& InVulkanDevice = static_cast<VulkanDevice&>(InContext.GetDevice());
			//WindowsOutputDevice& InOutputDevice = static_cast<WindowsOutputDevice&>(InContext.GetOutputDevice());

			//vk::Win32SurfaceCreateInfoKHR Win32SurfaceInfo(
			//	vk::Win32SurfaceCreateFlagBitsKHR(),
			//	InOutputDevice.GetHInstance(),
			//	InOutputDevice.GetWindowHandler()
			//);
			//vk::AndroidSurfaceCreateInfoKHR AndroidSurfaceInfo(
			//	vk::AndroidSurfaceCreateFlagBitsKHR(),

			//);

			vk::Instance& VulkanInstance = InVulkanDevice.GetInstance();
			//VerifySuccess(VulkanInstance.createWin32SurfaceKHR(&Win32SurfaceInfo, nullptr, &_Surface));
			//VerifySuccess(VulkanInstance.createAndroidSurfaceKHR());
		}
	}
}

#endif
