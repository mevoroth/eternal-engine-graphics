#ifndef _VULKAN_DEVICE_HPP_
#define _VULKAN_DEVICE_HPP_

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

namespace Eternal
{
	namespace Graphics
	{
		class Window;
		class VulkanCommandQueue;
		class VulkanRenderTarget;

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

			VulkanDevice(_In_ Window& WindowObj);

			inline uint32_t GetBackBufferFrameCount() const
			{
				return _BackBufferFrameCount;
			}

			uint32_t GetQueueFamilyIndex()
			{
				return 0;
			}

			VulkanRenderTarget*const & GetBackBuffer(_In_ uint32_t BackBufferIndex);
			VulkanCommandQueue*&	GetCommandQueue() { return _CommandQueue; }
			VkDevice& GetDevice();
			VkPhysicalDevice& GetPhysicalDevice() { return _PhysicalDevice; }
			VkInstance& GetInstance() { return _Instance; }
			uint32_t GetQueueFamilyPropertiesCount() const { return _QueueFamilyPropertiesCount; }

		private:
			void _CreateDirectCommandQueue();

			VulkanRenderTarget* _BackBuffer = nullptr;
			VulkanCommandQueue* _CommandQueue = nullptr;

			VkDebugReportCallbackEXT _DebugReportCallback = nullptr;
			VkInstance _Instance = nullptr;
			VkPhysicalDevice _PhysicalDevice = nullptr;
			VkDevice _Device = nullptr;
			uint32_t _QueueFamilyPropertiesCount = 0;
			uint32_t _BackBufferFrameCount = 2;
		};
	}
}

#endif