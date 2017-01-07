#ifndef _VULKAN_DEVICE_HPP_
#define _VULKAN_DEVICE_HPP_

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

namespace Eternal
{
	namespace Graphics
	{
		class Window;

		class VulkanDevice
		{
		public:
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport(
				VkDebugReportFlagsEXT                       flags,
				VkDebugReportObjectTypeEXT                  objectType,
				uint64_t                                    object,
				size_t                                      location,
				int32_t                                     messageCode,
				const char*                                 pLayerPrefix,
				const char*                                 pMessage,
				void*                                       pUserData
			);

			VulkanDevice();

			void CreateSwapChain(Window& WindowObj);

		private:
			VkDebugReportCallbackEXT _DebugReportCallback;
			VkInstance _Instance;
			VkPhysicalDevice _PhysicalDevice;
			VkDevice _Device;
			uint32_t _QueueFamilyPropertiesCount;
		};
	}
}

#endif
