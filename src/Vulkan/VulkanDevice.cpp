#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanDevice.hpp"

#include "Vulkan/VulkanUtils.hpp"
#include "OutputDevice/OutputDevice.hpp"
#include "Log/Log.hpp"
#include <algorithm>

namespace Eternal
{
	namespace Graphics
	{
		VkBool32 VulkanDevice::DebugReport(
			_In_ VkDebugReportFlagsEXT							InFlags,
			_In_ VkDebugReportObjectTypeEXT						InObjectType,
			_In_ uint64_t										InObject,
			_In_ size_t											InLocation,
			_In_ int32_t										InMessageCode,
			_In_ const char*									InLayerPrefix,
			_In_ const char*									InMessage,
			_In_ void*											InUserData
		)
		{
			const LogSystem::Log::LogLevel& Level = [InFlags]() -> const LogSystem::Log::LogLevel&
			{
				switch (InFlags)
				{
				case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
					return LogInfo;
				
				case VK_DEBUG_REPORT_WARNING_BIT_EXT:
				case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
					return LogWarning;

				case VK_DEBUG_REPORT_ERROR_BIT_EXT:
				case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
					return LogError;
				default:
					break;
				}
				return LogCritical;
			}();

			LogWrite(Level, LogGraphics, InMessage);
			//ETERNAL_BREAK();

			return VK_FALSE;
		}

		VkBool32 VulkanDevice::DebugUtilsMessenger(
			_In_ VkDebugUtilsMessageSeverityFlagBitsEXT			InMessageSeverity,
			_In_ VkDebugUtilsMessageTypeFlagsEXT				InMessageTypes,
			_In_ const VkDebugUtilsMessengerCallbackDataEXT*	InCallbackData,
			_In_ void*											InUserData
		)
		{
			const LogSystem::Log::LogLevel& Level = [InMessageSeverity]() -> const LogSystem::Log::LogLevel&
			{
				switch (InMessageSeverity)
				{
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
					return LogInfo;
				
				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
					return LogWarning;

				case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
					return LogError;
				default:
					break;
				}
				return LogCritical;
			}();

			LogWrite(Level, LogGraphics, InCallbackData->pMessage);

			ETERNAL_BREAK();
			return VK_FALSE;
		}

		EternalDebugDispatchLoader::EternalDebugDispatchLoader(vk::Instance& InInstance)
			: _Instance(InInstance)
		{
			_vkSetDebugUtilsObjectName	= (PFN_vkSetDebugUtilsObjectNameEXT)_Instance.getProcAddr("vkSetDebugUtilsObjectNameEXT");
			_vkCmdDebugMarkerBegin		= (PFN_vkCmdDebugMarkerBeginEXT)	_Instance.getProcAddr("vkCmdDebugMarkerBeginEXT");
			_vkCmdDebugMarkerEnd		= (PFN_vkCmdDebugMarkerEndEXT)		_Instance.getProcAddr("vkCmdDebugMarkerEndEXT");
			
			ETERNAL_ASSERT(_vkSetDebugUtilsObjectName);
			ETERNAL_ASSERT(_vkCmdDebugMarkerBegin);
			ETERNAL_ASSERT(_vkCmdDebugMarkerEnd);
		}

		size_t EternalDebugDispatchLoader::getVkHeaderVersion() const
		{
			return VK_HEADER_VERSION;
		}

		VkResult EternalDebugDispatchLoader::vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) const
		{
			return _vkSetDebugUtilsObjectName(device, pNameInfo);
		}

		void EternalDebugDispatchLoader::vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const
		{
			_vkCmdDebugMarkerBegin(commandBuffer, pMarkerInfo);
		}

		void EternalDebugDispatchLoader::vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) const
		{
			_vkCmdDebugMarkerEnd(commandBuffer);
		}

		namespace VulkanPrivate
		{
			class EternalDeviceDispatchLoader
			{
			public:
				EternalDeviceDispatchLoader(vk::Instance& InInstance)
					: _Instance(InInstance)
				{
					_vkCreateDebugReport			= (PFN_vkCreateDebugReportCallbackEXT)	_Instance.getProcAddr("vkCreateDebugReportCallbackEXT");
					_vkDestroyDebugReport			= (PFN_vkDestroyDebugReportCallbackEXT)	_Instance.getProcAddr("vkDestroyDebugReportCallbackEXT");
					_vkDebugReportMessage			= (PFN_vkDebugReportMessageEXT)			_Instance.getProcAddr("vkDebugReportMessageEXT");
					_vkCreateDebugUtilsMessenger	= (PFN_vkCreateDebugUtilsMessengerEXT)	_Instance.getProcAddr("vkCreateDebugUtilsMessengerEXT");

					ETERNAL_ASSERT(_vkCreateDebugReport);
					ETERNAL_ASSERT(_vkDestroyDebugReport);
					ETERNAL_ASSERT(_vkDebugReportMessage);
					//ETERNAL_ASSERT(_vkCreateDebugUtilsMessenger);
				}

				size_t getVkHeaderVersion() const
				{
					return VK_HEADER_VERSION;
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
					if (_vkCreateDebugUtilsMessenger)
						return _vkCreateDebugUtilsMessenger(instance, pCreateInfo, pAllocator, pMessenger);
					return VK_SUCCESS;
				}

			private:
				vk::Instance&						_Instance;
				PFN_vkCreateDebugReportCallbackEXT	_vkCreateDebugReport			= nullptr;
				PFN_vkDestroyDebugReportCallbackEXT	_vkDestroyDebugReport			= nullptr;
				PFN_vkDebugReportMessageEXT			_vkDebugReportMessage			= nullptr;
				PFN_vkCreateDebugUtilsMessengerEXT	_vkCreateDebugUtilsMessenger	= nullptr;
			};
		}

		VulkanDevice::VulkanDevice(_In_ OutputDevice& InOutputDevice)
		{
			using namespace Vulkan;
			using namespace Eternal::Graphics::Vulkan;

			memset(&_PhysicalDeviceMemoryProperties, 0x0, sizeof(VkPhysicalDeviceMemoryProperties));

			// SHOULD CHECK

			// Validation layer
			uint32_t PropertyCount;
			VerifySuccess(vk::enumerateInstanceLayerProperties(&PropertyCount, static_cast<vk::LayerProperties*>(nullptr)));

			std::vector<vk::LayerProperties> Layers;

			if (PropertyCount > 0)
			{
				Layers.resize(PropertyCount);
				VerifySuccess(vk::enumerateInstanceLayerProperties(&PropertyCount, Layers.data()));
			}

			vector<const char*> VulkanLayers =
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

			for (uint32_t VulkanLayerIndex = 0; VulkanLayerIndex < VulkanLayers.size(); ++VulkanLayerIndex)
			{
				const char* CurrentVulkanLayer = VulkanLayers[VulkanLayerIndex];

				auto FoundLayer = std::find_if(
					Layers.begin(),
					Layers.end(),
					[CurrentVulkanLayer](_In_ const vk::LayerProperties& InLayerProperties)
					{
						return strcmp(InLayerProperties.layerName, CurrentVulkanLayer) == 0;
					}
				);

				if (FoundLayer == Layers.end())
				{
					VulkanLayers.erase(VulkanLayers.begin() + VulkanLayerIndex);
					--VulkanLayerIndex;
				}
			}
			vector<const char*> VulkanInstanceExtensions =
			{
				VK_KHR_SURFACE_EXTENSION_NAME,
				VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME
			};
			VulkanPrivate::GatherVulkanInstanceExtensions(VulkanInstanceExtensions);

			uint32_t InstanceExtensionPropertiesCount;
			VerifySuccess(vk::enumerateInstanceExtensionProperties(nullptr, &InstanceExtensionPropertiesCount, static_cast<vk::ExtensionProperties*>(nullptr)));

			std::vector<vk::ExtensionProperties> InstanceExtensionProperties;
			InstanceExtensionProperties.resize(InstanceExtensionPropertiesCount);

			VerifySuccess(vk::enumerateInstanceExtensionProperties(nullptr, &InstanceExtensionPropertiesCount, InstanceExtensionProperties.data()));

			for (uint32_t VulkanInstanceExtensionIndex = 0; VulkanInstanceExtensionIndex < VulkanInstanceExtensions.size(); ++VulkanInstanceExtensionIndex)
			{
				const char* CurrentVulkanInstanceExtension = VulkanInstanceExtensions[VulkanInstanceExtensionIndex];

				auto FoundExtension = std::find_if(
					InstanceExtensionProperties.begin(),
					InstanceExtensionProperties.end(),
					[CurrentVulkanInstanceExtension](_In_ const vk::ExtensionProperties& InExtensionProperties)
					{
						return strcmp(InExtensionProperties.extensionName, CurrentVulkanInstanceExtension) == 0;
					}
				);

				if (FoundExtension == InstanceExtensionProperties.end())
				{
					VulkanInstanceExtensions.erase(VulkanInstanceExtensions.begin() + VulkanInstanceExtensionIndex);
					--VulkanInstanceExtensionIndex;
				}
			}

			_VulkanVersion = VK_API_VERSION_1_2; // Vulkan version
			vk::ApplicationInfo ApplicationInfo(
				InOutputDevice.GetApplicationName(),
				0,
				"EternalEngine",
				0,
				_VulkanVersion
			);

			vk::InstanceCreateInfo InstanceInfo(
				vk::InstanceCreateFlags(),
				&ApplicationInfo,
				VulkanLayers.size(),
				VulkanLayers.data(),
				VulkanInstanceExtensions.size(),
				VulkanInstanceExtensions.data()
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

			auto FindBestDevice = [&PhysicalDevices, &PhysicalDeviceProperties, PhysicalDevicesCount](
				_In_ const vk::PhysicalDeviceType& InPhysicalDeviceType,
				_Out_ vk::PhysicalDevice& OutPhysicalDevice,
				_Out_ vk::PhysicalDeviceProperties& OutPhysicalDeviceProperties
			) -> bool
			{
				for (uint32_t PhysicalDeviceIndex = 0; PhysicalDeviceIndex < PhysicalDevicesCount; ++PhysicalDeviceIndex)
				{
					if (PhysicalDeviceProperties[PhysicalDeviceIndex].deviceType == InPhysicalDeviceType)
					{
						OutPhysicalDevice			= PhysicalDevices[PhysicalDeviceIndex];
						OutPhysicalDeviceProperties	= PhysicalDeviceProperties[PhysicalDeviceIndex];
						return true;
					}
				}
				return false;
			};

			vk::PhysicalDeviceType DeviceTypePriority[] =
			{
				vk::PhysicalDeviceType::eDiscreteGpu,
				vk::PhysicalDeviceType::eIntegratedGpu,
				vk::PhysicalDeviceType::eVirtualGpu,
				vk::PhysicalDeviceType::eCpu,
				vk::PhysicalDeviceType::eOther
			};

			bool PhysicalDeviceIsFound = false;
			for (uint32_t DeviceTypeIndex = 0; !PhysicalDeviceIsFound && DeviceTypeIndex < ETERNAL_ARRAYSIZE(DeviceTypePriority); ++DeviceTypeIndex)
			{
				const vk::PhysicalDeviceType& CurrentDeviceType = DeviceTypePriority[DeviceTypeIndex];

				vk::PhysicalDevice FoundPhysicalDevice;
				vk::PhysicalDeviceProperties FoundPhysicalDeviceProperties;
				if (FindBestDevice(CurrentDeviceType, FoundPhysicalDevice, FoundPhysicalDeviceProperties))
				{
					_PhysicalDevice				= FoundPhysicalDevice;
					_PhysicalDeviceProperties	= FoundPhysicalDeviceProperties;
					PhysicalDeviceIsFound		= true;
				}
			}

			ETERNAL_ASSERT(PhysicalDeviceIsFound); // Device not found

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

			VulkanPrivate::EternalDeviceDispatchLoader EternalLoader(_Instance);
			_DebugDispatchLoader = new EternalDebugDispatchLoader(_Instance);

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

			std::vector<uint32_t> QueueIndices;
			QueueIndices.resize(QueueFamilyProperties.size());
			for (int32_t QueueIndex = 0; QueueIndex < QueueIndices.size(); ++QueueIndex)
			{
				QueueIndices[QueueIndex] = 0;
			}

			for (uint32_t QueueFamilyPropertyIndex = 0; QueueFamilyPropertyIndex < QueueFamilyProperties.size(); ++QueueFamilyPropertyIndex)
			{
				if (_QueueFamilyIndexGraphics == InvalidQueueFamilyIndex
					&& (QueueFamilyProperties[QueueFamilyPropertyIndex].queueFlags & vk::QueueFlagBits::eGraphics)
					&& QueueFamilyProperties[QueueFamilyPropertyIndex].queueCount > 0)
				{
					_QueueFamilyIndexGraphics = QueueFamilyPropertyIndex;
					_QueueIndexGraphics = QueueIndices[QueueFamilyPropertyIndex]++;
					QueueFamilyProperties[QueueFamilyPropertyIndex].queueCount--;
				}

				if (_QueueFamilyIndexCompute == InvalidQueueFamilyIndex
					&& (QueueFamilyProperties[QueueFamilyPropertyIndex].queueFlags & vk::QueueFlagBits::eCompute)
					&& QueueFamilyProperties[QueueFamilyPropertyIndex].queueCount > 0)
				{
					_QueueFamilyIndexCompute = QueueFamilyPropertyIndex;
					_QueueIndexCompute = QueueIndices[QueueFamilyPropertyIndex]++;
					QueueFamilyProperties[QueueFamilyPropertyIndex].queueCount--;
				}

				if (_QueueFamilyIndexCopy == InvalidQueueFamilyIndex
					&& (QueueFamilyProperties[QueueFamilyPropertyIndex].queueFlags & vk::QueueFlagBits::eTransfer)
					&& QueueFamilyProperties[QueueFamilyPropertyIndex].queueCount > 0)
				{
					_QueueFamilyIndexCopy = QueueFamilyPropertyIndex;
					_QueueIndexCopy = QueueIndices[QueueFamilyPropertyIndex]++;
					QueueFamilyProperties[QueueFamilyPropertyIndex].queueCount--;
				}
			}

			ETERNAL_ASSERT(_QueueFamilyIndexGraphics != InvalidQueueFamilyIndex);
			ETERNAL_ASSERT(_QueueFamilyIndexCompute != InvalidQueueFamilyIndex);
			ETERNAL_ASSERT(_QueueFamilyIndexCopy != InvalidQueueFamilyIndex);

			vk::PhysicalDeviceFeatures PhysicalDeviceFeatures;
			_PhysicalDevice.getFeatures(&PhysicalDeviceFeatures);
			
			vk::FormatProperties FormatProperties[static_cast<int32_t>(vk::Format::eAstc12x12SrgbBlock) + 1];
			for (int32_t FormatIndex = 0; FormatIndex < static_cast<int32_t>(vk::Format::eAstc12x12SrgbBlock) + 1; ++FormatIndex)
				_PhysicalDevice.getFormatProperties(vk::Format(FormatIndex), &FormatProperties[FormatIndex]);

			const char* VulkanDeviceExtensions[] =
			{
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
				VK_EXT_DEBUG_MARKER_EXTENSION_NAME
			};

			vector<vk::DeviceQueueCreateInfo> DeviceQueueCreateInfos;
			vector<vector<float>> Priorities;

			for (uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < QueueIndices.size(); ++QueueFamilyIndex)
			{
				if (QueueIndices[QueueFamilyIndex] > 0)
				{
					vector<float> QueueFamilyPriorities;
					QueueFamilyPriorities.resize(QueueIndices[QueueFamilyIndex]);
					for (uint32_t QueueIndex = 0; QueueIndex < QueueIndices[QueueFamilyIndex]; ++QueueIndex)
					{
						QueueFamilyPriorities[QueueIndex] = static_cast<float>(QueueIndices[QueueFamilyIndex] - QueueIndex - 1) / static_cast<float>(QueueIndices[QueueFamilyIndex] - 1);
					}
					Priorities.push_back(QueueFamilyPriorities);

					DeviceQueueCreateInfos.push_back(
						vk::DeviceQueueCreateInfo(
							vk::DeviceQueueCreateFlagBits(),
							QueueFamilyIndex,
							static_cast<uint32_t>(Priorities.front().size()),
							Priorities.front().data()
						)
					);
				}
			}

			vk::PhysicalDeviceSeparateDepthStencilLayoutsFeatures SeparateDepthStencilLayoutFeatures(true);

			vk::DeviceCreateInfo DeviceInfo(
				vk::DeviceCreateFlagBits(),
				static_cast<uint32_t>(DeviceQueueCreateInfos.size()), DeviceQueueCreateInfos.data(),
				VulkanLayers.size(), VulkanLayers.data(),
				ETERNAL_ARRAYSIZE(VulkanDeviceExtensions), VulkanDeviceExtensions,
				&PhysicalDeviceFeatures,
				&SeparateDepthStencilLayoutFeatures
			);

			VerifySuccess(_PhysicalDevice.createDevice(&DeviceInfo, nullptr, &_Device));

			_PhysicalDevice.getMemoryProperties(&_PhysicalDeviceMemoryProperties);
		}

		VulkanDevice::~VulkanDevice()
		{
			delete _DebugDispatchLoader;
			_DebugDispatchLoader = nullptr;
		}

		vk::Device& VulkanDevice::GetVulkanDevice()
		{
			return _Device;
		}

		uint32_t VulkanDevice::GetPushConstantMaxSize() const
		{
			static constexpr uint32_t ByteTo32Bits = 4;
			return _PhysicalDeviceProperties.limits.maxPushConstantsSize / ByteTo32Bits;
		}

		uint32_t VulkanDevice::GetNonCoherentMemoryAtomicSize() const
		{
			return static_cast<uint32_t>(_PhysicalDeviceProperties.limits.nonCoherentAtomSize);
		}

		uint32_t VulkanDevice::GetMinUniformBufferOffsetAlignment() const
		{
			return static_cast<uint32_t>(_PhysicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
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

			return ~0u;
		}

		void VulkanDevice::GetQueueFamilyIndices(QueueFamilyIndicesType& OutQueueFamilyIndices) const
		{
			OutQueueFamilyIndices =
			{
				_QueueFamilyIndexGraphics,
				_QueueFamilyIndexCompute,
				_QueueFamilyIndexCopy
			};
		}
	}
}

#endif
