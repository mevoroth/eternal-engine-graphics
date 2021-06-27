#pragma once

#include "Graphics/StencilTest.hpp"
#include "Graphics/Types/MemoryType.hpp"
#include "Graphics/StencilTest.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class LogicOperator;
		enum class RootSignatureAccess;
		enum class RootSignatureParameterType;
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

			void VerifySuccess(_In_ const vk::Result& VulkanResult);
			vk::StencilOpState CreateVulkanStencilOperatorState(_In_ const StencilTest& InStencilTest, _In_ const StencilTest::FaceOperator& InFaceOperator);
			vk::PipelineColorBlendAttachmentState CreateVulkanPipelineColorBlendStateAttachmentState(_In_ const BlendState& InBlendState);
			vk::LogicOp ConvertLogicOperatorToVulkanLogicOperator(_In_ const LogicOperator& InLogicOperator);
			vk::SamplerAddressMode ConvertAddressModeToVulkanSamplerAddressMode(_In_ const AddressMode& InAddressMode);
			vk::CompareOp ConvertComparisonFunctionToVulkanComparisonOperator(_In_ const ComparisonFunction& InComparisonFunction);
			vk::StencilOp ConvertStencilOperatorToVulkanStencilOperator(_In_ const StencilTest::StencilOperator& InStencilOperator);
			vk::BlendFactor ConvertBlendToVulkanBlendFactor(_In_ const Blend& InBlend);
			vk::BlendOp ConvertBlendOperatorToVulkanBlendOperator(_In_ const BlendOperator& InBlendOperator);
			vk::ColorComponentFlags ConvertBlendChannelToVulkanColorComponentFlags(_In_ const BlendChannel& InBlendChannel);
			vk::BorderColor ConvertBorderColorToVulkanBorderColor(_In_ const BorderColor& InBorderColor);
			vk::Rect2D ConvertViewportToRect2D(_In_ const Viewport& InViewport);
			vk::MemoryPropertyFlagBits ConvertGraphicsMemoryFlagsToMemoryPropertyFlags(_In_ const GraphicsMemoryFlag& InMemoryFlags);
			void VerifyMemoryPropertyFlags(_In_ const vk::MemoryPropertyFlagBits& Flags);
			vk::DescriptorType ConvertRootSignatureParameterTypeToVulkanDescriptorType(_In_ const RootSignatureParameterType& InRootSignatureParameterType);
			vk::ShaderStageFlags ConvertRootSignatureAccessToShaderStageFlags(_In_ const RootSignatureAccess& InAccess);
		}
	}
}
