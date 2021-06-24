#pragma once

#include "Graphics/StencilTest.hpp"
#include "Graphics/Types/MemoryType.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class LogicOp;
		enum class RootSignatureAccess;
		class Viewport;

		namespace Vulkan
		{
			static constexpr uint32_t InvalidQueueFamilyIndex = ~0;
			static constexpr uint32_t InvalidQueueIndex = ~0;

			void VerifySuccess(const vk::Result& VulkanResult);
			vk::StencilOpState CreateStencilOpState(const StencilTest& InStencilTest, const StencilTest::FaceOperator& InFaceOperator);
			vk::LogicOp ConvertLogicOpToVulkanLogicOp(const LogicOp& InLogicOp);
			vk::ShaderStageFlagBits ConvertRootSignatureAccessToShaderStageFlags(_In_ const RootSignatureAccess& InRootSignatureAccess);
			vk::Rect2D ConvertViewportToRect2D(_In_ const Viewport& InViewport);
			vk::MemoryPropertyFlagBits ConvertGraphicsMemoryFlagsToMemoryPropertyFlags(_In_ const GraphicsMemoryFlag& InMemoryFlags);
			void VerifyMemoryPropertyFlags(const vk::MemoryPropertyFlagBits& Flags);
		}
	}
}
