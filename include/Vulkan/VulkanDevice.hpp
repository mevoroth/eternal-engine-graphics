#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "VUlkan/VulkanUtils.hpp"

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
			uint32_t GetQueueFamilyIndexGraphics() const { return _QueueFamilyIndexGraphics; }
			uint32_t GetQueueFamilyIndexCompute() const { return _QueueFamilyIndexCompute; }
			uint32_t GetQueueFamilyIndexCopy() const { return _QueueFamilyIndexCopy; }
			uint32_t GetQueueIndexGraphics() const { return _QueueIndexGraphics; }
			uint32_t GetQueueIndexCompute() const { return _QueueIndexCompute; }
			uint32_t GetQueueIndexCopy() const { return _QueueIndexCopy; }

			virtual uint32_t GetDeviceMask() const override { return 0x1; }
			virtual DeviceType GetDeviceType() const override { return DeviceType::VULKAN; }

		private:
			vk::PhysicalDeviceMemoryProperties	_PhysicalDeviceMemoryProperties;
			vk::DebugReportCallbackEXT			_DebugReportCallback;
			vk::DebugUtilsMessengerEXT			_DebugUtilsMessengerCallback;
			vk::Instance						_Instance;
			vk::PhysicalDevice					_PhysicalDevice;
			vk::Device							_Device;
			vk::PhysicalDeviceProperties		_PhysicalDeviceProperties;
			uint32_t							_QueueFamilyPropertiesCount = 0;
			uint32_t							_VulkanVersion = 0;
			uint32_t							_QueueFamilyIndexGraphics	= Vulkan::InvalidQueueFamilyIndex;
			uint32_t							_QueueFamilyIndexCompute	= Vulkan::InvalidQueueFamilyIndex;
			uint32_t							_QueueFamilyIndexCopy		= Vulkan::InvalidQueueFamilyIndex;
			uint32_t							_QueueIndexGraphics			= Vulkan::InvalidQueueIndex;
			uint32_t							_QueueIndexCompute			= Vulkan::InvalidQueueIndex;
			uint32_t							_QueueIndexCopy				= Vulkan::InvalidQueueIndex;
		};
	}
}
