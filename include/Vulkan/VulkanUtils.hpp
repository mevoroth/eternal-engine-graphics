#pragma once

#include "Graphics/StencilTest.hpp"
#include "Graphics/Types/MemoryType.hpp"
#include "Graphics/StencilTest.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class CommandType;
		enum class LogicOperator;
		enum class RootSignatureAccess;
		enum class RootSignatureParameterType;
		enum class AddressMode;
		enum class ComparisonFunction;
		enum class Blend;
		enum class BlendOperator;
		enum class BlendChannel;
		enum class BorderColor;
		enum class TransitionState;
		enum class LoadOperator;
		enum class StoreOperator;
		enum class PrimitiveTopology;
		enum class BufferResourceUsage;
		enum class TextureResourceUsage;
		enum class ResourceMemoryType;
		enum class ResourceDimension;
		enum class ShaderTypeFlags;
		enum class HLSLRegisterType;

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
			vk::ColorComponentFlagBits ConvertBlendChannelToVulkanColorComponentFlags(_In_ const BlendChannel& InBlendChannel);
			vk::BorderColor ConvertBorderColorToVulkanBorderColor(_In_ const BorderColor& InBorderColor);
			vk::Rect2D ConvertViewportToRect2D(_In_ const Viewport& InViewport);
			vk::AttachmentLoadOp ConvertLoadOperatorToVulkanAttachmentLoadOperator(_In_ const LoadOperator& InLoadOperator);
			vk::AttachmentStoreOp ConvertStoreOperatorToVulkanAttachmentStoreOperator(_In_ const StoreOperator& InStoreOperator);
			vk::MemoryPropertyFlagBits ConvertGraphicsMemoryFlagsToMemoryPropertyFlags(_In_ const GraphicsMemoryFlag& InMemoryFlags);
			vk::DescriptorType ConvertRootSignatureParameterTypeToVulkanDescriptorType(_In_ const RootSignatureParameterType& InRootSignatureParameterType);
			vk::ShaderStageFlagBits ConvertRootSignatureAccessToShaderStageFlags(_In_ const RootSignatureAccess& InAccess);
			vk::AccessFlagBits ConvertTransitionStateToVulkanAccessFlags(_In_ const TransitionState& InTransitionState);
			vk::ImageLayout ConvertTransitionStateToVulkanImageLayout(_In_ const TransitionState& InTransitionState);
			vk::PipelineStageFlagBits ConvertCommandTypeAndTransitionStateToPipelineStageFlags(_In_ const CommandType& InCommandType, _In_ const TransitionState& InTransitionState);
			vk::PrimitiveTopology ConvertPrimitiveTopologyToVulkanPrimitiveTopology(_In_ const PrimitiveTopology& InPrimitiveTopology);
			vk::BufferUsageFlagBits ConvertBufferResourceUsageToVulkanBufferUsageFlags(_In_ const BufferResourceUsage& InBufferResourceUsage);
			vk::ImageUsageFlagBits ConvertTextureResourceUsageToVulkanImageUsageFlags(_In_ const TextureResourceUsage& InTextureResourceUsage);
			void VerifyMemoryPropertyFlags(_In_ const vk::MemoryPropertyFlagBits& Flags);
			vk::MemoryPropertyFlagBits ConvertResourceMemoryTypeToMemoryPropertyFlagBits(_In_ const ResourceMemoryType& InResourceMemoryType);
			vk::ImageCreateFlagBits ConvertResourceDimensionToVulkanImageCreateFlags(_In_ const ResourceDimension& InResourceDimension);
			vk::ImageType ConvertResourceDimensionToVulkanImageType(_In_ const ResourceDimension& InResourceDimension);
			vk::ShaderStageFlagBits ConvertShaderTypeFlagsToVulkanShaderStageFlags(_In_ const ShaderTypeFlags& InShaderTypeFlags);
			uint32_t ConvertHLSLRegisterTypeToVulkanShaderBindingBase(const HLSLRegisterType& InHLSLRegisterType);

			inline vk::PipelineStageFlagBits& operator|=(_Inout_ vk::PipelineStageFlagBits& InOutPipelineStageFlags, _In_ const vk::PipelineStageFlagBits& InPipelineStageFlags)
			{
				InOutPipelineStageFlags = static_cast<vk::PipelineStageFlagBits>(
					static_cast<uint32_t>(InOutPipelineStageFlags) | static_cast<uint32_t>(InPipelineStageFlags)
				);
				return InOutPipelineStageFlags;
			}
		}
	}
}
