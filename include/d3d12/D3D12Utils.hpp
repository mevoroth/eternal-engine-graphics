#pragma once

#include "Graphics/StencilTest.hpp"
#include "Bit/BitField.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Bit;

		class BlendState;
		class LogicBlend;
		class Device;

		enum class TransitionState;
		enum class CommandType;
		enum class LogicOperator;
		enum class ComparisonFunction;
		enum class LoadOperator;
		enum class StoreOperator;
		enum class Blend;
		enum class BlendOperator;
		enum class BlendChannel;
		enum class BorderColor;
		enum class StencilOperator;
		enum class RootSignatureAccess;
		enum class RootSignatureParameterType;
		enum class PrimitiveTopology;
		enum class ResourceMemoryType;
		enum class TextureResourceUsage;
		enum class BufferResourceUsage;
		enum class ResourceDimension;
		enum class IndexBufferType;
		enum class FillMode;
		enum class CullMode;
		enum class FrontFace;

		constexpr D3D12_RENDER_TARGET_BLEND_DESC	DefaultD3D12RenderTargetBlendDesc =
		{
			/*BlendEnable=*/ FALSE,
			/*LogicOpEnable=*/ FALSE,
			D3D12_BLEND_ONE,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE,
			D3D12_BLEND_ZERO,
			D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL
		};
		constexpr D3D12_COLOR_WRITE_ENABLE		D3D12_COLOR_WRITE_ENABLE_NONE = static_cast<D3D12_COLOR_WRITE_ENABLE>(0);

		struct D3D12Handle
		{
			Handle						DescriptorHandle			= Handle::InvalidHandle;
			D3D12_CPU_DESCRIPTOR_HANDLE	D3D12CPUDescriptorHandle	= { 0 };
			D3D12_GPU_DESCRIPTOR_HANDLE	D3D12GPUDescriptorHandle	= { 0 };
		};

		namespace D3D12
		{
			void VerifySuccess(_In_ const HRESULT& InHResult);
			void VerifySuccess(_In_ Device& InDevice, _In_ const HRESULT& InHResult);
			D3D12_RENDER_TARGET_BLEND_DESC CreateD3D12RenderTargetBlendDesc(_In_ const BlendState& InBlendState, _In_ const LogicBlend& InLogicBlend);
			D3D12_COMMAND_LIST_TYPE ConvertCommandTypeToD3D12CommandListType(_In_ const CommandType& InCommandType);
			D3D12_LOGIC_OP ConvertLogicOperatorToD3D12LogicOperator(_In_ const LogicOperator& InLogicOperator);
			D3D12_COMPARISON_FUNC ConvertComparisonFunctionToD3D12ComparisonFunction(_In_ const ComparisonFunction& InComparison);
			D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(_In_ const LoadOperator& InLoadOperator);
			D3D12_RENDER_PASS_ENDING_ACCESS_TYPE ConvertStoreOperatorToD3D12RenderPassEndingAccessType(_In_ const StoreOperator& InStoreOperator);
			D3D12_BLEND ConvertBlendToD3D12Blend(_In_ const Blend& InBlend);
			D3D12_BLEND_OP ConvertBlendOperatorToD3D12BlendOperator(_In_ const BlendOperator& InBlendOperator);
			D3D12_COLOR_WRITE_ENABLE ConvertBlendChannelToD3D12ColorWriteEnable(_In_ const BlendChannel& InBlendChannel);
			D3D12_STENCIL_OP ConvertStencilOperatorToD3D12StencilOperator(_In_ const StencilTest::StencilOperator& InStencilOperator);
			D3D12_SHADER_VISIBILITY ConvertRootSignatureAccessToD3D12ShaderVisibility(_In_ const RootSignatureAccess& InRootSignatureAccess);
			D3D12_ROOT_PARAMETER_TYPE ConvertRootSignatureParameterTypeToD3D12RootParameterType(_In_ const RootSignatureParameterType& InRootSignatureParameterType);
			D3D12_DESCRIPTOR_RANGE_TYPE ConvertRootSignatureParameterTypeToD3D12DescriptorRangeType(_In_ const RootSignatureParameterType& InRootSignatureParameterType);
			void ConvertBorderColorToFloats(_In_ const BorderColor& InBorderColor, _Out_ float OutColor[4]);
			D3D12_STATIC_BORDER_COLOR ConvertBorderColorToD3D12StaticBorderColor(_In_ const BorderColor& InBorderColor);
			D3D12_RESOURCE_STATES ConvertTransitionStateToD3D12ResourceStates(_In_ const TransitionState& InTransitionState);
			D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertPrimitiveTopologyToD3D12PrimitiveTopologyType(_In_ const PrimitiveTopology& InPrimitiveTopology);
			D3D12_PRIMITIVE_TOPOLOGY ConvertPrimitiveTopologyToD3D12PrimitiveTopology(_In_ const PrimitiveTopology& InPrimitiveTopology);
			D3D12_HEAP_TYPE ConvertResourceMemoryTypeToD3D12HeapType(_In_ const ResourceMemoryType& InResourceMemoryType);
			D3D12_CPU_PAGE_PROPERTY ConvertResourceMemoryTypeToD3D12CPUPageProperty(_In_ const ResourceMemoryType& InResourceMemoryType);
			D3D12_RESOURCE_DIMENSION ConvertResourceDimensionToD3D12ResourceDimension(_In_ const ResourceDimension& InResourceDimension);
			D3D12_RESOURCE_FLAGS ConvertTextureResourceUsageToD3D12ResourceFlags(_In_ const TextureResourceUsage& InResourceFlags);
			D3D12_RESOURCE_FLAGS ConvertBufferResourceUsageToD3D12ResourceFlags(_In_ const BufferResourceUsage& InResourceFlags);
			uint32_t ConvertIndexBufferTypeToD3D12Stride(_In_ const IndexBufferType& InIndexBufferType);
			DXGI_FORMAT ConvertIndexBufferTypeToDXGIFormat(_In_ const IndexBufferType& InIndexBufferType);
			D3D12_FILL_MODE ConvertFillModeToD3D12FillMode(_In_ const FillMode& InFillMode);
			D3D12_CULL_MODE ConvertCullModeToD3D12CullMode(_In_ const CullMode& InCullMode);
			BOOL ConvertFrontFaceToD3D12FrontFace(_In_ const FrontFace& InFrontFace);
		}
	}
}
