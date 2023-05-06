#include "Vulkan/VulkanUtils.hpp"

#include <algorithm>
#include "Graphics/CommandUtils.hpp"
#include "Graphics/RootSignature.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/BlendState.hpp"
#include "Graphics/Sampler.hpp"
#include "Graphics/RenderPass.hpp"
#include "Graphics/Pipeline.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/ShaderType.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Math/Math.hpp"
#include <cstdlib>

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
				vk::DescriptorType::eStorageBuffer,
				vk::DescriptorType::eStorageTexelBuffer,
				vk::DescriptorType::eUniformBuffer,
				vk::DescriptorType::eUniformBuffer,
				vk::DescriptorType::eStorageBuffer,
				vk::DescriptorType(~0) // Descriptor table doesn't exist on vulkan
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_DESCRIPTOR_TYPES) == static_cast<int32_t>(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "Mismatch between abstraction and vulkan descriptor types");

			static constexpr vk::ShaderStageFlagBits VULKAN_SHADER_STAGE_FLAGS[] =
			{
				vk::ShaderStageFlagBits::eVertex,
				vk::ShaderStageFlagBits::eTessellationControl,
				vk::ShaderStageFlagBits::eTessellationEvaluation,
				vk::ShaderStageFlagBits::eGeometry,
				vk::ShaderStageFlagBits::eFragment,
				vk::ShaderStageFlagBits::eCompute,
				vk::ShaderStageFlagBits::eTaskNV,
				vk::ShaderStageFlagBits::eMeshNV
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_SHADER_STAGE_FLAGS) == static_cast<int32_t>(RootSignatureAccess::ROOT_SIGNATURE_ACCESS_COUNT), "Mismatch between abstraction and vulkan shader stage flags");

			static constexpr vk::PipelineStageFlagBits VULKAN_COMMAND_TYPES_TO_PIPELINE_STAGE_FLAGS[] =
			{
				vk::PipelineStageFlagBits::eAllGraphics,
				vk::PipelineStageFlagBits::eComputeShader,
				vk::PipelineStageFlagBits::eHost
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_COMMAND_TYPES_TO_PIPELINE_STAGE_FLAGS) == static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT), "Mismatch between abstraction and vulkan command types to pipeline stage flags");

			static constexpr vk::AttachmentLoadOp VULKAN_ATTACHMENT_LOAD_OPERATORS[] =
			{
				vk::AttachmentLoadOp::eDontCare,
				vk::AttachmentLoadOp::eLoad,
				vk::AttachmentLoadOp::eClear
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_ATTACHMENT_LOAD_OPERATORS) == static_cast<int32_t>(LoadOperator::COUNT), "Mismatch between abstraction and vulkan attachment load operators");

			static constexpr vk::AttachmentStoreOp VULKAN_ATTACHMENT_STORE_OPERATORS[] =
			{
				vk::AttachmentStoreOp::eDontCare,
				vk::AttachmentStoreOp::eStore,
				vk::AttachmentStoreOp::eStore
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_ATTACHMENT_STORE_OPERATORS) == static_cast<int32_t>(StoreOperator::COUNT), "Mismatch between abstraction and vulkan attachment store operators");

			static constexpr vk::PrimitiveTopology VULKAN_PRIMITIVE_TOPOLOGIES[] =
			{
				vk::PrimitiveTopology::ePointList,
				vk::PrimitiveTopology::eLineList,
				vk::PrimitiveTopology::eLineStrip,
				vk::PrimitiveTopology::eTriangleList,
				vk::PrimitiveTopology::eTriangleStrip,
				vk::PrimitiveTopology::eLineListWithAdjacency,
				vk::PrimitiveTopology::eLineStripWithAdjacency,
				vk::PrimitiveTopology::eTriangleListWithAdjacency,
				vk::PrimitiveTopology::eTriangleStripWithAdjacency,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList,
				vk::PrimitiveTopology::ePatchList
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_PRIMITIVE_TOPOLOGIES) == static_cast<int32_t>(PrimitiveTopology::PRIMITIVE_TOPOLOGY_COUNT), "Mismatch between abstraction and vulkan primitive topologies");

			static constexpr vk::MemoryPropertyFlagBits VULKAN_MEMORY_PROPERTY_FLAGS[] =
			{
				vk::MemoryPropertyFlagBits::eDeviceLocal,
				vk::MemoryPropertyFlagBits::eHostVisible,
				static_cast<vk::MemoryPropertyFlagBits>(static_cast<uint32_t>(vk::MemoryPropertyFlagBits::eHostVisible) | static_cast<uint32_t>(vk::MemoryPropertyFlagBits::eHostCached))
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_MEMORY_PROPERTY_FLAGS) == static_cast<int32_t>(ResourceMemoryType::RESOURCE_MEMORY_TYPE_COUNT), "Mismatch between abstraction and vulkan memory property flags");

			static constexpr vk::ImageType VULKAN_IMAGE_TYPES[] =
			{
				vk::ImageType::e1D,
				vk::ImageType::e1D,
				vk::ImageType::e1D,
				vk::ImageType::e1D,
				vk::ImageType::e2D,
				vk::ImageType::e2D,
				vk::ImageType::e3D,
				vk::ImageType::e2D,
				vk::ImageType::e2D
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_IMAGE_TYPES) == static_cast<int32_t>(ResourceDimension::RESOURCE_DIMENSION_COUNT), "Mismatch between abstraction and vulkan image types");

			static constexpr vk::BlendFactor VULKAN_BLEND_STATES[] =
			{
				vk::BlendFactor::eZero,
				vk::BlendFactor::eOne,

				// SOURCE
				vk::BlendFactor::eSrcColor,
				vk::BlendFactor::eOneMinusSrcColor,
				vk::BlendFactor::eSrcAlpha,
				vk::BlendFactor::eOneMinusSrcAlpha,

				// DESTINATION
				vk::BlendFactor::eDstAlpha,
				vk::BlendFactor::eOneMinusDstAlpha,
				vk::BlendFactor::eDstColor,
				vk::BlendFactor::eOneMinusDstColor,

				vk::BlendFactor::eSrcAlphaSaturate,

				// BLEND FACTOR
				vk::BlendFactor::eConstantColor,
				vk::BlendFactor::eOneMinusConstantColor,
				vk::BlendFactor::eConstantAlpha,
				vk::BlendFactor::eOneMinusConstantAlpha,

				vk::BlendFactor::eSrc1Color,
				vk::BlendFactor::eOneMinusSrc1Color,
				vk::BlendFactor::eSrc1Alpha,
				vk::BlendFactor::eOneMinusSrc1Alpha
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_BLEND_STATES) == static_cast<int32_t>(Blend::BLEND_COUNT), "Mismatch between abstraction and blend states");

			static constexpr vk::PolygonMode VULKAN_POLYGON_MODES[] =
			{
				vk::PolygonMode::eLine,
				vk::PolygonMode::eFill
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_POLYGON_MODES) == static_cast<int32_t>(FillMode::FILL_MODE_COUNT), "Mismatch between abstraction and vulkan polygon modes");

			static constexpr vk::CullModeFlagBits VULKAN_CULL_MODE_FLAGS[] =
			{
				vk::CullModeFlagBits::eNone,
				vk::CullModeFlagBits::eFront,
				vk::CullModeFlagBits::eBack
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_CULL_MODE_FLAGS) == static_cast<int32_t>(CullMode::CULL_MODE_COUNT), "Mismatch between abstraction and vulkan cull mode flags");

			static constexpr vk::FrontFace VULKAN_FRONT_FACES[] =
			{
				vk::FrontFace::eCounterClockwise,
				vk::FrontFace::eClockwise
			};
			ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VULKAN_FRONT_FACES) == static_cast<int32_t>(FrontFace::FRONT_FACE_COUNT), "Mismatch between abstraction and vulkan front faces");

			static constexpr uint32_t VULKAN_SHADER_REGISTER_OFFSETS[] =
			{
				VulkanGraphicsContext::ShaderRegisterShaderResourcesOffset,
				VulkanGraphicsContext::ShaderRegisterConstantBuffersOffset,
				VulkanGraphicsContext::ShaderRegisterUnorderedAccessesOffset,
				VulkanGraphicsContext::ShaderRegisterSamplersOffset
			};

			static constexpr TransitionState CPURead_CPUWrite_State		= TransitionState::TRANSITION_CPU_READ
																		| TransitionState::TRANSITION_CPU_WRITE;

			static constexpr TransitionState CopyRead_CopyWrite_State	= TransitionState::TRANSITION_COPY_READ
																		| TransitionState::TRANSITION_COPY_WRITE;
			void VerifySuccess(_In_ const vk::Result& VulkanResult)
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
				return VULKAN_BLEND_STATES[static_cast<int32_t>(InBlend)];
			}

			vk::BlendOp ConvertBlendOperatorToVulkanBlendOperator(_In_ const BlendOperator& InBlendOperator)
			{
				return static_cast<vk::BlendOp>(InBlendOperator);
			}

			vk::ColorComponentFlagBits ConvertBlendChannelToVulkanColorComponentFlags(_In_ const BlendChannel& InBlendChannel)
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

			vk::AttachmentLoadOp ConvertLoadOperatorToVulkanAttachmentLoadOperator(_In_ const LoadOperator& InLoadOperator)
			{
				return VULKAN_ATTACHMENT_LOAD_OPERATORS[static_cast<int32_t>(InLoadOperator)];
			}

			vk::AttachmentStoreOp ConvertStoreOperatorToVulkanAttachmentStoreOperator(_In_ const StoreOperator& InStoreOperator)
			{
				return VULKAN_ATTACHMENT_STORE_OPERATORS[static_cast<int32_t>(InStoreOperator)];
			}

			vk::MemoryPropertyFlagBits ConvertGraphicsMemoryFlagsToMemoryPropertyFlags(_In_ const GraphicsMemoryFlag& InMemoryFlags)
			{
				ETERNAL_BREAK(); // Deprecated
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

			void VerifyMemoryPropertyFlags(_In_ const vk::MemoryPropertyFlagBits& Flags)
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
					//vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
				};
				vk::MemoryPropertyFlagBits* EndAllowedMemoryProperties = AllowedMemoryProperties + ETERNAL_ARRAYSIZE(AllowedMemoryProperties);
				ETERNAL_ASSERT(std::find(AllowedMemoryProperties, EndAllowedMemoryProperties, Flags) != EndAllowedMemoryProperties);
			}

			vk::MemoryPropertyFlagBits ConvertResourceMemoryTypeToMemoryPropertyFlagBits(_In_ const ResourceMemoryType& InResourceMemoryType)
			{
				return VULKAN_MEMORY_PROPERTY_FLAGS[static_cast<int32_t>(InResourceMemoryType)];
			}

			vk::DescriptorType ConvertRootSignatureParameterTypeToVulkanDescriptorType(_In_ const RootSignatureParameterType& InRootSignatureParameterType)
			{
				ETERNAL_ASSERT(InRootSignatureParameterType != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);
				return VULKAN_DESCRIPTOR_TYPES[static_cast<int32_t>(InRootSignatureParameterType)];
			}

			vk::ShaderStageFlagBits ConvertRootSignatureAccessToShaderStageFlags(_In_ const RootSignatureAccess& InRootSignatureAccess)
			{
				ETERNAL_ASSERT(InRootSignatureAccess != RootSignatureAccess::ROOT_SIGNATURE_ACCESS_INVALID);
				return VULKAN_SHADER_STAGE_FLAGS[static_cast<int32_t>(InRootSignatureAccess)];
			}

			vk::AccessFlagBits& operator|=(_Inout_ vk::AccessFlagBits& InOutAccessFlags, _In_ const vk::AccessFlagBits& InOtherAccessFlags)
			{
				InOutAccessFlags = static_cast<vk::AccessFlagBits>(
					static_cast<uint32_t>(InOutAccessFlags) | static_cast<uint32_t>(InOtherAccessFlags)
				);
				return InOutAccessFlags;
			}

			vk::AccessFlagBits& operator|=(_Inout_ vk::AccessFlagBits& InOutAccessFlags, _In_ const vk::AccessFlags& InOtherAccessFlags)
			{
				InOutAccessFlags = static_cast<vk::AccessFlagBits>(
					static_cast<uint32_t>(InOutAccessFlags) | static_cast<uint32_t>(InOtherAccessFlags)
					);
				return InOutAccessFlags;
			}
			
			vk::AccessFlagBits ConvertTransitionStateToVulkanAccessFlags(_In_ const TransitionState& InTransitionState)
			{
				// Resolve destination & resolve source not implemented
				// Vulkan input attachment read not implemented

				using namespace Math;


				static constexpr TransitionState StandaloneState												= TransitionState::TRANSITION_PREINITIALIZED
																												| TransitionState::TRANSITION_PRESENT;

				static constexpr TransitionState Indirect_IndexRead_VertexBufferRead_ConstantBufferRead_State	= TransitionState::TRANSITION_INDIRECT
																												| TransitionState::TRANSITION_INDEX_READ
																												| TransitionState::TRANSITION_VERTEX_BUFFER_READ
																												| TransitionState::TRANSITION_CONSTANT_BUFFER_READ;

				static constexpr TransitionState NonPixelShaderRead_PixelShaderRead_State						= TransitionState::TRANSITION_NON_PIXEL_SHADER_READ
																												| TransitionState::TRANSITION_PIXEL_SHADER_READ;

				static constexpr TransitionState DepthStencilRead_DepthStencilWrite_CopyRead_CopyWrite_State	= TransitionState::TRANSITION_DEPTH_STENCIL_READ
																												| TransitionState::TRANSITION_DEPTH_STENCIL_WRITE
																												| CopyRead_CopyWrite_State;

				static const vk::AccessFlags ColorAttachmentRead_ColorAttachmentWrite_VulkanAccessFlags			= vk::AccessFlagBits::eColorAttachmentRead
																												| vk::AccessFlagBits::eColorAttachmentWrite;

				if (InTransitionState == TransitionState::TRANSITION_UNDEFINED
					|| OnlyHasFlags(InTransitionState, StandaloneState))
				{
					if (InTransitionState != TransitionState::TRANSITION_UNDEFINED &&
						OnlyHasFlags(InTransitionState, StandaloneState))
					{
						ETERNAL_ASSERT(IsPowerOfTwo<uint32_t>(static_cast<uint32_t>(InTransitionState)));
					}
					return vk::AccessFlagBits::eNoneKHR;
				}

				ETERNAL_ASSERT(OnlyHasFlags(InTransitionState, ~StandaloneState));

				if ((InTransitionState & TransitionState::TRANSITION_GENERIC_READ) == TransitionState::TRANSITION_GENERIC_READ)
					return vk::AccessFlagBits::eMemoryRead;

				vk::AccessFlagBits States = vk::AccessFlagBits::eNoneKHR;

				if ((InTransitionState & NonPixelShaderRead_PixelShaderRead_State) != TransitionState::TRANSITION_UNDEFINED)
					States |= vk::AccessFlagBits::eShaderRead;

				if ((InTransitionState & TransitionState::TRANSITION_RENDER_TARGET) != TransitionState::TRANSITION_UNDEFINED)
					States |= ColorAttachmentRead_ColorAttachmentWrite_VulkanAccessFlags;

				States |= static_cast<vk::AccessFlagBits>(InTransitionState & Indirect_IndexRead_VertexBufferRead_ConstantBufferRead_State);
				States |= static_cast<vk::AccessFlagBits>(InTransitionState & TransitionState::TRANSITION_SHADER_WRITE);
				States |= static_cast<vk::AccessFlagBits>((InTransitionState & DepthStencilRead_DepthStencilWrite_CopyRead_CopyWrite_State) << 1);
				States |= static_cast<vk::AccessFlagBits>((InTransitionState & CPURead_CPUWrite_State) >> 1);

				return States;
			}

			vk::ImageLayout ConvertTransitionStateToVulkanImageLayout(_In_ const TransitionState& InTransitionState)
			{
				// Resolve destination & resolve source not implemented

				static constexpr TransitionState NonPixelShaderRead_PixelShaderRead_State	= TransitionState::TRANSITION_NON_PIXEL_SHADER_READ
																							| TransitionState::TRANSITION_PIXEL_SHADER_READ;

				static constexpr TransitionState GeneralState								= TransitionState::TRANSITION_INDIRECT
																							| TransitionState::TRANSITION_INDEX_READ
																							| TransitionState::TRANSITION_VERTEX_BUFFER_READ
																							| TransitionState::TRANSITION_CONSTANT_BUFFER_READ
																							| TransitionState::TRANSITION_SHADER_WRITE
																							//| TransitionState::TRANSITION_RESOLVE_DESTINATION
																							//| TransitionState::TRANSITION_RESOLVE_SOURCE
																							| TransitionState::TRANSITION_CPU_READ
																							| TransitionState::TRANSITION_CPU_WRITE;

				// Process specific states first
				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_RENDER_TARGET))
					return vk::ImageLayout::eColorAttachmentOptimal;

				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_DEPTH_STENCIL_WRITE))
					return vk::ImageLayout::eDepthStencilAttachmentOptimal;

				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_DEPTH_STENCIL_READ))
					return vk::ImageLayout::eDepthReadOnlyOptimal;

				if (OnlyHasFlags(InTransitionState, NonPixelShaderRead_PixelShaderRead_State))
					return vk::ImageLayout::eShaderReadOnlyOptimal;

				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_COPY_READ))
					return vk::ImageLayout::eTransferSrcOptimal;

				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_COPY_WRITE))
					return vk::ImageLayout::eTransferDstOptimal;

				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_PREINITIALIZED))
					return vk::ImageLayout::ePreinitialized;

				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_PRESENT))
					return vk::ImageLayout::ePresentSrcKHR;

				// Process general states then
				if (OnlyHasFlags(InTransitionState, GeneralState))
					return vk::ImageLayout::eGeneral;
				
				// Undefined
				return vk::ImageLayout::eUndefined;
			}

			static vk::PipelineStageFlagBits ConvertCommandTypeToPipelineStageFlags(_In_ const CommandType& InCommandType)
			{
				return VULKAN_COMMAND_TYPES_TO_PIPELINE_STAGE_FLAGS[static_cast<int32_t>(InCommandType)];
			}

			vk::PipelineStageFlagBits ConvertCommandTypeAndTransitionStateToPipelineStageFlags(_In_ const CommandType& InCommandType, _In_ const TransitionState& InTransitionState)
			{
				using namespace Math;

				static constexpr TransitionState GraphicsOnlyState		= TransitionState::TRANSITION_INDIRECT
																		| TransitionState::TRANSITION_INDEX_READ
																		| TransitionState::TRANSITION_VERTEX_BUFFER_READ
																		| TransitionState::TRANSITION_PIXEL_SHADER_READ
																		| TransitionState::TRANSITION_RENDER_TARGET
																		| TransitionState::TRANSITION_DEPTH_STENCIL_READ
																		| TransitionState::TRANSITION_DEPTH_STENCIL_WRITE;

				static constexpr TransitionState UndefinedPipelineState	= TransitionState::TRANSITION_CONSTANT_BUFFER_READ
																		| TransitionState::TRANSITION_NON_PIXEL_SHADER_READ
																		| TransitionState::TRANSITION_SHADER_WRITE;

				if (InTransitionState == TransitionState::TRANSITION_UNDEFINED
					|| OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_PREINITIALIZED))
					return vk::PipelineStageFlagBits::eHost;

				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_PRESENT))
					return vk::PipelineStageFlagBits::eAllGraphics;

				if (OnlyHasFlags(InTransitionState, TransitionState::TRANSITION_COPY_WRITE))
				{
					return vk::PipelineStageFlagBits::eTransfer;
				}

				vk::PipelineStageFlagBits PipelineStageFlags = vk::PipelineStageFlagBits::eNoneKHR;

				PipelineStageFlags |= ((InTransitionState & GraphicsOnlyState) != TransitionState::TRANSITION_UNDEFINED)		? vk::PipelineStageFlagBits::eAllGraphics				: vk::PipelineStageFlagBits::eNoneKHR;
				PipelineStageFlags |= ((InTransitionState & UndefinedPipelineState) != TransitionState::TRANSITION_UNDEFINED)	? ConvertCommandTypeToPipelineStageFlags(InCommandType)	: vk::PipelineStageFlagBits::eNoneKHR;
				
				return PipelineStageFlags;
			}

			vk::PrimitiveTopology ConvertPrimitiveTopologyToVulkanPrimitiveTopology(_In_ const PrimitiveTopology& InPrimitiveTopology)
			{
				return VULKAN_PRIMITIVE_TOPOLOGIES[static_cast<uint32_t>(InPrimitiveTopology)];
			}

			vk::BufferUsageFlagBits ConvertBufferResourceUsageToVulkanBufferUsageFlags(_In_ const BufferResourceUsage& InBufferResourceUsage)
			{
				static constexpr BufferResourceUsage CopyRead_CopyWrite_ConstantBuffer_Usage						= BufferResourceUsage::BUFFER_RESOURCE_USAGE_COPY_READ
																													| BufferResourceUsage::BUFFER_RESOURCE_USAGE_COPY_WRITE
																													//| BufferResourceUsage::BUFFER_RESOURCE_USAGE_STRUCTURED_BUFFER
																													//| BufferResourceUsage::BUFFER_RESOURCE_USAGE_RW_STRUCTURED_BUFFER
																													| BufferResourceUsage::BUFFER_RESOURCE_USAGE_CONSTANT_BUFFER;

				static constexpr BufferResourceUsage StructuredBuffer_Usage											= BufferResourceUsage::BUFFER_RESOURCE_USAGE_STRUCTURED_BUFFER;

				static constexpr BufferResourceUsage RWStructuredBuffer_Usage										= BufferResourceUsage::BUFFER_RESOURCE_USAGE_RW_STRUCTURED_BUFFER;

				static constexpr BufferResourceUsage Buffer_RWBuffer_IndexBuffer_VertexBuffer_IndirectBuffer_Usage	= BufferResourceUsage::BUFFER_RESOURCE_USAGE_BUFFER
																													| BufferResourceUsage::BUFFER_RESOURCE_USAGE_RW_BUFFER
																													| BufferResourceUsage::BUFFER_RESOURCE_USAGE_INDEX_BUFFER
																													| BufferResourceUsage::BUFFER_RESOURCE_USAGE_VERTEX_BUFFER
																													| BufferResourceUsage::BUFFER_RESOURCE_USAGE_INDIRECT_BUFFER;

				return static_cast<vk::BufferUsageFlagBits>(
					(InBufferResourceUsage & CopyRead_CopyWrite_ConstantBuffer_Usage) |
					((InBufferResourceUsage & StructuredBuffer_Usage) << 3) |
					((InBufferResourceUsage & RWStructuredBuffer_Usage) /* << 2*/) |
					((InBufferResourceUsage & Buffer_RWBuffer_IndexBuffer_VertexBuffer_IndirectBuffer_Usage) >> 1)
				);
			}
			
			vk::ImageUsageFlagBits& operator|=(_Inout_ vk::ImageUsageFlagBits& InOutImageUsageFlags, _In_ const vk::ImageUsageFlagBits& InOtherImageUsageFlags)
			{
				InOutImageUsageFlags = static_cast<vk::ImageUsageFlagBits>(
					static_cast<uint32_t>(InOutImageUsageFlags) | static_cast<uint32_t>(InOtherImageUsageFlags)
				);
				return InOutImageUsageFlags;
			}

			vk::ImageUsageFlagBits ConvertTextureResourceUsageToVulkanImageUsageFlags(_In_ const TextureResourceUsage& InTextureResourceUsage)
			{
				static constexpr vk::ImageUsageFlagBits ImageUsageFlagsNone = static_cast<vk::ImageUsageFlagBits>(0);

				vk::ImageUsageFlagBits Flags = ImageUsageFlagsNone;

				Flags |= (InTextureResourceUsage & TextureResourceUsage::TEXTURE_RESOURCE_USAGE_RENDER_TARGET) != TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE		? vk::ImageUsageFlagBits::eColorAttachment			: ImageUsageFlagsNone;
				Flags |= (InTextureResourceUsage & TextureResourceUsage::TEXTURE_RESOURCE_USAGE_DEPTH_STENCIL) != TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE		? vk::ImageUsageFlagBits::eDepthStencilAttachment	: ImageUsageFlagsNone;
				Flags |= (InTextureResourceUsage & TextureResourceUsage::TEXTURE_RESOURCE_USAGE_UNORDERED_ACCESS) != TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE	? vk::ImageUsageFlagBits::eStorage					: ImageUsageFlagsNone;
				Flags |= (InTextureResourceUsage & TextureResourceUsage::TEXTURE_RESOURCE_USAGE_SHADER_RESOURCE) != TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE	? vk::ImageUsageFlagBits::eSampled					: ImageUsageFlagsNone;
				Flags |= (InTextureResourceUsage & TextureResourceUsage::TEXTURE_RESOURCE_USAGE_COPY_READ) != TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE			? vk::ImageUsageFlagBits::eTransferSrc				: ImageUsageFlagsNone;
				Flags |= (InTextureResourceUsage & TextureResourceUsage::TEXTURE_RESOURCE_USAGE_COPY_WRITE) != TextureResourceUsage::TEXTURE_RESOURCE_USAGE_NONE		? vk::ImageUsageFlagBits::eTransferDst				: ImageUsageFlagsNone;

				return Flags;
			}

			vk::ImageCreateFlagBits& operator|=(_Inout_ vk::ImageCreateFlagBits& InOutImageCreateFlags, _In_ const vk::ImageCreateFlagBits& InOtherImageCreateFlags)
			{
				InOutImageCreateFlags = static_cast<vk::ImageCreateFlagBits>(
					static_cast<uint32_t>(InOutImageCreateFlags) | static_cast<uint32_t>(InOtherImageCreateFlags)
				);
				return InOutImageCreateFlags;
			}

			vk::ImageCreateFlagBits ConvertResourceDimensionToVulkanImageCreateFlags(_In_ const ResourceDimension& InResourceDimension)
			{
				vk::ImageCreateFlagBits ImageCreateFlags = vk::ImageCreateFlagBits::eMutableFormat;

				switch (InResourceDimension)
				{
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
					ImageCreateFlags |= vk::ImageCreateFlagBits::e2DArrayCompatible;
					break;

				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
				case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
					ImageCreateFlags |= vk::ImageCreateFlagBits::eCubeCompatible;
					break;
				}

				return ImageCreateFlags;
			}

			vk::ImageType ConvertResourceDimensionToVulkanImageType(_In_ const ResourceDimension& InResourceDimension)
			{
				ETERNAL_ASSERT(InResourceDimension >= ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D);
				return VULKAN_IMAGE_TYPES[static_cast<int32_t>(InResourceDimension)];
			}

			vk::ShaderStageFlagBits ConvertShaderTypeFlagsToVulkanShaderStageFlags(_In_ const ShaderTypeFlags& InShaderTypeFlags)
			{
				if (InShaderTypeFlags == ShaderTypeFlags::CS)
					return vk::ShaderStageFlagBits::eCompute;

				return static_cast<vk::ShaderStageFlagBits>(InShaderTypeFlags);
			}

			uint32_t ConvertHLSLRegisterTypeToVulkanShaderBindingBase(_In_ const HLSLRegisterType& InHLSLRegisterType)
			{
				return VULKAN_SHADER_REGISTER_OFFSETS[static_cast<int32_t>(InHLSLRegisterType)];
			}

			vk::IndexType ConvertIndexBufferTypeToVulkanIndexType(_In_ const IndexBufferType& InIndexBufferType)
			{
				return static_cast<vk::IndexType>(InIndexBufferType);
			}

			vk::PolygonMode ConvertFillModeToVulkanPolygonMode(_In_ const FillMode& InFillMode)
			{
				return VULKAN_POLYGON_MODES[static_cast<int32_t>(InFillMode)];
			}

			vk::CullModeFlagBits ConvertCullModeToVulkanCullModeFlags(_In_ const CullMode& InCullMode)
			{
				return VULKAN_CULL_MODE_FLAGS[static_cast<int32_t>(InCullMode)];
			}

			vk::FrontFace ConvertFrontFaceToVulkanFrontFace(_In_ const FrontFace& InFrontFace)
			{
				return VULKAN_FRONT_FACES[static_cast<int32_t>(InFrontFace)];
			}
		}
	}
}
