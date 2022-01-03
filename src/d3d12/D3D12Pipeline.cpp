#include "d3d12/D3D12Pipeline.hpp"

#include "d3dx12.h"
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
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		template<typename PipelineStateDescriptionType>
		static void InitializePipelineStateDescription(_In_ const D3D12Device& InD3D12Device, _In_ const PipelineCreateInformation& InPipelineCreateInformation, _Inout_ PipelineStateDescriptionType& InOutPipelineStateDesc)
		{
			const DepthTest& InDepthTest											= InPipelineCreateInformation.PipelineDepthStencil.GetDepthTest();
			const StencilTest& InStencilTest										= InPipelineCreateInformation.PipelineDepthStencil.GetStencilTest();

			InOutPipelineStateDesc.pRootSignature									= static_cast<const D3D12RootSignature&>(InPipelineCreateInformation.PipelineRootSignature).GetD3D12RootSignature();

			static_cast<D3D12Shader*>(InPipelineCreateInformation.PS)->GetD3D12Shader(InOutPipelineStateDesc.PS);

			const Rasterizer& InRasterizer = InPipelineCreateInformation.PipelineRasterizer;

			InOutPipelineStateDesc.RasterizerState.FillMode							= ConvertFillModeToD3D12FillMode(InRasterizer.GetFillMode());
			InOutPipelineStateDesc.RasterizerState.CullMode							= ConvertCullModeToD3D12CullMode(InRasterizer.GetCullMode());
			InOutPipelineStateDesc.RasterizerState.FrontCounterClockwise			= ConvertFrontFaceToD3D12FrontFace(InRasterizer.GetFrontFace());
			InOutPipelineStateDesc.RasterizerState.DepthBias						= InRasterizer.GetDepthBias();
			InOutPipelineStateDesc.RasterizerState.DepthBiasClamp					= InRasterizer.GetDepthBiasClamp();
			InOutPipelineStateDesc.RasterizerState.SlopeScaledDepthBias				= InRasterizer.GetDepthBiasSlopeScale();
			InOutPipelineStateDesc.RasterizerState.DepthClipEnable					= static_cast<BOOL>(InRasterizer.GetDepthClip());
			InOutPipelineStateDesc.RasterizerState.MultisampleEnable				= TRUE;
			InOutPipelineStateDesc.RasterizerState.AntialiasedLineEnable			= TRUE;
			InOutPipelineStateDesc.RasterizerState.ForcedSampleCount				= 0;
			InOutPipelineStateDesc.RasterizerState.ConservativeRaster				= D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON;

			InOutPipelineStateDesc.DepthStencilState.DepthEnable					= InDepthTest.IsEnabled() ? TRUE : FALSE;
			InOutPipelineStateDesc.DepthStencilState.DepthFunc						= ConvertComparisonFunctionToD3D12ComparisonFunction(InDepthTest.GetComparisonFunction());
			InOutPipelineStateDesc.DepthStencilState.DepthWriteMask					= D3D12_DEPTH_WRITE_MASK(InDepthTest.GetMask());
			InOutPipelineStateDesc.DepthStencilState.StencilEnable					= InStencilTest.IsEnabled() ? TRUE : FALSE;
			InOutPipelineStateDesc.DepthStencilState.StencilReadMask				= D3D12_DEFAULT_STENCIL_READ_MASK;
			InOutPipelineStateDesc.DepthStencilState.StencilWriteMask				= D3D12_DEFAULT_STENCIL_WRITE_MASK;
			InOutPipelineStateDesc.DepthStencilState.FrontFace.StencilFailOp		= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetFront().Fail);
			InOutPipelineStateDesc.DepthStencilState.FrontFace.StencilDepthFailOp	= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetFront().FailDepth);
			InOutPipelineStateDesc.DepthStencilState.FrontFace.StencilPassOp		= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetFront().Pass);
			InOutPipelineStateDesc.DepthStencilState.FrontFace.StencilFunc			= ConvertComparisonFunctionToD3D12ComparisonFunction(InStencilTest.GetFront().Comparison);
			InOutPipelineStateDesc.DepthStencilState.BackFace.StencilFailOp			= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetBack().Fail);
			InOutPipelineStateDesc.DepthStencilState.BackFace.StencilDepthFailOp	= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetBack().FailDepth);
			InOutPipelineStateDesc.DepthStencilState.BackFace.StencilPassOp			= ConvertStencilOperatorToD3D12StencilOperator(InStencilTest.GetBack().Pass);
			InOutPipelineStateDesc.DepthStencilState.BackFace.StencilFunc			= ConvertComparisonFunctionToD3D12ComparisonFunction(InStencilTest.GetBack().Comparison);

			InOutPipelineStateDesc.BlendState.AlphaToCoverageEnable					= FALSE;
			InOutPipelineStateDesc.BlendState.IndependentBlendEnable				= TRUE;

			const LogicBlend& InLogicBlend = InPipelineCreateInformation.PipelineRenderPass->GetLogicBlend();
			const vector<RenderTargetInformation>& InRenderTargets = InPipelineCreateInformation.PipelineRenderPass->GetRenderTargets();

			uint32_t RenderTargetIndex = 0;
			for (; RenderTargetIndex < InRenderTargets.size(); ++RenderTargetIndex)
			{
				InOutPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex]	= CreateD3D12RenderTargetBlendDesc(
					InRenderTargets[RenderTargetIndex].RenderTargetBlendState,
					InLogicBlend
				);
				InOutPipelineStateDesc.RTVFormats[RenderTargetIndex]				= ConvertFormatToD3D12Format(InRenderTargets[RenderTargetIndex].RenderTarget->GetViewFormat()).Format;
			}

			// Fill-in unused rendertargets
			for (; RenderTargetIndex < ETERNAL_ARRAYSIZE(InOutPipelineStateDesc.BlendState.RenderTarget); ++RenderTargetIndex)
			{
				InOutPipelineStateDesc.BlendState.RenderTarget[RenderTargetIndex]	= DefaultD3D12RenderTargetBlendDesc;
				InOutPipelineStateDesc.RTVFormats[RenderTargetIndex]				= DXGI_FORMAT_UNKNOWN;
			}

			InOutPipelineStateDesc.NumRenderTargets									= static_cast<UINT>(InRenderTargets.size());
			
			const View* DepthStencilView											= InPipelineCreateInformation.PipelineRenderPass->GetDepthStencilRenderTarget();
			InOutPipelineStateDesc.DSVFormat										= DepthStencilView ? ConvertFormatToD3D12Format(DepthStencilView->GetViewFormat()).Format : DXGI_FORMAT_UNKNOWN;
	
			InOutPipelineStateDesc.SampleDesc.Count									= 1;
			InOutPipelineStateDesc.SampleDesc.Quality								= 0;
			InOutPipelineStateDesc.SampleMask										= UINT_MAX;

			InOutPipelineStateDesc.NodeMask											= InD3D12Device.GetDeviceMask();

			InOutPipelineStateDesc.CachedPSO.pCachedBlob							= nullptr;
			InOutPipelineStateDesc.CachedPSO.CachedBlobSizeInBytes					= 0;

			InOutPipelineStateDesc.Flags											= D3D12_PIPELINE_STATE_FLAG_NONE;

		}
		
		D3D12Pipeline::D3D12Pipeline(
			_In_ Device& InDevice,
			_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InPipelineCreateInformation)
			, _PrimitiveTopology(ConvertPrimitiveTopologyToD3D12PrimitiveTopology(InPipelineCreateInformation.PipelinePrimitiveTopology))
		{
			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InDevice);

			D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc	= {};

			InitializePipelineStateDescription(InD3DDevice, InPipelineCreateInformation, PipelineStateDesc);

			const vector<D3D12_INPUT_ELEMENT_DESC>& InputElements	= static_cast<D3D12InputLayout*>(InPipelineCreateInformation.PipelineInputLayout)->GetD3D12InputElements();
			PipelineStateDesc.InputLayout.pInputElementDescs		= InputElements.size() ? InputElements.data() : nullptr;
			PipelineStateDesc.InputLayout.NumElements				= static_cast<UINT>(InputElements.size());
	
			static_cast<D3D12Shader*>(InPipelineCreateInformation.VS)->GetD3D12Shader(PipelineStateDesc.VS);
			PipelineStateDesc.GS.pShaderBytecode					= nullptr;
			PipelineStateDesc.GS.BytecodeLength						= 0;
			PipelineStateDesc.DS.pShaderBytecode					= nullptr;
			PipelineStateDesc.DS.BytecodeLength						= 0;
			PipelineStateDesc.HS.pShaderBytecode					= nullptr;
			PipelineStateDesc.HS.BytecodeLength						= 0;

			PipelineStateDesc.StreamOutput.pSODeclaration			= nullptr;
			PipelineStateDesc.StreamOutput.NumEntries				= 0;
			PipelineStateDesc.StreamOutput.pBufferStrides			= nullptr;
			PipelineStateDesc.StreamOutput.NumStrides				= 0;
			PipelineStateDesc.StreamOutput.RasterizedStream			= 0;

			PipelineStateDesc.IBStripCutValue						= D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
			PipelineStateDesc.PrimitiveTopologyType					= ConvertPrimitiveTopologyToD3D12PrimitiveTopologyType(InPipelineCreateInformation.PipelinePrimitiveTopology);

			VerifySuccess(
				InD3DDevice.GetD3D12Device()->CreateGraphicsPipelineState(
					&PipelineStateDesc,
					__uuidof(ID3D12PipelineState),
					reinterpret_cast<void**>(&_PipelineState)
				)
			);

			std::string PipelineStateName = "VS_" + InPipelineCreateInformation.VS->GetName() + " PS_" + InPipelineCreateInformation.PS->GetName();
			std::wstring UTF8PipelineStateName(PipelineStateName.begin(), PipelineStateName.end());
			VerifySuccess(
				_PipelineState->SetName(UTF8PipelineStateName.c_str())
			);
		}

		D3D12Pipeline::D3D12Pipeline(
			_In_ Device& InDevice,
			_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InPipelineCreateInformation)
		{
			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InDevice);
			
			D3D12_COMPUTE_PIPELINE_STATE_DESC PipelineStateDesc = {};
			
			PipelineStateDesc.pRootSignature	= static_cast<const D3D12RootSignature&>(InPipelineCreateInformation.PipelineRootSignature).GetD3D12RootSignature();
			static_cast<D3D12Shader*>(InPipelineCreateInformation.CS)->GetD3D12Shader(PipelineStateDesc.CS);
			PipelineStateDesc.NodeMask			= InD3DDevice.GetDeviceMask();

			VerifySuccess(
				InD3DDevice.GetD3D12Device()->CreateComputePipelineState(
					&PipelineStateDesc,
					__uuidof(ID3D12PipelineState),
					reinterpret_cast<void**>(&_PipelineState)
				)
			);

			std::string PipelineStateName = "CS_" + InPipelineCreateInformation.CS->GetName();
			std::wstring UTF8PipelineStateName(PipelineStateName.begin(), PipelineStateName.end());
			VerifySuccess(
				_PipelineState->SetName(UTF8PipelineStateName.c_str())
			);
			_PipelineState->SetName(UTF8PipelineStateName.c_str());
		}

		D3D12Pipeline::D3D12Pipeline(
			_In_ Device& InDevice,
			_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InPipelineCreateInformation)
		{
			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InDevice);

			D3DX12_MESH_SHADER_PIPELINE_STATE_DESC PipelineStateDesc = {};

			InitializePipelineStateDescription(InD3DDevice, InPipelineCreateInformation, PipelineStateDesc);

			static_cast<D3D12Shader*>(InPipelineCreateInformation.MS)->GetD3D12Shader(PipelineStateDesc.MS);
			if (InPipelineCreateInformation.AS)
				static_cast<D3D12Shader*>(InPipelineCreateInformation.AS)->GetD3D12Shader(PipelineStateDesc.AS);
			static_cast<D3D12Shader*>(InPipelineCreateInformation.PS)->GetD3D12Shader(PipelineStateDesc.PS);

			CD3DX12_PIPELINE_MESH_STATE_STREAM PipelineMeshStateStream(PipelineStateDesc);

			D3D12_PIPELINE_STATE_STREAM_DESC PipelineStateStreamDesc;
			PipelineStateStreamDesc.pPipelineStateSubobjectStream	= &PipelineMeshStateStream;
			PipelineStateStreamDesc.SizeInBytes						= sizeof(PipelineMeshStateStream);

			VerifySuccess(
				InD3DDevice.GetD3D12Device5()->CreatePipelineState(
					&PipelineStateStreamDesc,
					__uuidof(ID3D12PipelineState),
					reinterpret_cast<void**>(&_PipelineState)
				)
			);
		}

		D3D12Pipeline::~D3D12Pipeline()
		{
			_PipelineState->Release();
			_PipelineState = nullptr;
		}

		const D3D12RootSignature& D3D12Pipeline::GetD3D12RootSignature() const
		{
			return static_cast<const D3D12RootSignature&>(GetRootSignature());
		}
	}
}
