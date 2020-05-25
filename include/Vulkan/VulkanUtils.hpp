#pragma once

#include "Macros/Macros.hpp"
#include <vulkan/vulkan.hpp>
#include "Graphics/StencilTest.hpp"
#include "NextGenGraphics/Types/MemoryType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class LogicOp;
		enum class RootSignatureAccess;
		class Viewport;

		namespace Vulkan
		{
			void VerifySuccess(const vk::Result& VulkanResult);
			vk::StencilOpState CreateStencilOpState(const StencilTest& InStencilTest, const StencilTest::FaceOperator& InFaceOperator);
			vk::LogicOp ConvertLogicOpToVulkanLogicOp(const LogicOp& InLogicOp);
			vk::ShaderStageFlagBits ConvertRootSignatureAccessToShaderStageFlags(_In_ const RootSignatureAccess& InRootSignatureAccess);
			vk::Rect2D ConvertViewportToRect2D(_In_ const Viewport& InViewport);
			vk::MemoryPropertyFlagBits ConvertGraphicsMemoryFlagsToMemoryPropertyFlags(_In_ const GraphicsMemoryFlag& InMemoryFlags);
			void VerifyMemoryPropertyFlags(const vk::MemoryPropertyFlagBits& Flags);

			static bool CheckMemoryPropertiesFlags(_In_ const VkMemoryPropertyFlagBits& Flags)
			{
				VkMemoryPropertyFlagBits AllowedMemoryProperties[] =
				{
					(VkMemoryPropertyFlagBits)0,
					(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT),
					(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
					(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT),
					(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					(VkMemoryPropertyFlagBits)(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
				};
				VkMemoryPropertyFlagBits* EndAllowedMemoryProperties = AllowedMemoryProperties + ETERNAL_ARRAYSIZE(AllowedMemoryProperties);
				return std::find(AllowedMemoryProperties, EndAllowedMemoryProperties, Flags) != EndAllowedMemoryProperties;
			}

		}
	}
}
