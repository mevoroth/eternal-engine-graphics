#include "Vulkan/VulkanDevice.hpp"

#include "Macros/Macros.hpp"

#include <vector>
#include <algorithm>
#include "Window/Window.hpp"
#include "Vulkan/VulkanCommandQueue.hpp"

using namespace Eternal::Graphics;

VkBool32 VulkanDevice::DebugReport(
	VkDebugReportFlagsEXT						flags,
	VkDebugReportObjectTypeEXT					objectType,
	uint64_t									object,
	size_t										location,
	int32_t										messageCode,
	const char*									pLayerPrefix,
	const char*									pMessage,
	void*										pUserData
)
{
	//ETERNAL_ASSERT(false);
	OutputDebugString(pMessage);
	OutputDebugString("\n");
	return VK_FALSE;
}

//PFN_vkGetPhysicalDeviceSurfaceSupportKHR		vkGetPhysicalDeviceSurfaceSupportKHR;
//PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
//PFN_vkGetPhysicalDeviceSurfaceFormatsKHR		vkGetPhysicalDeviceSurfaceFormatsKHR;
//PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	vkGetPhysicalDeviceSurfacePresentModesKHR;
//PFN_vkGetSwapchainImagesKHR						vkGetSwapchainImagesKHR;

VulkanDevice::VulkanDevice(_In_ Window& WindowObj)
{
	memset(&_PhysicalDeviceMemoryProperties, 0x0, sizeof(VkPhysicalDeviceMemoryProperties));

	// 0 = OK
	VkResult Result;

	// SHOULD CHECK

	// Validation layer
	uint32_t PropertyCount;
	Result = vkEnumerateInstanceLayerProperties(&PropertyCount, nullptr);
	ETERNAL_ASSERT(!Result);

	if (!PropertyCount) // No validation layer
		return;

	std::vector<VkLayerProperties> Layers;
	Layers.resize(PropertyCount);

	Result = vkEnumerateInstanceLayerProperties(&PropertyCount, Layers.data());
	ETERNAL_ASSERT(!Result);

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

	const char* VulkanInstanceExtensions[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	uint32_t InstanceExtensionPropertiesCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &InstanceExtensionPropertiesCount, nullptr);

	std::vector<VkExtensionProperties> InstanceExtensionProperties;
	InstanceExtensionProperties.resize(InstanceExtensionPropertiesCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &InstanceExtensionPropertiesCount, InstanceExtensionProperties.data());

	VkApplicationInfo ApplicationInfo;
	ApplicationInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pNext				= nullptr;
	ApplicationInfo.pApplicationName	= WindowObj.GetClassName().c_str();
	ApplicationInfo.applicationVersion	= 0;
	ApplicationInfo.pEngineName			= "EternalEngine";
	ApplicationInfo.engineVersion		= 0;
	ApplicationInfo.apiVersion			= VK_API_VERSION_1_0;

	VkInstanceCreateInfo InstanceInfo;
	InstanceInfo.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pNext						= nullptr;
	InstanceInfo.flags						= 0;
	InstanceInfo.pApplicationInfo			= &ApplicationInfo;
	InstanceInfo.ppEnabledLayerNames		= VulkanValidationLayers;
	InstanceInfo.enabledLayerCount			= ETERNAL_ARRAYSIZE(VulkanValidationLayers);
	InstanceInfo.ppEnabledExtensionNames	= VulkanInstanceExtensions;
	InstanceInfo.enabledExtensionCount		= ETERNAL_ARRAYSIZE(VulkanInstanceExtensions);

	Result = vkCreateInstance(&InstanceInfo, nullptr, &_Instance);
	ETERNAL_ASSERT(!Result);

	uint32_t PhysicalDevicesCount;
	Result = vkEnumeratePhysicalDevices(_Instance, &PhysicalDevicesCount, nullptr);
	ETERNAL_ASSERT(!Result);

	ETERNAL_ASSERT(PhysicalDevicesCount > 0);
	ETERNAL_ASSERT(PhysicalDevicesCount == 1); // 1gpu

	vkEnumeratePhysicalDevices(_Instance, &PhysicalDevicesCount, &_PhysicalDevice);

	uint32_t ExtensionsCount;
	Result = vkEnumerateDeviceExtensionProperties(_PhysicalDevice, nullptr, &ExtensionsCount, nullptr);
	ETERNAL_ASSERT(!Result);
	
	std::vector<VkExtensionProperties> ExtensionProperties;
	ExtensionProperties.resize(ExtensionsCount);
	Result = vkEnumerateDeviceExtensionProperties(_PhysicalDevice, nullptr, &ExtensionsCount, ExtensionProperties.data());
	ETERNAL_ASSERT(!Result);

	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReport		= (PFN_vkCreateDebugReportCallbackEXT)	vkGetInstanceProcAddr(_Instance, "vkCreateDebugReportCallbackEXT");
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReport	= (PFN_vkDestroyDebugReportCallbackEXT)	vkGetInstanceProcAddr(_Instance, "vkDestroyDebugReportCallbackEXT");
	PFN_vkDebugReportMessageEXT vkDebugReportMessage			= (PFN_vkDebugReportMessageEXT)			vkGetInstanceProcAddr(_Instance, "vkDebugReportMessageEXT");

	PFN_vkDebugReportCallbackEXT vkDebugReport = VulkanDevice::DebugReport;

	ETERNAL_ASSERT(vkCreateDebugReport);
	ETERNAL_ASSERT(vkDestroyDebugReport);
	ETERNAL_ASSERT(vkDebugReportMessage);

	VkDebugReportCallbackCreateInfoEXT DebugReportCallbackInfo;
	DebugReportCallbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	DebugReportCallbackInfo.pNext = nullptr;
	DebugReportCallbackInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;// | VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
	DebugReportCallbackInfo.pfnCallback = vkDebugReport;

	Result = vkCreateDebugReport(_Instance, &DebugReportCallbackInfo, nullptr, &_DebugReportCallback);
	ETERNAL_ASSERT(!Result);

	VkPhysicalDeviceProperties PhysicalDeviceProperties;
	vkGetPhysicalDeviceProperties(_PhysicalDevice, &PhysicalDeviceProperties);

	vkGetPhysicalDeviceQueueFamilyProperties(_PhysicalDevice, &_QueueFamilyPropertiesCount, nullptr);
	ETERNAL_ASSERT(_QueueFamilyPropertiesCount > 0);

	std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
	QueueFamilyProperties.resize(_QueueFamilyPropertiesCount);

	vkGetPhysicalDeviceQueueFamilyProperties(_PhysicalDevice, &_QueueFamilyPropertiesCount, QueueFamilyProperties.data());

	ETERNAL_ASSERT(QueueFamilyProperties[GetQueueFamilyIndex()].queueFlags & VK_QUEUE_GRAPHICS_BIT); // Assume main device has graphics queue

	VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(_PhysicalDevice, &PhysicalDeviceFeatures);
	
	uint32_t VulkanQueueCount = QueueFamilyProperties[GetQueueFamilyIndex()].queueCount;

	vector<float> QueuePriorities;
	QueuePriorities.resize(VulkanQueueCount);
	for (uint32_t QueuePriorityIndex = 0; QueuePriorityIndex < VulkanQueueCount; ++QueuePriorityIndex)
	{
		QueuePriorities[QueuePriorityIndex] = 0.0f;
	}

	const char* VulkanDeviceExtensions[] =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceQueueCreateInfo DeviceQueueInfo;
	DeviceQueueInfo.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	DeviceQueueInfo.pNext				= nullptr;
	DeviceQueueInfo.flags				= 0;
	DeviceQueueInfo.queueFamilyIndex	= 0; //__debugbreak(); fix this
	DeviceQueueInfo.queueCount			= VulkanQueueCount; //__debugbreak(); fix this
	DeviceQueueInfo.pQueuePriorities	= QueuePriorities.data();

	VkDeviceCreateInfo DeviceInfo;
	DeviceInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceInfo.pNext					= nullptr;// &DebugReportCallbackInfo;
	DeviceInfo.flags					= 0;
	DeviceInfo.pQueueCreateInfos		= &DeviceQueueInfo;
	DeviceInfo.queueCreateInfoCount		= 1;
	DeviceInfo.ppEnabledExtensionNames	= VulkanDeviceExtensions;
	DeviceInfo.enabledExtensionCount	= ETERNAL_ARRAYSIZE(VulkanDeviceExtensions);
	DeviceInfo.ppEnabledLayerNames		= VulkanValidationLayers;
	DeviceInfo.enabledLayerCount		= ETERNAL_ARRAYSIZE(VulkanValidationLayers);
	DeviceInfo.pEnabledFeatures			= nullptr;

	Result = vkCreateDevice(_PhysicalDevice, &DeviceInfo, nullptr, &_Device);
	ETERNAL_ASSERT(!Result);

	vkGetPhysicalDeviceMemoryProperties(_PhysicalDevice, &_PhysicalDeviceMemoryProperties);

	_VulkanQueues.resize(VulkanQueueCount);
	for (uint32_t VulkanQueueIndex = 0; VulkanQueueIndex < VulkanQueueCount; ++VulkanQueueIndex)
	{
		vkGetDeviceQueue(_Device, GetQueueFamilyIndex(), VulkanQueueIndex, &_VulkanQueues[VulkanQueueCount - VulkanQueueIndex - 1]);
	}
}

VkDevice& VulkanDevice::GetVulkanDevice()
{
	return _Device;
}

uint32_t VulkanDevice::FindBestMemoryTypeIndex(_In_ const VkMemoryPropertyFlagBits& Flags) const
{
	uint32_t MemoryTypeIndex = 0;
	uint32_t MemoryTypeCount = _PhysicalDeviceMemoryProperties.memoryTypeCount;
	for (; MemoryTypeIndex < MemoryTypeCount; ++MemoryTypeIndex)
	{
		if (_PhysicalDeviceMemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags == Flags)
			break;
	}
	ETERNAL_ASSERT(MemoryTypeIndex < MemoryTypeCount);
	return MemoryTypeIndex;
}

VkQueue VulkanDevice::PopVulkanQueue()
{
	VkQueue CurrentQueue = _VulkanQueues.back();
	_VulkanQueues.pop_back();
	return CurrentQueue;
}
