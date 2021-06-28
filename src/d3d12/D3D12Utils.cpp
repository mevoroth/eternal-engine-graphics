#include "d3d12/D3D12Utils.hpp"
#include "Graphics/CommandUtils.h"
#include "Graphics/BlendState.hpp"
#include "Graphics/Sampler.hpp"
#include "Graphics/RenderPass.hpp"
#include "Graphics/RootSignature.hpp"
#include "Graphics/Pipeline.hpp"
#include "Math/Math.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static constexpr D3D12_COMMAND_LIST_TYPE D3D12_COMMAND_LIST_TYPES[] =
		{
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			D3D12_COMMAND_LIST_TYPE_COMPUTE,
			D3D12_COMMAND_LIST_TYPE_COPY
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_COMMAND_LIST_TYPES) == static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT), "Mismatch between abstraction and d3d12 command types");

		static constexpr D3D12_LOGIC_OP D3D12_LOGIC_OPERATORS[] =
		{
			D3D12_LOGIC_OP_CLEAR,
			D3D12_LOGIC_OP_SET,
			D3D12_LOGIC_OP_COPY,
			D3D12_LOGIC_OP_COPY_INVERTED,
			D3D12_LOGIC_OP_NOOP,
			D3D12_LOGIC_OP_INVERT,
			D3D12_LOGIC_OP_AND,
			D3D12_LOGIC_OP_NAND,
			D3D12_LOGIC_OP_OR,
			D3D12_LOGIC_OP_NOR,
			D3D12_LOGIC_OP_XOR,
			D3D12_LOGIC_OP_EQUIV,
			D3D12_LOGIC_OP_AND_REVERSE,
			D3D12_LOGIC_OP_AND_INVERTED,
			D3D12_LOGIC_OP_OR_REVERSE,
			D3D12_LOGIC_OP_OR_INVERTED
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_LOGIC_OPERATORS) == static_cast<int32_t>(LogicOperator::LOGIC_OPERATOR_COUNT), "Mismatch between abstraction and d3d12 logic operators");

		static constexpr D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE D3D12_LOAD_OPERATORS[] =
		{
			D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS,
			D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE,
			D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_LOAD_OPERATORS) == static_cast<int32_t>(LoadOperator::COUNT), "Mismatch between abstraction and d3d12 beginning access type");
		
		static constexpr D3D12_RENDER_PASS_ENDING_ACCESS_TYPE D3D12_STORE_OPERATORS[] =
		{
			D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS,
			D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE,
			D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_STORE_OPERATORS) == static_cast<int32_t>(StoreOperator::COUNT), "Mismatch between abstraction and d3d12 ending access type");

		static constexpr D3D12_BLEND_OP D3D12_BLEND_OPERATORS[] =
		{
			D3D12_BLEND_OP_ADD,
			D3D12_BLEND_OP_SUBTRACT,
			D3D12_BLEND_OP_REV_SUBTRACT,
			D3D12_BLEND_OP_MIN,
			D3D12_BLEND_OP_MAX
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_BLEND_OPERATORS) == static_cast<int32_t>(BlendOperator::BLEND_OP_COUNT), "Mismatch between abstraction and d3d12 blend operators");

		static constexpr D3D12_BLEND D3D12_BLENDS[] =
		{
			D3D12_BLEND_ZERO,
			D3D12_BLEND_ONE,

			D3D12_BLEND_SRC_COLOR,
			D3D12_BLEND_INV_SRC_COLOR,
			D3D12_BLEND_SRC_ALPHA,
			D3D12_BLEND_INV_SRC_ALPHA,

			D3D12_BLEND_DEST_ALPHA,
			D3D12_BLEND_INV_DEST_ALPHA,
			D3D12_BLEND_DEST_COLOR,
			D3D12_BLEND_INV_DEST_COLOR,

			D3D12_BLEND_SRC_ALPHA_SAT,

			D3D12_BLEND_BLEND_FACTOR,
			D3D12_BLEND_INV_BLEND_FACTOR,
			D3D12_BLEND_BLEND_FACTOR,
			D3D12_BLEND_INV_BLEND_FACTOR,

			D3D12_BLEND_SRC1_COLOR,
			D3D12_BLEND_INV_SRC1_COLOR,
			D3D12_BLEND_SRC1_ALPHA,
			D3D12_BLEND_INV_SRC1_ALPHA
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_BLENDS) == static_cast<int32_t>(Blend::BLEND_COUNT), "Mismatch between abstraction and d3d12 blends");

		static constexpr D3D12_ROOT_PARAMETER_TYPE D3D12_ROOT_PARAMETER_TYPES[] =
		{
			D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
			D3D12_ROOT_PARAMETER_TYPE_SRV,
			D3D12_ROOT_PARAMETER_TYPE_UAV,
			D3D12_ROOT_PARAMETER_TYPE_SRV,
			D3D12_ROOT_PARAMETER_TYPE_UAV,
			D3D12_ROOT_PARAMETER_TYPE_CBV,
			D3D12_ROOT_PARAMETER_TYPE_SRV,
			D3D12_ROOT_PARAMETER_TYPE_UAV,
			D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_ROOT_PARAMETER_TYPES) == static_cast<int32_t>(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "Mismatch between abstraction and d3d12 root parameter types");

		static constexpr D3D12_DESCRIPTOR_RANGE_TYPE D3D12_DESCRIPTOR_RANGE_TYPES[] =
		{
			D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
			D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
			D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
			D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
			D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
			D3D12_DESCRIPTOR_RANGE_TYPE(~0) // Invalid to have descriptor table within descriptor table
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_DESCRIPTOR_RANGE_TYPES) == static_cast<int32_t>(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "Mismatch between abstraction and d3d121 descriptor ranges types");

		static constexpr D3D12_PRIMITIVE_TOPOLOGY_TYPE D3D12_PRIMITIVE_TOPOLOGY_TYPES[] =
		{
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_PRIMITIVE_TOPOLOGY_TYPES) == static_cast<int32_t>(PrimitiveTopology::PRIMITIVE_TOPOLOGY_COUNT), "Mismatch between abstraction and d3d12 primitive topology types");

		static constexpr D3D12_PRIMITIVE_TOPOLOGY D3D12_PRIMITIVE_TOPOLOGIES[] =
		{
			D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
			D3D_PRIMITIVE_TOPOLOGY_LINELIST,
			D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
			D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
			D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST,
			D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_PRIMITIVE_TOPOLOGIES) == static_cast<int32_t>(PrimitiveTopology::PRIMITIVE_TOPOLOGY_COUNT), "Mismatch between abstraction and d3d12 primitive topologies");

		namespace D3D12
		{
			void VerifySuccess(_In_ const HRESULT& HResult)
			{
				ETERNAL_ASSERT(HResult == S_OK);
			}

			D3D12_RENDER_TARGET_BLEND_DESC CreateD3D12RenderTargetBlendDesc(_In_ const BlendState& InBlendState, _In_ const LogicBlend& InLogicBlend)
			{
				D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc;

				RenderTargetBlendDesc.BlendEnable			= static_cast<BOOL>(InBlendState.IsEnabled());
				RenderTargetBlendDesc.LogicOpEnable			= static_cast<BOOL>(InLogicBlend.IsEnabled());
				RenderTargetBlendDesc.SrcBlend				= ConvertBlendToD3D12Blend(InBlendState.GetSource());
				RenderTargetBlendDesc.DestBlend				= ConvertBlendToD3D12Blend(InBlendState.GetDestination());
				RenderTargetBlendDesc.BlendOp				= ConvertBlendOperatorToD3D12BlendOperator(InBlendState.GetBlendOperator());
				RenderTargetBlendDesc.SrcBlendAlpha			= ConvertBlendToD3D12Blend(InBlendState.GetSourceAlpha());
				RenderTargetBlendDesc.DestBlendAlpha		= ConvertBlendToD3D12Blend(InBlendState.GetDestinationAlpha());
				RenderTargetBlendDesc.BlendOpAlpha			= ConvertBlendOperatorToD3D12BlendOperator(InBlendState.GetBlendAlphaOperator());
				RenderTargetBlendDesc.LogicOp				= ConvertLogicOperatorToD3D12LogicOperator(InLogicBlend.GetLogicOperator());
				RenderTargetBlendDesc.RenderTargetWriteMask	= ConvertBlendChannelToD3D12ColorWriteEnable(InBlendState.GetBlendChannel());

				return RenderTargetBlendDesc;
			}

			D3D12_COMMAND_LIST_TYPE ConvertCommandTypeToD3D12CommandListType(_In_ const CommandType& InCommandType)
			{
				return D3D12_COMMAND_LIST_TYPES[static_cast<int32_t>(InCommandType)];
			}

			D3D12_LOGIC_OP ConvertLogicOperatorToD3D12LogicOperator(_In_ const LogicOperator& InLogicOperator)
			{
				return D3D12_LOGIC_OPERATORS[static_cast<int32_t>(InLogicOperator)];
			}

			D3D12_COMPARISON_FUNC ConvertComparisonFunctionToD3D12ComparisonFunction(_In_ const ComparisonFunction& InComparison)
			{
				return static_cast<D3D12_COMPARISON_FUNC>(static_cast<int32_t>(InComparison) + 1);
			}

			D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(_In_ const LoadOperator& InLoadOperator)
			{
				return D3D12_LOAD_OPERATORS[static_cast<int32_t>(InLoadOperator)];
			}

			D3D12_RENDER_PASS_ENDING_ACCESS_TYPE ConvertStoreOperatorToD3D12RenderPassEndingAccessType(_In_ const StoreOperator& InStoreOperator)
			{
				return D3D12_STORE_OPERATORS[static_cast<int32_t>(InStoreOperator)];
			}

			D3D12_BLEND ConvertBlendToD3D12Blend(_In_ const Blend& InBlend)
			{
				return D3D12_BLENDS[static_cast<int32_t>(InBlend)];
			}

			D3D12_BLEND_OP ConvertBlendOperatorToD3D12BlendOperator(_In_ const BlendOperator& InBlendOperator)
			{
				return D3D12_BLEND_OPERATORS[static_cast<int32_t>(InBlendOperator)];
			}

			D3D12_COLOR_WRITE_ENABLE ConvertBlendChannelToD3D12ColorWriteEnable(_In_ const BlendChannel& InBlendChannel)
			{
				return static_cast<D3D12_COLOR_WRITE_ENABLE>(InBlendChannel);
			}

			D3D12_STENCIL_OP ConvertStencilOperatorToD3D12StencilOperator(_In_ const StencilTest::StencilOperator& InStencilOperator)
			{
				return static_cast<D3D12_STENCIL_OP>(static_cast<int32_t>(InStencilOperator) + 1);
			}

			D3D12_SHADER_VISIBILITY ConvertRootSignatureAccessToD3D12ShaderVisibility(const RootSignatureAccess& InRootSignatureAccess)
			{
				if (InRootSignatureAccess == RootSignatureAccess::ROOT_SIGNATURE_ACCESS_CS)
					return D3D12_SHADER_VISIBILITY_ALL;
				return static_cast<D3D12_SHADER_VISIBILITY>(static_cast<int32_t>(InRootSignatureAccess) + 1);

			}
			D3D12_ROOT_PARAMETER_TYPE ConvertRootSignatureParameterTypeToD3D12RootParameterType(const RootSignatureParameterType& InRootSignatureParameterType)
			{
				return D3D12_ROOT_PARAMETER_TYPES[static_cast<int32_t>(InRootSignatureParameterType)];
			}

			D3D12_DESCRIPTOR_RANGE_TYPE ConvertRootSignatureParameterTypeToD3D12DescriptorRangeType(const RootSignatureParameterType& InRootSignatureParameterType)
			{
				return D3D12_DESCRIPTOR_RANGE_TYPES[static_cast<int32_t>(InRootSignatureParameterType)];
			}

			void ConvertBorderColorToFloats(_In_ const BorderColor& InBorderColor, _Out_ float OutColor[4])
			{
				switch (InBorderColor)
				{
				case BorderColor::BORDER_COLOR_TRANSPARENT_BLACK:
					OutColor[0] = 0.0f;
					OutColor[1] = 0.0f;
					OutColor[2] = 0.0f;
					OutColor[3] = 0.0f;
					return;
				case BorderColor::BORDER_COLOR_OPAQUE_BLACK:
					OutColor[0] = 0.0f;
					OutColor[1] = 0.0f;
					OutColor[2] = 0.0f;
					OutColor[3] = 1.0f;
					return;
				case BorderColor::BORDER_COLOR_OPAQUE_WHITE:
					OutColor[0] = 1.0f;
					OutColor[1] = 1.0f;
					OutColor[2] = 1.0f;
					OutColor[3] = 1.0f;
					return;
				}

				ETERNAL_BREAK();
			}
			D3D12_STATIC_BORDER_COLOR ConvertBorderColorToD3D12StaticBorderColor(_In_ const BorderColor& InBorderColor)
			{
				return static_cast<D3D12_STATIC_BORDER_COLOR>(InBorderColor);
			}

			D3D12_RESOURCE_STATES ConvertTransitionStateToD3D12ResourceStates(const TransitionState& InTransitionState)
			{
				// Stream ouput not supported yet

				using namespace Math;

				static constexpr TransitionState CommonTransitionState			= TransitionState::TRANSITION_CPU_READ
																				| TransitionState::TRANSITION_CPU_WRITE
																				| TransitionState::TRANSITION_PREINITIALIZED
																				| TransitionState::TRANSITION_PRESENT;

				static constexpr TransitionState CPUTransitionState				= TransitionState::TRANSITION_CPU_READ
																				| TransitionState::TRANSITION_CPU_WRITE;

				static constexpr TransitionState VertexAndConstantBufferState	= TransitionState::TRANSITION_VERTEX_BUFFER_READ
																				| TransitionState::TRANSITION_CONSTANT_BUFFER_READ;

				static constexpr TransitionState ShaderReadState				= TransitionState::TRANSITION_NON_PIXEL_SHADER_READ
																				| TransitionState::TRANSITION_PIXEL_SHADER_READ;

				if (InTransitionState == TransitionState::TRANSITION_UNDEFINED
					|| OnlyHasFlags(InTransitionState, CommonTransitionState))
				{
					bool HasOnlyCPUFlags	= (InTransitionState & CPUTransitionState) == InTransitionState;
					bool OnlyOneFlag		= IsPowerOfTwo<uint32_t>(static_cast<uint32_t>(InTransitionState));
					ETERNAL_ASSERT(HasOnlyCPUFlags || OnlyOneFlag); // Only CPU flags
					return D3D12_RESOURCE_STATE_COMMON;
				}

				// Only having non common transition state
				ETERNAL_ASSERT(OnlyHasFlags(InTransitionState, ~CommonTransitionState));

				if ((InTransitionState & TransitionState::TRANSITION_GENERIC_READ) == TransitionState::TRANSITION_GENERIC_READ)
					return D3D12_RESOURCE_STATE_GENERIC_READ;

				D3D12_RESOURCE_STATES States = D3D12_RESOURCE_STATE_COMMON;

				if ((InTransitionState & VertexAndConstantBufferState) != TransitionState::TRANSITION_UNDEFINED)
					States |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

				States |= static_cast<D3D12_RESOURCE_STATES>( InTransitionState & TransitionState::TRANSITION_INDEX_READ);
				States |= static_cast<D3D12_RESOURCE_STATES>((InTransitionState & TransitionState::TRANSITION_INDIRECT)				<< 9);
				States |= static_cast<D3D12_RESOURCE_STATES>((InTransitionState & TransitionState::TRANSITION_COPY_READ)			>> 1);
				States |= static_cast<D3D12_RESOURCE_STATES>((InTransitionState & TransitionState::TRANSITION_COPY_WRITE)			<< 1);
				States |= static_cast<D3D12_RESOURCE_STATES>((InTransitionState & ShaderReadState)									>> 2);
				States |= static_cast<D3D12_RESOURCE_STATES>((InTransitionState & TransitionState::TRANSITION_SHADER_WRITE)			>> 3);
				States |= static_cast<D3D12_RESOURCE_STATES>((InTransitionState & TransitionState::TRANSITION_RENDER_TARGET)		>> 5);
				States |= static_cast<D3D12_RESOURCE_STATES>((InTransitionState & TransitionState::TRANSITION_DEPTH_STENCIL_READ)	>> 3);
				States |= static_cast<D3D12_RESOURCE_STATES>((InTransitionState & TransitionState::TRANSITION_DEPTH_STENCIL_WRITE)	>> 5);
				States |= static_cast<D3D12_RESOURCE_STATES>( InTransitionState & TransitionState::TRANSITION_RESOLVE_DESTINATION);
				States |= static_cast<D3D12_RESOURCE_STATES>( InTransitionState & TransitionState::TRANSITION_RESOLVE_SOURCE);

				return States;
			}

			D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertPrimitiveTopologyToD3D12PrimitiveTopologyType(const PrimitiveTopology& InPrimitiveTopology)
			{
				return D3D12_PRIMITIVE_TOPOLOGY_TYPES[static_cast<int32_t>(InPrimitiveTopology)];
			}

			D3D12_PRIMITIVE_TOPOLOGY ConvertPrimitiveTopologyToD3D12PrimitiveTopology(const PrimitiveTopology& InPrimitiveTopology)
			{
				return D3D12_PRIMITIVE_TOPOLOGIES[static_cast<int32_t>(InPrimitiveTopology)];
			}
		}
	}
}
