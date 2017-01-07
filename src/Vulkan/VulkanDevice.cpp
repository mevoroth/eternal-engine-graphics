#include "Vulkan/VulkanDevice.hpp"

#include "Macros/Macros.hpp"

//#define VK_USE_PLATFORM_WIN32_KHR
//#include <vulkan/vulkan.h>
#include <vector>
#include "Window/Window.hpp"

using namespace Eternal::Graphics;

VkBool32 VulkanDevice::DebugReport(
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMessage,
	void*                                       pUserData
)
{
	ETERNAL_ASSERT(false);
	return VK_FALSE;
}

PFN_vkGetPhysicalDeviceSurfaceSupportKHR		vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	vkGetPhysicalDeviceSurfacePresentModesKHR;
PFN_vkGetSwapchainImagesKHR						vkGetSwapchainImagesKHR;

VulkanDevice::VulkanDevice()
{
	// 0 = OK
	VkResult Result;

	// SHOULD CHECK

	//// Validation layer
	//uint32_t PropertyCount;
	//Result = vkEnumerateInstanceLayerProperties(&PropertyCount, nullptr);
	//ETERNAL_ASSERT(!Result);

	//if (!PropertyCount) // No validation layer
	//	return;

	//std::vector<VkLayerProperties> Layers;
	//Layers.resize(PropertyCount);

	//Result = vkEnumerateInstanceLayerProperties(&PropertyCount, Layers.data());
	//ETERNAL_ASSERT(!Result);

	const char* VulkanValidationLayers[] = 
	{
		"VK_LAYER_LUNARG_standard_validation",
		//"VK_LAYER_GOOGLE_threading",
		//"VK_LAYER_LUNARG_parameter_validation",
		//"VK_LAYER_LUNARG_object_tracker",
		//"VK_LAYER_LUNARG_image",
		//"VK_LAYER_LUNARG_core_validation",
		//"VK_LAYER_LUNARG_swapchain",
		//"VK_LAYER_GOOGLE_unique_objects",
	};

	const char* VulkanExtensions[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	VkApplicationInfo ApplicationInfo;
	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pNext = nullptr;
	ApplicationInfo.pApplicationName = "Vulkan App";
	ApplicationInfo.applicationVersion = 0;
	ApplicationInfo.pEngineName = "EternalEngine";
	ApplicationInfo.engineVersion = 0;
	ApplicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo InstanceInfo;
	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pNext = nullptr;
	InstanceInfo.flags = 0;
	InstanceInfo.pApplicationInfo = &ApplicationInfo;
	InstanceInfo.ppEnabledLayerNames = VulkanValidationLayers;
	InstanceInfo.enabledLayerCount = ETERNAL_ARRAYSIZE(VulkanValidationLayers);
	InstanceInfo.ppEnabledExtensionNames = VulkanExtensions;
	InstanceInfo.enabledExtensionCount = ETERNAL_ARRAYSIZE(VulkanExtensions);

	Result = vkCreateInstance(&InstanceInfo, nullptr, &_Instance);
	ETERNAL_ASSERT(!Result);

	uint32_t PhysicalDevicesCount;
	Result = vkEnumeratePhysicalDevices(_Instance, &PhysicalDevicesCount, nullptr);
	ETERNAL_ASSERT(!Result);

	ETERNAL_ASSERT(PhysicalDevicesCount > 0);
	ETERNAL_ASSERT(PhysicalDevicesCount == 1); // 1gpu

	vkEnumeratePhysicalDevices(_Instance, &PhysicalDevicesCount, &_PhysicalDevice);
	//VK_KHR_SWAPCHAIN_EXTENSION_NAME

	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReport = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(_Instance, "vkCreateDebugReportCallbackEXT");
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReport = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(_Instance, "vkDestroyDebugReportCallbackEXT");
	PFN_vkDebugReportMessageEXT vkDebugReportMessage = (PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(_Instance, "vkDebugReportMessageEXT");

	PFN_vkDebugReportCallbackEXT vkDebugReport = VulkanDevice::DebugReport;

	ETERNAL_ASSERT(vkCreateDebugReport);
	ETERNAL_ASSERT(vkDestroyDebugReport);
	ETERNAL_ASSERT(vkDebugReportMessage);

	VkDebugReportCallbackCreateInfoEXT DebugCreateInfo;
	DebugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	DebugCreateInfo.pNext = nullptr;
	DebugCreateInfo.pfnCallback = vkDebugReport;

	Result = vkCreateDebugReport(_Instance, &DebugCreateInfo, nullptr, &_DebugReportCallback);
	ETERNAL_ASSERT(!Result);

	VkPhysicalDeviceProperties PhysicalDeviceProperties;
	vkGetPhysicalDeviceProperties(_PhysicalDevice, &PhysicalDeviceProperties);

	vkGetPhysicalDeviceQueueFamilyProperties(_PhysicalDevice, &_QueueFamilyPropertiesCount, nullptr);
	ETERNAL_ASSERT(_QueueFamilyPropertiesCount > 0);

	std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
	QueueFamilyProperties.resize(_QueueFamilyPropertiesCount);

	vkGetPhysicalDeviceQueueFamilyProperties(_PhysicalDevice, &_QueueFamilyPropertiesCount, QueueFamilyProperties.data());

	VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(_PhysicalDevice, &PhysicalDeviceFeatures);
	
	vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)vkGetInstanceProcAddr(_Instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)vkGetInstanceProcAddr(_Instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)vkGetInstanceProcAddr(_Instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)vkGetInstanceProcAddr(_Instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)vkGetInstanceProcAddr(_Instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	ETERNAL_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR);
	ETERNAL_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	ETERNAL_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR);
	ETERNAL_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR);
	ETERNAL_ASSERT(vkGetSwapchainImagesKHR);
}

void VulkanDevice::CreateSwapChain(Window& WindowObj)
{
	VkResult Result;
	float QueuePriorities = 0.0f;

	VkDeviceQueueCreateInfo DeviceQueueInfo;
	DeviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	DeviceQueueInfo.pNext = nullptr;
	DeviceQueueInfo.queueFamilyIndex = 0; __debugbreak(); //fix this
	DeviceQueueInfo.queueCount = 0; __debugbreak(); //fix this
	DeviceQueueInfo.pQueuePriorities = &QueuePriorities;
	DeviceQueueInfo.flags = 0;

	VkDeviceCreateInfo DeviceInfo;
	DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceInfo.pNext = nullptr;
	DeviceInfo.flags = 0;

	Result = vkCreateDevice(_PhysicalDevice, &DeviceInfo, nullptr, &_Device);
	ETERNAL_ASSERT(!Result);

	VkWin32SurfaceCreateInfoKHR Win32SurfaceInfo;
	Win32SurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	Win32SurfaceInfo.pNext = nullptr;
	Win32SurfaceInfo.hinstance = WindowObj.GetHInstance();
	Win32SurfaceInfo.hwnd = WindowObj.GetWindowHandler();
	Win32SurfaceInfo.flags = 0;

	VkSurfaceKHR RenderTarget;
	Result = vkCreateWin32SurfaceKHR(_Instance, &Win32SurfaceInfo, nullptr, &RenderTarget);
	ETERNAL_ASSERT(!Result);

	//for (int )
	//vkGetPhysicalDeviceSurfaceSupportKHR(_PhysicalDevice, );
}
