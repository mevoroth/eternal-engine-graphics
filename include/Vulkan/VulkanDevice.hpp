#ifndef _VULKAN_DEVICE_HPP_
#define _VULKAN_DEVICE_HPP_

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class Window;

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

			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessenger(
				VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
				const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
				void*                                            pUserData
			);

			VulkanDevice(_In_ Window& WindowObj);

			uint32_t GetQueueFamilyIndex()
			{
				return 0;
			}

			vk::Device& GetVulkanDevice();
			vk::PhysicalDevice& GetPhysicalDevice() { return _PhysicalDevice; }
			vk::Instance& GetInstance() { return _Instance; }
			uint32_t GetQueueFamilyPropertiesCount() const { return _QueueFamilyPropertiesCount; }
			const vk::PhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties() const { return _PhysicalDeviceMemoryProperties; }
			uint32_t FindBestMemoryTypeIndex(_In_ uint32_t MemoryTypeBitsRequirement, _In_ const vk::MemoryPropertyFlagBits& Flags) const;
			uint32_t GetVulkanVersion() const { return _VulkanVersion; }
			//vk::Queue PopVulkanQueue();

			virtual uint32_t GetDeviceMask() const override { return 0x1; }
			virtual DeviceType GetDeviceType() const override { return DeviceType::VULKAN; }

		private:
			vector<vk::Queue>						_VulkanQueues;
			vk::PhysicalDeviceMemoryProperties	_PhysicalDeviceMemoryProperties;
			vk::DebugReportCallbackEXT			_DebugReportCallback;
			vk::DebugUtilsMessengerEXT			_DebugUtilsMessengerCallback;
			vk::Instance						_Instance;
			vk::PhysicalDevice					_PhysicalDevice;
			vk::Device							_Device;
			vk::PhysicalDeviceProperties		_PhysicalDeviceProperties;
			uint32_t							_QueueFamilyPropertiesCount = 0;
			uint32_t							_VulkanVersion = 0;
		};
	}
}

#endif
