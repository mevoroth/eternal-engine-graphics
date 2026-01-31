#if ETERNAL_PLATFORM_ANDROID && ETERNAL_ENABLE_VULKAN

#include "Android/Vulkan/AndroidVulkanSwapChain.hpp"
#include "Android/AndroidOutputDevice.hpp"
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
			AndroidOutputDevice& InOutputDevice = static_cast<AndroidOutputDevice&>(InContext.GetOutputDevice());

			vk::AndroidSurfaceCreateInfoKHR AndroidSurfaceInfo(
				vk::AndroidSurfaceCreateFlagBitsKHR(),
				InOutputDevice.GetAndroidNativeWindow()
			);

			vk::Instance& VulkanInstance = InVulkanDevice.GetInstance();
			VerifySuccess(VulkanInstance.createAndroidSurfaceKHR(&AndroidSurfaceInfo, nullptr, &_Surface));
		}
	}
}

#endif
