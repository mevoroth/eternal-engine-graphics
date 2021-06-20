#include "Vulkan_deprecated/VulkanDevice.hpp"

#include <algorithm>
#include "Window/Window.hpp"
#include "Vulkan/VulkanUtils.hpp"

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
	//ETERNAL_BREAK();
	OutputDebugString(pMessage);
	OutputDebugString("\n");
	return VK_FALSE;
}

VkBool32 VulkanDevice::DebugUtilsMessenger(
	VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
	const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
	void*                                            pUserData
)
{
	OutputDebugString(pCallbackData->pMessage);
	OutputDebugString("\n");
	return VK_FALSE;
}

namespace Eternal
{
	namespace Graphics
	{
		namespace VulkanPrivate
		{
			class EternalDispatchLoader
			{
			public:
				EternalDispatchLoader(vk::Instance& InInstance)
					: _Instance(InInstance)
				{
					_vkCreateDebugReport			= (PFN_vkCreateDebugReportCallbackEXT)	_Instance.getProcAddr("vkCreateDebugReportCallbackEXT");
					_vkDestroyDebugReport			= (PFN_vkDestroyDebugReportCallbackEXT)	_Instance.getProcAddr("vkDestroyDebugReportCallbackEXT");
					_vkDebugReportMessage			= (PFN_vkDebugReportMessageEXT)			_Instance.getProcAddr("vkDebugReportMessageEXT");
					_vkCreateDebugUtilsMessenger	= (PFN_vkCreateDebugUtilsMessengerEXT)	_Instance.getProcAddr("vkCreateDebugUtilsMessengerEXT");

					ETERNAL_ASSERT(_vkCreateDebugReport);
					ETERNAL_ASSERT(_vkDestroyDebugReport);
					ETERNAL_ASSERT(_vkDebugReportMessage);
					ETERNAL_ASSERT(_vkCreateDebugUtilsMessenger);
				}

				VkResult vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) const VULKAN_HPP_NOEXCEPT
				{
					return _vkCreateDebugReport(instance, pCreateInfo, pAllocator, pCallback);
				}

				void vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
				{
					_vkDestroyDebugReport(instance, callback, pAllocator);
				}

				void vkDebugReportMessageEXT(VkInstance instance, VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage)
				{
					_vkDebugReportMessage(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
				}

				VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) const VULKAN_HPP_NOEXCEPT
				{
					return _vkCreateDebugUtilsMessenger(instance, pCreateInfo, pAllocator, pMessenger);
				}

			private:
				vk::Instance& _Instance;
				PFN_vkCreateDebugReportCallbackEXT _vkCreateDebugReport = nullptr;
				PFN_vkDestroyDebugReportCallbackEXT _vkDestroyDebugReport = nullptr;
				PFN_vkDebugReportMessageEXT _vkDebugReportMessage = nullptr;
				PFN_vkCreateDebugUtilsMessengerEXT _vkCreateDebugUtilsMessenger = nullptr;
			};
		}
	}
}

VulkanDevice::VulkanDevice(_In_ Window& WindowObj)
{
	using namespace Vulkan;

	memset(&_PhysicalDeviceMemoryProperties, 0x0, sizeof(VkPhysicalDeviceMemoryProperties));

	// SHOULD CHECK

	// Validation layer
	uint32_t PropertyCount;
	VerifySuccess(vk::enumerateInstanceLayerProperties(&PropertyCount, static_cast<vk::LayerProperties*>(nullptr)));

	if (!PropertyCount) // No validation layer
		return;

	std::vector<vk::LayerProperties> Layers;
	Layers.resize(PropertyCount);

	VerifySuccess(vk::enumerateInstanceLayerProperties(&PropertyCount, Layers.data()));

	const char* VulkanValidationLayers[] =
	{
		//"VK_LAYER_KHRONOS_validation"
		//"VK_LAYER_LUNARG_standard_validation",
		//"VK_LAYER_GOOGLE_threading",
		//"VK_LAYER_LUNARG_parameter_validation",
		//"VK_LAYER_LUNARG_object_tracker",
		//"VK_LAYER_LUNARG_image",
		//"VK_LAYER_LUNARG_core_validation",
		//"VK_LAYER_LUNARG_swapchain",
		//"VK_LAYER_GOOGLE_unique_objects",

		//"VK_LAYER_LUNARG_api_dump",
		//"VK_LAYER_LUNARG_device_simulation",
		"VK_LAYER_KHRONOS_validation",
		//"VK_LAYER_LUNARG_monitor",
		//"VK_LAYER_LUNARG_screenshot",
		//"VK_LAYER_LUNARG_standard_validation", //deprecated?
		//"VK_LAYER_LUNARG_vktrace"
	};

	const char* VulkanInstanceExtensions[] =
	{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	uint32_t InstanceExtensionPropertiesCount;
	vk::enumerateInstanceExtensionProperties(nullptr, &InstanceExtensionPropertiesCount, static_cast<vk::ExtensionProperties *>(nullptr));

	std::vector<vk::ExtensionProperties> InstanceExtensionProperties;
	InstanceExtensionProperties.resize(InstanceExtensionPropertiesCount);

	vk::enumerateInstanceExtensionProperties(nullptr, &InstanceExtensionPropertiesCount, InstanceExtensionProperties.data());

	_VulkanVersion = VK_API_VERSION_1_2; // Vulkan version
	vk::ApplicationInfo ApplicationInfo(
		WindowObj.GetClassName().c_str(),
		0,
		"EternalEngine",
		0,
		_VulkanVersion
	);

	vk::InstanceCreateInfo InstanceInfo(
		vk::InstanceCreateFlags(),
		&ApplicationInfo,
		ETERNAL_ARRAYSIZE(VulkanValidationLayers),
		VulkanValidationLayers,
		ETERNAL_ARRAYSIZE(VulkanInstanceExtensions),
		VulkanInstanceExtensions
	);

	VerifySuccess(vk::createInstance(&InstanceInfo, nullptr, &_Instance));

	uint32_t PhysicalDevicesCount;
	VerifySuccess(_Instance.enumeratePhysicalDevices(&PhysicalDevicesCount, static_cast<vk::PhysicalDevice*>(nullptr)));

	ETERNAL_ASSERT(PhysicalDevicesCount > 0);
	//ETERNAL_ASSERT(PhysicalDevicesCount == 1); // 1gpu

	vector<vk::PhysicalDevice> PhysicalDevices;
	PhysicalDevices.resize(PhysicalDevicesCount);

	VerifySuccess(_Instance.enumeratePhysicalDevices(&PhysicalDevicesCount, PhysicalDevices.data()));

	vector<vk::PhysicalDeviceProperties> PhysicalDeviceProperties;
	PhysicalDeviceProperties.resize(PhysicalDevicesCount);

	for (uint32_t PhysicalDeviceIndex = 0; PhysicalDeviceIndex < PhysicalDevicesCount; ++PhysicalDeviceIndex)
	{
		PhysicalDevices[PhysicalDeviceIndex].getProperties(&PhysicalDeviceProperties[PhysicalDeviceIndex]);
	}

	bool FoundIntegratedGPU = false;
	vk::PhysicalDevice IntegratedPhysicalDevice;
	vk::PhysicalDeviceProperties IntegratedPhysicalDeviceProperties;

	for (uint32_t PhysicalDeviceIndex = 0; PhysicalDeviceIndex < PhysicalDevicesCount; ++PhysicalDeviceIndex)
	{
		if (PhysicalDeviceProperties[PhysicalDeviceIndex].deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
		{
			_PhysicalDevice = PhysicalDevices[PhysicalDeviceIndex];
			_PhysicalDeviceProperties = PhysicalDeviceProperties[PhysicalDeviceIndex];
			break;
		}
		else if (PhysicalDeviceProperties[PhysicalDeviceIndex].deviceType == vk::PhysicalDeviceType::eIntegratedGpu)
		{
			if (!FoundIntegratedGPU)
			{
				FoundIntegratedGPU = true;
				IntegratedPhysicalDevice = PhysicalDevices[PhysicalDeviceIndex];
				IntegratedPhysicalDeviceProperties = PhysicalDeviceProperties[PhysicalDeviceIndex];
			}
		}
	}

	if (_PhysicalDeviceProperties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu && FoundIntegratedGPU)
	{
		_PhysicalDevice = IntegratedPhysicalDevice;
		_PhysicalDeviceProperties = IntegratedPhysicalDeviceProperties;
	}
	else
	{
		ETERNAL_ASSERT(_PhysicalDeviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu);
	}

	uint32_t ExtensionsCount;
	VerifySuccess(_PhysicalDevice.enumerateDeviceExtensionProperties(nullptr, &ExtensionsCount, static_cast<vk::ExtensionProperties*>(nullptr)));
	
	std::vector<vk::ExtensionProperties> ExtensionProperties;
	ExtensionProperties.resize(ExtensionsCount);
	VerifySuccess(_PhysicalDevice.enumerateDeviceExtensionProperties(nullptr, &ExtensionsCount, ExtensionProperties.data()));

	PFN_vkDebugReportCallbackEXT vkDebugReport = VulkanDevice::DebugReport;

	vk::DebugReportCallbackCreateInfoEXT DebugReportCallbackInfo(
		vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eInformation | vk::DebugReportFlagBitsEXT::eDebug, // | VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT,
		vkDebugReport,
		nullptr
	);

	VulkanPrivate::EternalDispatchLoader EternalLoader(_Instance);

	VerifySuccess(_Instance.createDebugReportCallbackEXT(&DebugReportCallbackInfo, nullptr, &_DebugReportCallback, EternalLoader));
	
	vk::DebugUtilsMessengerCreateInfoEXT DebugUtilsCallbackInfo(
		vk::DebugUtilsMessengerCreateFlagBitsEXT(),
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,// | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,// | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral,
		VulkanDevice::DebugUtilsMessenger
	);
	VerifySuccess(_Instance.createDebugUtilsMessengerEXT(&DebugUtilsCallbackInfo, nullptr, &_DebugUtilsMessengerCallback, EternalLoader));

	_PhysicalDevice.getQueueFamilyProperties(&_QueueFamilyPropertiesCount, static_cast<vk::QueueFamilyProperties*>(nullptr));
	ETERNAL_ASSERT(_QueueFamilyPropertiesCount > 0);

	std::vector<vk::QueueFamilyProperties> QueueFamilyProperties = _PhysicalDevice.getQueueFamilyProperties();

	ETERNAL_ASSERT(QueueFamilyProperties[GetQueueFamilyIndex()].queueFlags & vk::QueueFlagBits::eGraphics); // Assume main device has graphics queue

	vk::PhysicalDeviceFeatures PhysicalDeviceFeatures;
	_PhysicalDevice.getFeatures(&PhysicalDeviceFeatures);
	
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

	vk::DeviceQueueCreateInfo DeviceQueueInfo(
		vk::DeviceQueueCreateFlagBits(),
		0,
		VulkanQueueCount,
		QueuePriorities.data()
	);

	vk::DeviceCreateInfo DeviceInfo(
		vk::DeviceCreateFlagBits(),
		1, &DeviceQueueInfo,
		ETERNAL_ARRAYSIZE(VulkanValidationLayers), VulkanValidationLayers,
		ETERNAL_ARRAYSIZE(VulkanDeviceExtensions), VulkanDeviceExtensions,
		static_cast<vk::PhysicalDeviceFeatures*>(nullptr)
	);

	VerifySuccess(_PhysicalDevice.createDevice(&DeviceInfo, nullptr, &_Device));

	_PhysicalDevice.getMemoryProperties(&_PhysicalDeviceMemoryProperties);

	_VulkanQueues.resize(VulkanQueueCount);
	for (uint32_t VulkanQueueIndex = 0; VulkanQueueIndex < VulkanQueueCount; ++VulkanQueueIndex)
	{
		_Device.getQueue(GetQueueFamilyIndex(), VulkanQueueIndex, &_VulkanQueues[VulkanQueueCount - VulkanQueueIndex - 1]);
	}
}

vk::Device& VulkanDevice::GetVulkanDevice()
{
	return _Device;
}

uint32_t VulkanDevice::FindBestMemoryTypeIndex(_In_ uint32_t MemoryTypeBitsRequirement, _In_ const vk::MemoryPropertyFlagBits& Flags) const
{
	uint32_t MemoryTypeIndex = 0;
	uint32_t MemoryTypeCount = _PhysicalDeviceMemoryProperties.memoryTypeCount;
	for (; MemoryTypeIndex < MemoryTypeCount; ++MemoryTypeIndex)
	{
		const bool IsRequiredMemoryType		= (MemoryTypeBitsRequirement & (1 << MemoryTypeIndex)) != 0;
		const bool HasRequiredProperties	= (_PhysicalDeviceMemoryProperties.memoryTypes[MemoryTypeIndex].propertyFlags & Flags) == Flags;

		if (IsRequiredMemoryType && HasRequiredProperties)
			break;
	}
	
	ETERNAL_ASSERT(MemoryTypeIndex < MemoryTypeCount);
	if (MemoryTypeIndex < MemoryTypeCount)
		return MemoryTypeIndex;

	return -1;
}
