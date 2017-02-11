#include "d3d12/D3D12State.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>

#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12InputLayout.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"
#include "d3d12/D3D12BlendState.hpp"

using namespace Eternal::Graphics;

D3D12State::D3D12State(
	_In_ D3D12Device& Device,
	_In_ InputLayout& InputLayoutObj,
	_In_ Shader* VS,
	_In_ Shader* PS,
	_In_ const DepthTest& DepthTestObj,
	_In_ const StencilTest& StencilTestObj,
	_In_ const BlendState BlendStates[],
	_In_ const D3D12RenderTarget RenderTargets[],
	_In_ uint32_t RenderTargetsCount,
	_In_ const D3D12Sampler Samplers[],
	_In_ uint32_t SamplersCount
)
{
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;

	RootSignatureDesc.NumParameters = 0;
	RootSignatureDesc.pParameters = nullptr;
	RootSignatureDesc.NumStaticSamplers = 0;
	RootSignatureDesc.pStaticSamplers = nullptr;
	RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* RootSignatureBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &RootSignatureBlob, &ErrorBlob);
	ETERNAL_ASSERT(hr == S_OK);
	
	hr = Device.GetDevice()->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(), RootSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&_RootSignature);
	ETERNAL_ASSERT(hr == S_OK);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;

	const vector<D3D12_INPUT_ELEMENT_DESC>& InputElements = static_cast<D3D12InputLayout&>(InputLayoutObj).GetInputElements();
	PipelineStateDesc.InputLayout.pInputElementDescs = InputElements.size() ? &InputElements[0] : nullptr;
	PipelineStateDesc.InputLayout.NumElements = (UINT)InputElements.size();
	PipelineStateDesc.pRootSignature = _RootSignature;
	
	static_cast<D3D12Shader*>(VS)->GetD3D12Shader(PipelineStateDesc.VS);
	static_cast<D3D12Shader*>(PS)->GetD3D12Shader(PipelineStateDesc.PS);
	PipelineStateDesc.GS.pShaderBytecode = nullptr;
	PipelineStateDesc.GS.BytecodeLength = 0;
	PipelineStateDesc.DS.pShaderBytecode = nullptr;
	PipelineStateDesc.DS.BytecodeLength = 0;
	PipelineStateDesc.HS.pShaderBytecode = nullptr;
	PipelineStateDesc.HS.BytecodeLength = 0;

	PipelineStateDesc.StreamOutput.pSODeclaration = nullptr;
	PipelineStateDesc.StreamOutput.NumEntries = 0;
	PipelineStateDesc.StreamOutput.pBufferStrides = nullptr;
	PipelineStateDesc.StreamOutput.NumStrides = 0;
	PipelineStateDesc.StreamOutput.RasterizedStream = 0;

	PipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	PipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	PipelineStateDesc.RasterizerState.FrontCounterClockwise = TRUE;
	PipelineStateDesc.RasterizerState.DepthBias = 0;
	PipelineStateDesc.RasterizerState.DepthBiasClamp = 0.0f;
	PipelineStateDesc.RasterizerState.SlopeScaledDepthBias = 0.f;
	PipelineStateDesc.RasterizerState.DepthClipEnable = TRUE;
	PipelineStateDesc.RasterizerState.MultisampleEnable = TRUE;
	PipelineStateDesc.RasterizerState.AntialiasedLineEnable = FALSE;
	PipelineStateDesc.RasterizerState.ForcedSampleCount = 0;
	PipelineStateDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	PipelineStateDesc.DepthStencilState.DepthEnable = DepthTestObj.IsEnabled() ? TRUE : FALSE;
	PipelineStateDesc.DepthStencilState.DepthFunc = (D3D12_COMPARISON_FUNC)DepthTestObj.GetComparisonOperator();
	PipelineStateDesc.DepthStencilState.DepthWriteMask = (D3D12_DEPTH_WRITE_MASK)DepthTestObj.GetMask();
	PipelineStateDesc.DepthStencilState.StencilEnable = StencilTestObj.IsEnabled() ? TRUE : FALSE;
	PipelineStateDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	PipelineStateDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	PipelineStateDesc.DepthStencilState.FrontFace.StencilFailOp = (D3D12_STENCIL_OP)StencilTestObj.GetFront().Fail;
	PipelineStateDesc.DepthStencilState.FrontFace.StencilDepthFailOp = (D3D12_STENCIL_OP)StencilTestObj.GetFront().FailDepth;
	PipelineStateDesc.DepthStencilState.FrontFace.StencilPassOp = (D3D12_STENCIL_OP)StencilTestObj.GetFront().Pass;
	PipelineStateDesc.DepthStencilState.FrontFace.StencilFunc = (D3D12_COMPARISON_FUNC)StencilTestObj.GetFront().ComparisonOp;
	PipelineStateDesc.DepthStencilState.BackFace.StencilFailOp = (D3D12_STENCIL_OP)StencilTestObj.GetBack().Fail;
	PipelineStateDesc.DepthStencilState.BackFace.StencilDepthFailOp = (D3D12_STENCIL_OP)StencilTestObj.GetBack().FailDepth;
	PipelineStateDesc.DepthStencilState.BackFace.StencilPassOp = (D3D12_STENCIL_OP)StencilTestObj.GetBack().Pass;
	PipelineStateDesc.DepthStencilState.BackFace.StencilFunc = (D3D12_COMPARISON_FUNC)StencilTestObj.GetBack().ComparisonOp;

	PipelineStateDesc.BlendState.AlphaToCoverageEnable = FALSE;
	PipelineStateDesc.BlendState.IndependentBlendEnable = FALSE;
	for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < ETERNAL_ARRAYSIZE(PipelineStateDesc.BlendState.RenderTarget); ++RenderTargetIndex)
	{
		D3D12_RENDER_TARGET_BLEND_DESC& CurrentRTBlendDesc = PipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex];
		const D3D12BlendState& CurrentBlendState = static_cast<const D3D12BlendState&>(BlendStates[RenderTargetIndex]);
		CurrentRTBlendDesc.BlendEnable = CurrentBlendState.IsEnabled() ? TRUE : FALSE;

		CurrentRTBlendDesc.LogicOpEnable = FALSE;
		CurrentRTBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
		CurrentRTBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		if (!CurrentBlendState.IsEnabled())
			continue;

		CurrentRTBlendDesc.SrcBlend = CurrentBlendState.GetD3D12Src();
		CurrentRTBlendDesc.DestBlend = CurrentBlendState.GetD3D12Dest();
		CurrentRTBlendDesc.BlendOp = CurrentBlendState.GetD3D12BlendOp();
		CurrentRTBlendDesc.SrcBlendAlpha = CurrentBlendState.GetD3D12SrcAlpha();
		CurrentRTBlendDesc.DestBlendAlpha = CurrentBlendState.GetD3D12DestAlpha();
		CurrentRTBlendDesc.BlendOpAlpha = CurrentBlendState.GetD3D12BlendAlphaOp();
	}

	PipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	PipelineStateDesc.NumRenderTargets = RenderTargetsCount;
	uint32_t RenderTargetIndex = 0;
	for (; RenderTargetIndex < RenderTargetsCount; ++RenderTargetIndex)
	{
		PipelineStateDesc.RTVFormats[RenderTargetIndex] = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	for (; RenderTargetIndex < ETERNAL_ARRAYSIZE(PipelineStateDesc.RTVFormats); ++RenderTargetIndex)
	{
		PipelineStateDesc.RTVFormats[RenderTargetIndex] = DXGI_FORMAT_UNKNOWN;
	}
	PipelineStateDesc.DSVFormat = DXGI_FORMAT_R32_FLOAT;
	
	PipelineStateDesc.SampleDesc.Count = 1;
	PipelineStateDesc.SampleDesc.Quality = 0;
	PipelineStateDesc.SampleMask = UINT_MAX;

	PipelineStateDesc.NodeMask = Device.GetDeviceMask();

	PipelineStateDesc.CachedPSO.pCachedBlob = nullptr;
	PipelineStateDesc.CachedPSO.CachedBlobSizeInBytes = 0;

	PipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	hr = Device.GetDevice()->CreateGraphicsPipelineState(&PipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&_PipelineState);
	ETERNAL_ASSERT(hr == S_OK);
}
