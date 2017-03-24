#ifndef _VULKAN_DEVICE_HPP_
#define _VULKAN_DEVICE_HPP_

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include "NextGenGraphics/Device.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Window;
		class Vulkan;

		class VulkanDevice : public Device
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

			VulkanDevice(_In_ Window& WindowObj);

			uint32_t GetQueueFamilyIndex()
			{
				return 0;
			}

			VkDevice& GetVulkanDevice();
			VkPhysicalDevice& GetPhysicalDevice() { return _PhysicalDevice; }
			VkInstance& GetInstance() { return _Instance; }
			uint32_t GetQueueFamilyPropertiesCount() const { return _QueueFamilyPropertiesCount; }
			const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties() const { return _PhysicalDeviceMemoryProperties; }
			uint32_t FindBestMemoryTypeIndex(_In_ const VkMemoryPropertyFlagBits& Flags) const;

			virtual uint32_t GetDeviceMask() const override { return 0x1; }
			virtual DeviceType GetDeviceType() const override { return VULKAN; }

		private:
			VkPhysicalDeviceMemoryProperties	_PhysicalDeviceMemoryProperties;
			VkDebugReportCallbackEXT			_DebugReportCallback			= nullptr;
			VkInstance							_Instance						= nullptr;
			VkPhysicalDevice					_PhysicalDevice					= nullptr;
			VkDevice							_Device							= nullptr;
			uint32_t							_QueueFamilyPropertiesCount		= 0;
		};
	}
}

#endif
