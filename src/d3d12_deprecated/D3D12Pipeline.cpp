#include "d3d12/D3D12Pipeline.hpp"

#include <d3d12.h>

#include "Graphics/Viewport.hpp"
#include "Graphics/DepthStencil.hpp"
#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12_deprecated/D3D12InputLayout.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "d3d12_deprecated/D3D12BlendState.hpp"
#include "d3d12_deprecated/D3D12RootSignature.hpp"
#include "d3d12/D3D12RenderPass.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12/D3D12Format.hpp"
#include "d3d12/D3D12Utils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		
		D3D12Pipeline::D3D12Pipeline(
			Device& InDevice,
			const PipelineCreateInformation& CreateInformation
		)
			: Pipeline(CreateInformation)
			, _RootSignature(CreateInformation.RootSignature)
		{
			D3D12Device& D3D12DeviceObj = static_cast<D3D12Device&>(InDevice);

			const DepthTest& InDepthTest = CreateInformation.DepthStencil.GetDepthTest();
			const StencilTest& InStencilTest = CreateInformation.DepthStencil.GetStencilTest();

			D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;

			const vector<D3D12_INPUT_ELEMENT_DESC>& InputElements	= static_cast<D3D12InputLayout&>(CreateInformation.InputLayout).GetInputElements();
			PipelineStateDesc.InputLayout.pInputElementDescs		= InputElements.size() ? &InputElements[0] : nullptr;
			PipelineStateDesc.InputLayout.NumElements				= (UINT)InputElements.size();
			PipelineStateDesc.pRootSignature						= static_cast<D3D12RootSignature&>(_RootSignature).GetD3D12RootSignature();
	
			static_cast<D3D12Shader&>(CreateInformation.VS).GetD3D12Shader(PipelineStateDesc.VS);
			static_cast<D3D12Shader&>(CreateInformation.PS).GetD3D12Shader(PipelineStateDesc.PS);
			PipelineStateDesc.GS.pShaderBytecode	= nullptr;
			PipelineStateDesc.GS.BytecodeLength		= 0;
			PipelineStateDesc.DS.pShaderBytecode	= nullptr;
			PipelineStateDesc.DS.BytecodeLength		= 0;
			PipelineStateDesc.HS.pShaderBytecode	= nullptr;
			PipelineStateDesc.HS.BytecodeLength		= 0;

			PipelineStateDesc.StreamOutput.pSODeclaration	= nullptr;
			PipelineStateDesc.StreamOutput.NumEntries		= 0;
			PipelineStateDesc.StreamOutput.pBufferStrides	= nullptr;
			PipelineStateDesc.StreamOutput.NumStrides		= 0;
			PipelineStateDesc.StreamOutput.RasterizedStream	= 0;

			PipelineStateDesc.RasterizerState.FillMode				= D3D12_FILL_MODE_SOLID;
			PipelineStateDesc.RasterizerState.CullMode				= D3D12_CULL_MODE_BACK;
			PipelineStateDesc.RasterizerState.FrontCounterClockwise	= FALSE;
			PipelineStateDesc.RasterizerState.DepthBias				= 0;
			PipelineStateDesc.RasterizerState.DepthBiasClamp		= 0.0f;
			PipelineStateDesc.RasterizerState.SlopeScaledDepthBias	= 0.f;
			PipelineStateDesc.RasterizerState.DepthClipEnable		= TRUE;
			PipelineStateDesc.RasterizerState.MultisampleEnable		= TRUE;
			PipelineStateDesc.RasterizerState.AntialiasedLineEnable	= FALSE;
			PipelineStateDesc.RasterizerState.ForcedSampleCount		= 0;
			PipelineStateDesc.RasterizerState.ConservativeRaster	= D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

			PipelineStateDesc.DepthStencilState.DepthEnable						= InDepthTest.IsEnabled() ? TRUE : FALSE;
			PipelineStateDesc.DepthStencilState.DepthFunc						= D3D12_COMPARISON_FUNC(int(InDepthTest.GetComparisonOperator()) + 1);
			PipelineStateDesc.DepthStencilState.DepthWriteMask					= D3D12_DEPTH_WRITE_MASK(InDepthTest.GetMask());
			PipelineStateDesc.DepthStencilState.StencilEnable					= InStencilTest.IsEnabled() ? TRUE : FALSE;
			PipelineStateDesc.DepthStencilState.StencilReadMask					= D3D12_DEFAULT_STENCIL_READ_MASK;
			PipelineStateDesc.DepthStencilState.StencilWriteMask				= D3D12_DEFAULT_STENCIL_WRITE_MASK;
			PipelineStateDesc.DepthStencilState.FrontFace.StencilFailOp			= D3D12_STENCIL_OP(int(InStencilTest.GetFront().Fail) + 1);
			PipelineStateDesc.DepthStencilState.FrontFace.StencilDepthFailOp	= D3D12_STENCIL_OP(int(InStencilTest.GetFront().FailDepth) + 1);
			PipelineStateDesc.DepthStencilState.FrontFace.StencilPassOp			= D3D12_STENCIL_OP(int(InStencilTest.GetFront().Pass) + 1);
			PipelineStateDesc.DepthStencilState.FrontFace.StencilFunc			= D3D12_COMPARISON_FUNC(int(InStencilTest.GetFront().ComparisonOp) + 1);
			PipelineStateDesc.DepthStencilState.BackFace.StencilFailOp			= D3D12_STENCIL_OP(int(InStencilTest.GetBack().Fail) + 1);
			PipelineStateDesc.DepthStencilState.BackFace.StencilDepthFailOp		= D3D12_STENCIL_OP(int(InStencilTest.GetBack().FailDepth) + 1);
			PipelineStateDesc.DepthStencilState.BackFace.StencilPassOp			= D3D12_STENCIL_OP(int(InStencilTest.GetBack().Pass) + 1);
			PipelineStateDesc.DepthStencilState.BackFace.StencilFunc			= D3D12_COMPARISON_FUNC(int(InStencilTest.GetBack().ComparisonOp) + 1);

			PipelineStateDesc.BlendState.AlphaToCoverageEnable = FALSE;
			PipelineStateDesc.BlendState.IndependentBlendEnable = TRUE;

			const LogicBlend& LogicBlend = CreateInformation.RenderPass.GetLogicBlend();

			uint32_t RenderTargetIndex = 0;
			for (; RenderTargetIndex < CreateInformation.RenderPass.GetRenderTargets().size(); ++RenderTargetIndex)
			{
				D3D12_RENDER_TARGET_BLEND_DESC& CurrentRTBlendDesc = PipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex];
				const BlendState& CurrentBlendState = CreateInformation.RenderPass.GetRenderTargets()[RenderTargetIndex].RenderTargetBlendState;

				CurrentRTBlendDesc.BlendEnable				= CurrentBlendState.IsEnabled() ? TRUE : FALSE;

				CurrentRTBlendDesc.LogicOpEnable			= LogicBlend.IsEnabled();
				CurrentRTBlendDesc.LogicOp					= D3D12_LOGIC_OPS[static_cast<int32_t>(LogicBlend.GetLogicOp())];
				CurrentRTBlendDesc.RenderTargetWriteMask	= D3D12_COLOR_WRITE_ENABLE_ALL;

				if (!CurrentBlendState.IsEnabled())
					continue;

				CurrentRTBlendDesc.SrcBlend			= D3D12_BLENDS[int(CurrentBlendState.GetSrc())];
				CurrentRTBlendDesc.DestBlend		= D3D12_BLENDS[int(CurrentBlendState.GetDest())];
				CurrentRTBlendDesc.BlendOp			= D3D12_BLEND_OPS[int(CurrentBlendState.GetBlendOp())];
				CurrentRTBlendDesc.SrcBlendAlpha	= D3D12_BLENDS[int(CurrentBlendState.GetSrcAlpha())];
				CurrentRTBlendDesc.DestBlendAlpha	= D3D12_BLENDS[int(CurrentBlendState.GetDestAlpha())];
				CurrentRTBlendDesc.BlendOpAlpha		= D3D12_BLEND_OPS[int(CurrentBlendState.GetBlendAlphaOp())];
			}

			for (; RenderTargetIndex < ETERNAL_ARRAYSIZE(PipelineStateDesc.BlendState.RenderTarget); ++RenderTargetIndex)
			{
				D3D12_RENDER_TARGET_BLEND_DESC& CurrentRTBlendDesc = PipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex];
				CurrentRTBlendDesc.BlendEnable				= FALSE;
				CurrentRTBlendDesc.LogicOpEnable			= FALSE;
				CurrentRTBlendDesc.LogicOp					= D3D12_LOGIC_OP_NOOP;
				CurrentRTBlendDesc.RenderTargetWriteMask	= D3D12_COLOR_WRITE_ENABLE_NONE;
			}

			PipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

			PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

			D3D12RenderPass& InD3D12RenderPass = static_cast<D3D12RenderPass&>(CreateInformation.RenderPass);
			PipelineStateDesc.NumRenderTargets = UINT(InD3D12RenderPass.GetRenderTargets().size());
			RenderTargetIndex = 0;
			for (; RenderTargetIndex < static_cast<D3D12RenderPass&>(CreateInformation.RenderPass).GetRenderTargets().size(); ++RenderTargetIndex)
			{
				PipelineStateDesc.RTVFormats[RenderTargetIndex] = D3D12_FORMATS[static_cast<int32_t>(InD3D12RenderPass.GetRenderTargets()[RenderTargetIndex].RenderTarget->GetViewFormat())].Format;
			}
			for (; RenderTargetIndex < ETERNAL_ARRAYSIZE(PipelineStateDesc.RTVFormats); ++RenderTargetIndex)
			{
				PipelineStateDesc.RTVFormats[RenderTargetIndex] = DXGI_FORMAT_UNKNOWN;
			}
			PipelineStateDesc.DSVFormat	= InD3D12RenderPass.GetDepthStencilRenderTarget() ? D3D12_FORMATS[static_cast<int32_t>(InD3D12RenderPass.GetDepthStencilRenderTarget()->GetViewFormat())].Format : DXGI_FORMAT_UNKNOWN;
	
			PipelineStateDesc.SampleDesc.Count		= 1;
			PipelineStateDesc.SampleDesc.Quality	= 0;
			PipelineStateDesc.SampleMask			= UINT_MAX;

			PipelineStateDesc.NodeMask				= D3D12DeviceObj.GetDeviceMask();

			PipelineStateDesc.CachedPSO.pCachedBlob				= nullptr;
			PipelineStateDesc.CachedPSO.CachedBlobSizeInBytes	= 0;

			PipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

			D3D12::VerifySuccess(
				D3D12DeviceObj.GetD3D12Device()->CreateGraphicsPipelineState(
					&PipelineStateDesc,
					__uuidof(ID3D12PipelineState),
					(void**)&_PipelineState
				)
			);
		}
	}
}
