#include "d3d12/D3D12Pipeline.hpp"

#include <d3d12.h>

#include "Graphics/Viewport.hpp"
#include "Graphics/DepthStencil.hpp"
#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12InputLayout.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "d3d12/D3D12RootSignature.hpp"
#include "d3d12/D3D12RenderPass.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12/D3D12Format.hpp"
#include "d3d12/D3D12Utils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		D3D12Pipeline::D3D12Pipeline(
			_In_ Device& InDevice,
			_In_ const PipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InPipelineCreateInformation)
			, _RootSignature(InPipelineCreateInformation.PipelineRootSignature)
		{
			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InDevice);

			const DepthTest& InDepthTest		= InPipelineCreateInformation.PipelineDepthStencil.GetDepthTest();
			const StencilTest& InStencilTest	= InPipelineCreateInformation.PipelineDepthStencil.GetStencilTest();

			D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;

			const vector<D3D12_INPUT_ELEMENT_DESC>& InputElements = static_cast<D3D12InputLayout&>(InPipelineCreateInformation.PipelineInputLayout).GetInputElements();
			PipelineStateDesc.InputLayout.pInputElementDescs	= InputElements.size() ? InputElements.data() : nullptr;
			PipelineStateDesc.InputLayout.NumElements			= static_cast<UINT>(InputElements.size());
			PipelineStateDesc.pRootSignature					= static_cast<D3D12RootSignature&>(_RootSignature).GetD3D12RootSignature();
	
			static_cast<D3D12Shader&>(InPipelineCreateInformation.VS).GetD3D12Shader(PipelineStateDesc.VS);
			static_cast<D3D12Shader&>(InPipelineCreateInformation.PS).GetD3D12Shader(PipelineStateDesc.PS);
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
			PipelineStateDesc.DepthStencilState.DepthFunc						= ConvertComparisonFunctionToD3D12ComparisonFunction(InDepthTest.GetComparisonFunction());
			PipelineStateDesc.DepthStencilState.DepthWriteMask					= D3D12_DEPTH_WRITE_MASK(InDepthTest.GetMask());
			PipelineStateDesc.DepthStencilState.StencilEnable					= InStencilTest.IsEnabled() ? TRUE : FALSE;
			PipelineStateDesc.DepthStencilState.StencilReadMask					= D3D12_DEFAULT_STENCIL_READ_MASK;
			PipelineStateDesc.DepthStencilState.StencilWriteMask				= D3D12_DEFAULT_STENCIL_WRITE_MASK;
			PipelineStateDesc.DepthStencilState.FrontFace.StencilFailOp			= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetFront().Fail);
			PipelineStateDesc.DepthStencilState.FrontFace.StencilDepthFailOp	= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetFront().FailDepth);
			PipelineStateDesc.DepthStencilState.FrontFace.StencilPassOp			= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetFront().Pass);
			PipelineStateDesc.DepthStencilState.FrontFace.StencilFunc			= ConvertComparisonFunctionToD3D12ComparisonFunction(InStencilTest.GetFront().Comparison);
			PipelineStateDesc.DepthStencilState.BackFace.StencilFailOp			= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetBack().Fail);
			PipelineStateDesc.DepthStencilState.BackFace.StencilDepthFailOp		= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetBack().FailDepth);
			PipelineStateDesc.DepthStencilState.BackFace.StencilPassOp			= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetBack().Pass);
			PipelineStateDesc.DepthStencilState.BackFace.StencilFunc			= ConvertComparisonFunctionToD3D12ComparisonFunction(InStencilTest.GetBack().Comparison);

			PipelineStateDesc.BlendState.AlphaToCoverageEnable	= FALSE;
			PipelineStateDesc.BlendState.IndependentBlendEnable	= TRUE;

			const LogicBlend& InLogicBlend = InPipelineCreateInformation.PipelineRenderPass.GetLogicBlend();
			const vector<RenderTargetInformation>& InRenderTargets = InPipelineCreateInformation.PipelineRenderPass.GetRenderTargets();

			uint32_t RenderTargetIndex = 0;
			for (; RenderTargetIndex < InRenderTargets.size(); ++RenderTargetIndex)
			{
				PipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex]	= CreateD3D12RenderTargetBlendDesc(
					InRenderTargets[RenderTargetIndex].RenderTargetBlendState,
					InLogicBlend
				);
				PipelineStateDesc.RTVFormats[RenderTargetIndex]					= ConvertFormatToD3D12Format(InRenderTargets[RenderTargetIndex].RenderTarget->GetViewFormat()).Format;
			}

			// Fill-in unused rendertargets
			for (; RenderTargetIndex < ETERNAL_ARRAYSIZE(PipelineStateDesc.BlendState.RenderTarget); ++RenderTargetIndex)
			{
				PipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex]	= DefaultD3D12RenderTargetBlendDesc;
				PipelineStateDesc.RTVFormats[RenderTargetIndex]					= DXGI_FORMAT_UNKNOWN;
			}

			PipelineStateDesc.NumRenderTargets					= static_cast<UINT>(InRenderTargets.size());

			PipelineStateDesc.IBStripCutValue					= D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
			PipelineStateDesc.PrimitiveTopologyType				= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

			const View* DepthStencilView						= InPipelineCreateInformation.PipelineRenderPass.GetDepthStencilRenderTarget();
			PipelineStateDesc.DSVFormat							= DepthStencilView ? ConvertFormatToD3D12Format(DepthStencilView->GetViewFormat()).Format : DXGI_FORMAT_UNKNOWN;
	
			PipelineStateDesc.SampleDesc.Count					= 1;
			PipelineStateDesc.SampleDesc.Quality				= 0;
			PipelineStateDesc.SampleMask						= UINT_MAX;

			PipelineStateDesc.NodeMask							= InD3DDevice.GetDeviceMask();

			PipelineStateDesc.CachedPSO.pCachedBlob				= nullptr;
			PipelineStateDesc.CachedPSO.CachedBlobSizeInBytes	= 0;

			PipelineStateDesc.Flags								= D3D12_PIPELINE_STATE_FLAG_NONE;

			VerifySuccess(
				InD3DDevice.GetD3D12Device()->CreateGraphicsPipelineState(
					&PipelineStateDesc,
					__uuidof(ID3D12PipelineState),
					(void**)&_PipelineState
				)
			);
		}
	}
}
