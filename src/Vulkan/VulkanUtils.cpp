#include "Vulkan/VulkanUtils.hpp"

#include <algorithm>
#include "Graphics/RootSignature.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/BlendState.hpp"
#include "Graphics/Sampler.hpp"
#include "Vulkan/VulkanHeader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		namespace Vulkan
		{
			static constexpr vk::LogicOp VULKAN_LOGIC_OPERATORS[] =
			{
				vk::LogicOp::eClear,
				vk::LogicOp::eSet,
				vk::LogicOp::eCopy,
				vk::LogicOp::eCopyInverted,
				vk::LogicOp::eNoOp,
				vk::LogicOp::eInvert,
				vk::LogicOp::eAnd,
				vk::LogicOp::eNand,
				vk::LogicOp::eOr,
				vk::LogicOp::eNor,
				vk::LogicOp::eXor,
				vk::LogicOp::eEquivalent,
				vk::LogicOp::eAndReverse,
				vk::LogicOp::eAndInverted,
				vk::LogicOp::eOrReverse,
				vk::LogicOp::eOrInverted
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_LOGIC_OPERATORS) == static_cast<int32_t>(LogicOperator::LOGIC_OPERATOR_COUNT), "Mismatch between abstraction and vulkan logic operators");

			static constexpr vk::BorderColor VULKAN_BORDER_COLORS[] =
			{
				vk::BorderColor::eFloatTransparentBlack,
				vk::BorderColor::eFloatOpaqueBlack,
				vk::BorderColor::eFloatOpaqueWhite
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_BORDER_COLORS) == static_cast<int32_t>(BorderColor::BORDER_COLOR_COUNT), "Mismatch between abstraction and vulkan border colors");

			static constexpr vk::DescriptorType VULKAN_DESCRIPTOR_TYPES[] =
			{
				vk::DescriptorType::eSampler,
				vk::DescriptorType::eSampledImage,
				vk::DescriptorType::eStorageImage,
				vk::DescriptorType::eUniformTexelBuffer,
				vk::DescriptorType::eStorageTexelBuffer,
				vk::DescriptorType::eUniformBuffer,
				vk::DescriptorType::eUniformBuffer,
				vk::DescriptorType::eStorageBuffer,
				vk::DescriptorType(~0) // Descriptor table doesn't exist on vulkan
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_DESCRIPTOR_TYPES) == static_cast<int32_t>(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "Mismatch between abstraction and vulkan descriptor types");

			static constexpr vk::ShaderStageFlags VULKAN_SHADER_STAGE_FLAGS[] =
			{
				vk::ShaderStageFlagBits::eVertex,
				vk::ShaderStageFlagBits::eTessellationControl,
				vk::ShaderStageFlagBits::eTessellationEvaluation,
				vk::ShaderStageFlagBits::eGeometry,
				vk::ShaderStageFlagBits::eFragment,
				vk::ShaderStageFlagBits::eCompute
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_SHADER_STAGE_FLAGS) == static_cast<int32_t>(RootSignatureAccess::ROOT_SIGNATURE_ACCESS_COUNT), "Mismatch between abstraction and vulkan shader stage flags");

			void VerifySuccess(const vk::Result& VulkanResult)
			{
				ETERNAL_ASSERT(VulkanResult == vk::Result::eSuccess);
			}

			vk::StencilOpState CreateVulkanStencilOperatorState(_In_ const StencilTest& InStencilTest, _In_ const StencilTest::FaceOperator& InFaceOperator)
			{
				return vk::StencilOpState(
					ConvertStencilOperatorToVulkanStencilOperator(InFaceOperator.Fail),
					ConvertStencilOperatorToVulkanStencilOperator(InFaceOperator.Pass),
					ConvertStencilOperatorToVulkanStencilOperator(InFaceOperator.FailDepth),
					ConvertComparisonFunctionToVulkanComparisonOperator(InFaceOperator.Comparison),
					static_cast<uint32_t>(InStencilTest.GetReadMask()),
					static_cast<uint32_t>(InStencilTest.GetWriteMask()),
					static_cast<uint32_t>(InStencilTest.GetReference())
				);
			}

			vk::PipelineColorBlendAttachmentState CreateVulkanPipelineColorBlendStateAttachmentState(_In_ const BlendState& InBlendState)
			{
				return vk::PipelineColorBlendAttachmentState(
					InBlendState.IsEnabled(),
					ConvertBlendToVulkanBlendFactor(InBlendState.GetSource()),
					ConvertBlendToVulkanBlendFactor(InBlendState.GetDestination()),
					ConvertBlendOperatorToVulkanBlendOperator(InBlendState.GetBlendOperator()),
					ConvertBlendToVulkanBlendFactor(InBlendState.GetSourceAlpha()),
					ConvertBlendToVulkanBlendFactor(InBlendState.GetDestinationAlpha()),
					ConvertBlendOperatorToVulkanBlendOperator(InBlendState.GetBlendAlphaOperator()),
					ConvertBlendChannelToVulkanColorComponentFlags(InBlendState.GetBlendChannel())
				);
			}

			vk::LogicOp ConvertLogicOperatorToVulkanLogicOperator(_In_ const LogicOperator& InLogicOperator)
			{
				return VULKAN_LOGIC_OPERATORS[static_cast<int32_t>(InLogicOperator)];
			}

			vk::SamplerAddressMode ConvertAddressModeToVulkanSamplerAddressMode(_In_ const AddressMode& InAddressMode)
			{
				return static_cast<vk::SamplerAddressMode>(InAddressMode);
			}

			vk::CompareOp ConvertComparisonFunctionToVulkanComparisonOperator(_In_ const ComparisonFunction& InComparisonFunction)
			{
				return static_cast<vk::CompareOp>(InComparisonFunction);
			}

			vk::StencilOp ConvertStencilOperatorToVulkanStencilOperator(_In_ const StencilTest::StencilOperator& InStencilOperator)
			{
				return static_cast<vk::StencilOp>(InStencilOperator);
			}

			vk::BlendFactor ConvertBlendToVulkanBlendFactor(_In_ const Blend& InBlend)
			{
				return static_cast<vk::BlendFactor>(InBlend);
			}

			vk::BlendOp ConvertBlendOperatorToVulkanBlendOperator(_In_ const BlendOperator& InBlendOperator)
			{
				return static_cast<vk::BlendOp>(InBlendOperator);
			}

			vk::ColorComponentFlags ConvertBlendChannelToVulkanColorComponentFlags(_In_ const BlendChannel& InBlendChannel)
			{
				return static_cast<vk::ColorComponentFlagBits>(InBlendChannel);
			}

			vk::BorderColor ConvertBorderColorToVulkanBorderColor(_In_ const BorderColor& InBorderColor)
			{
				return VULKAN_BORDER_COLORS[static_cast<int32_t>(InBorderColor)];
			}

			vk::Rect2D ConvertViewportToRect2D(_In_ const Viewport& InViewport)
			{
				return vk::Rect2D(
					vk::Offset2D(InViewport.GetX(), InViewport.GetY()),
					vk::Extent2D(InViewport.GetWidth(), InViewport.GetHeight())
				);
			}

			vk::MemoryPropertyFlagBits ConvertGraphicsMemoryFlagsToMemoryPropertyFlags(_In_ const GraphicsMemoryFlag& InMemoryFlags)
			{
				int VulkanMemoryPropertyFlags = 0x0;
				int MemoryFlagsInt = int(InMemoryFlags);

				if (MemoryFlagsInt & int(GraphicsMemoryFlag::MEMORY_FLAG_GPU))
					VulkanMemoryPropertyFlags |= int(vk::MemoryPropertyFlagBits::eDeviceLocal);

				if (MemoryFlagsInt & int(GraphicsMemoryFlag::MEMORY_FLAG_MAPPABLE))
					VulkanMemoryPropertyFlags |= int(vk::MemoryPropertyFlagBits::eHostVisible);

				if (MemoryFlagsInt & int(GraphicsMemoryFlag::MEMORY_FLAG_MANAGED_CACHE))
					VulkanMemoryPropertyFlags |= int(vk::MemoryPropertyFlagBits::eHostCoherent);

				if (MemoryFlagsInt & int(GraphicsMemoryFlag::MEMORY_FLAG_CPU_CACHED))
					VulkanMemoryPropertyFlags |= int(vk::MemoryPropertyFlagBits::eHostCached);

				return vk::MemoryPropertyFlagBits(VulkanMemoryPropertyFlags);
			}

			void VerifyMemoryPropertyFlags(const vk::MemoryPropertyFlagBits& Flags)
			{
				vk::MemoryPropertyFlagBits AllowedMemoryProperties[] =
				{
					vk::MemoryPropertyFlagBits(),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
				};
				vk::MemoryPropertyFlagBits* EndAllowedMemoryProperties = AllowedMemoryProperties + ETERNAL_ARRAYSIZE(AllowedMemoryProperties);
				ETERNAL_ASSERT(std::find(AllowedMemoryProperties, EndAllowedMemoryProperties, Flags) != EndAllowedMemoryProperties);
			}

			vk::DescriptorType ConvertRootSignatureParameterTypeToVulkanDescriptorType(const RootSignatureParameterType& InRootSignatureParameterType)
			{
				ETERNAL_ASSERT(InRootSignatureParameterType != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);
				return VULKAN_DESCRIPTOR_TYPES[static_cast<int32_t>(InRootSignatureParameterType)];
			}

			vk::ShaderStageFlags ConvertRootSignatureAccessToShaderStageFlags(_In_ const RootSignatureAccess& InRootSignatureAccess)
			{
				ETERNAL_ASSERT(InRootSignatureAccess != RootSignatureAccess::ROOT_SIGNATURE_ACCESS_INVALID);
				return VULKAN_SHADER_STAGE_FLAGS[static_cast<int32_t>(InRootSignatureAccess)];
			}
		}
	}
}
