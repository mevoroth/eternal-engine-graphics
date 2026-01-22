#pragma once

#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanHeader.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Graphics/CommandUtils.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include <vulkan/vulkan.hpp>
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		namespace VulkanPrivate
		{
			class EternalDeviceDispatchLoader;

			void GatherVulkanInstanceExtensions(_Inout_ std::vector<const char*>& InOutVulkanInstanceExtensions);
		}

		using namespace std;
		class OutputDevice;

		using QueueFamilyIndicesType = std::array<uint32_t, static_cast<uint32_t>(CommandType::COMMAND_TYPE_COUNT)>;

		class EternalDebugDispatchLoader
		{
		public:
			EternalDebugDispatchLoader(vk::Instance& InInstance);

			size_t getVkHeaderVersion() const;

			VkResult vkSetDebugUtilsObjectNameEXT(VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) const;
			void vkCmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const;
			void vkCmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) const;

		private:
			vk::Instance& _Instance;
			PFN_vkSetDebugUtilsObjectNameEXT	_vkSetDebugUtilsObjectName;
			PFN_vkCmdDebugMarkerBeginEXT		_vkCmdDebugMarkerBegin;
			PFN_vkCmdDebugMarkerEndEXT			_vkCmdDebugMarkerEnd;
		};

		class VulkanDevice final : public Device
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

			VulkanDevice(_In_ OutputDevice& InOutputDevice);
			~VulkanDevice();

			virtual uint32_t GetDeviceMask() const override final { return 0x1; }
			virtual DeviceType GetDeviceType() const override final { return DeviceType::DEVICE_TYPE_VULKAN; }

			vk::Device& GetVulkanDevice();
			vk::PhysicalDevice& GetPhysicalDevice() { return _PhysicalDevice; }
			vk::Instance& GetInstance() { return _Instance; }
			const EternalDebugDispatchLoader& GetDebugDispatchLoader() const { return *_DebugDispatchLoader; };
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
			void GetQueueFamilyIndices(QueueFamilyIndicesType& OutQueueFamilyIndices) const;
			uint32_t GetPushConstantMaxSize() const;
			uint32_t GetNonCoherentMemoryAtomicSize() const;
			uint32_t GetMinUniformBufferOffsetAlignment() const;

		private:
			EternalDebugDispatchLoader*			_DebugDispatchLoader		= nullptr;
			
			vk::PhysicalDeviceMemoryProperties	_PhysicalDeviceMemoryProperties;
			vk::DebugReportCallbackEXT			_DebugReportCallback;
			vk::DebugUtilsMessengerEXT			_DebugUtilsMessengerCallback;
			vk::Instance						_Instance;
			vk::PhysicalDevice					_PhysicalDevice;
			vk::Device							_Device;
			vk::PhysicalDeviceProperties		_PhysicalDeviceProperties;
			uint32_t							_QueueFamilyPropertiesCount	= 0;
			uint32_t							_VulkanVersion				= 0;
			uint32_t							_QueueFamilyIndexGraphics	= Vulkan::InvalidQueueFamilyIndex;
			uint32_t							_QueueFamilyIndexCompute	= Vulkan::InvalidQueueFamilyIndex;
			uint32_t							_QueueFamilyIndexCopy		= Vulkan::InvalidQueueFamilyIndex;
			uint32_t							_QueueIndexGraphics			= Vulkan::InvalidQueueIndex;
			uint32_t							_QueueIndexCompute			= Vulkan::InvalidQueueIndex;
			uint32_t							_QueueIndexCopy				= Vulkan::InvalidQueueIndex;
		};
	}
}

namespace VULKAN_HPP_NAMESPACE
{
	namespace detail
	{
		template <>
		struct isDispatchLoader<Eternal::Graphics::EternalDebugDispatchLoader>
		{
			static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = true;
		};

		template <>
		struct isDispatchLoader<Eternal::Graphics::VulkanPrivate::EternalDeviceDispatchLoader>
		{
			static VULKAN_HPP_CONST_OR_CONSTEXPR bool value = true;
		};
	}
}

#endif
