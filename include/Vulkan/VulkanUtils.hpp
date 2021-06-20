#pragma once

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
		}
	}
}
