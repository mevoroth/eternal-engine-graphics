#pragma once

#include "Graphics/StencilTest.hpp"
#include "Graphics/Types/MemoryType.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class LogicOperator;
		enum class RootSignatureAccess;
		enum class AddressMode;
		enum class ComparisonFunction;
		enum class Blend;
		enum class BlendOperator;
		enum class BlendChannel;
		enum class BorderColor;
		class BlendState;
		class Viewport;

		namespace Vulkan
		{
			static constexpr uint32_t InvalidQueueFamilyIndex = ~0;
			static constexpr uint32_t InvalidQueueIndex = ~0;

			void VerifySuccess(const vk::Result& VulkanResult);
			vk::StencilOpState CreateVulkanStencilOperatorState(const StencilTest& InStencilTest, const StencilTest::FaceOperator& InFaceOperator);
			vk::PipelineColorBlendAttachmentState CreateVulkanPipelineColorBlendStateAttachmentState(const BlendState& InBlendState);
			vk::LogicOp ConvertLogicOperatorToVulkanLogicOperator(const LogicOperator& InLogicOperator);
			vk::SamplerAddressMode ConvertAddressModeToVulkanSamplerAddressMode(const AddressMode& InAddressMode);
			vk::CompareOp ConvertComparisonFunctionToVulkanComparisonOperator(const ComparisonFunction& InComparisonFunction);
			vk::BlendFactor ConvertBlendToVulkanBlendFactor(const Blend& InBlend);
			vk::BlendOp ConvertBlendOperatorToVulkanBlendOperator(const BlendOperator& InBlendOperator);
			vk::ColorComponentFlags ConvertBlendChannelToVulkanColorComponentFlags(const BlendChannel& InBlendChannel);
			vk::BorderColor ConvertBorderColorToVulkanBorderColor(const BorderColor& InBorderColor);
			vk::ShaderStageFlagBits ConvertRootSignatureAccessToShaderStageFlags(_In_ const RootSignatureAccess& InRootSignatureAccess);
			vk::Rect2D ConvertViewportToRect2D(_In_ const Viewport& InViewport);
			vk::MemoryPropertyFlagBits ConvertGraphicsMemoryFlagsToMemoryPropertyFlags(_In_ const GraphicsMemoryFlag& InMemoryFlags);
			void VerifyMemoryPropertyFlags(const vk::MemoryPropertyFlagBits& Flags);
		}
	}
}
