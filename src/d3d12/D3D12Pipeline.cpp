#include "d3d12/D3D12Pipeline.hpp"

#include "d3dx12.h"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/DepthStencil.hpp"
#include "Graphics/DepthTest.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "Graphics/StencilTest.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12InputLayout.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "d3d12/D3D12RootSignature.hpp"
#include "d3d12/D3D12RenderPass.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12/D3D12Format.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "Math/Math.hpp"
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;
		
		namespace GraphicsPrivate
		{
			struct D3D12_STATE_SUBOBJECT_STATE_OBJECT_CONFIG : public D3D12_STATE_SUBOBJECT
			{
				D3D12_STATE_SUBOBJECT_STATE_OBJECT_CONFIG(_In_ D3D12_STATE_OBJECT_CONFIG& InStateObjectConfig)
				{
					Type	= D3D12_STATE_SUBOBJECT_TYPE_STATE_OBJECT_CONFIG;
					pDesc	= &InStateObjectConfig;
				}
			};

			struct D3D12_STATE_SUBOBJECT_RAYTRACING_PIPELINE_CONFIG1 : public D3D12_STATE_SUBOBJECT
			{
				D3D12_STATE_SUBOBJECT_RAYTRACING_PIPELINE_CONFIG1(_In_ D3D12_RAYTRACING_PIPELINE_CONFIG1& InRayTracingPipelineConfig1)
				{
					Type	= D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG1;
					pDesc	= &InRayTracingPipelineConfig1;
				}
			};

			struct D3D12_STATE_SUBOBJECT_RAYTRACING_SHADER_CONFIG : public D3D12_STATE_SUBOBJECT
			{
				D3D12_STATE_SUBOBJECT_RAYTRACING_SHADER_CONFIG(_In_ D3D12_RAYTRACING_SHADER_CONFIG& InRayTracingShaderConfig)
				{
					Type	= D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
					pDesc	= &InRayTracingShaderConfig;
				}
			};

			struct D3D12_STATE_SUBOBJECT_GLOBAL_ROOT_SIGNATURE : public D3D12_STATE_SUBOBJECT
			{
				D3D12_STATE_SUBOBJECT_GLOBAL_ROOT_SIGNATURE(_In_ D3D12_GLOBAL_ROOT_SIGNATURE& InGlobalRootSignature)
				{
					Type	= D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
					pDesc	= &InGlobalRootSignature;
				}
			};

			struct D3D12_STATE_SUBOBJECT_DXIL_LIBRARY_DESC : public D3D12_STATE_SUBOBJECT
			{
				D3D12_STATE_SUBOBJECT_DXIL_LIBRARY_DESC(_In_ D3D12_DXIL_LIBRARY_DESC& InDXILLibraryDescription)
				{
					Type	= D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
					pDesc	= &InDXILLibraryDescription;
				}
			};
			
			struct D3D12_STATE_SUBOBJECT_HIT_GROUP_DESC : public D3D12_STATE_SUBOBJECT
			{
				D3D12_STATE_SUBOBJECT_HIT_GROUP_DESC(_In_ D3D12_HIT_GROUP_DESC& HitGroupDescription)
				{
					Type	= D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
					pDesc	= &HitGroupDescription;
				}
			};

			static constexpr uint32_t RayTracingPipelineStateSubObjectsMaxCount	= 16;
			static constexpr uint32_t RayTracingPipelineDXILLibrariesMaxCount	= 4;
		}

		template<typename PipelineStateDescriptionType>
		static bool InitializePipelineStateDescription(_In_ const D3D12Device& InD3D12Device, _In_ const PipelineCreateInformation& InPipelineCreateInformation, _Inout_ PipelineStateDescriptionType& InOutPipelineStateDesc)
		{
			if (!InPipelineCreateInformation.ShaderPixel || !InPipelineCreateInformation.ShaderPixel->IsShaderCompiled())
				return false;
			
			const DepthTest& InDepthTest											= InPipelineCreateInformation.PipelineDepthStencil.GetDepthTest();
			const StencilTest& InStencilTest										= InPipelineCreateInformation.PipelineDepthStencil.GetStencilTest();

			InOutPipelineStateDesc.pRootSignature									= static_cast<const D3D12RootSignature&>(InPipelineCreateInformation.PipelineRootSignature).GetD3D12RootSignature();

			static_cast<D3D12Shader*>(InPipelineCreateInformation.ShaderPixel)->GetD3D12Shader(InOutPipelineStateDesc.PS);

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
			InOutPipelineStateDesc.RasterizerState.ConservativeRaster				= InRasterizer.GetConservativeRasterization() ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

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

			return true;
		}
		
		static void ConditionalGetD3D12Shader(_In_ Shader* InShader, _Out_ D3D12_SHADER_BYTECODE& OutShaderByteCode)
		{
			OutShaderByteCode = {};
			if (InShader)
				static_cast<D3D12Shader*>(InShader)->GetD3D12Shader(OutShaderByteCode);
		}

		D3D12Pipeline::D3D12Pipeline(
			_Inout_ GraphicsContext& InOutContext,
			_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InOutContext, InPipelineCreateInformation)
			, _PrimitiveTopology(ConvertPrimitiveTopologyToD3D12PrimitiveTopology(InPipelineCreateInformation.PipelinePrimitiveTopology))
		{
			if (!InPipelineCreateInformation.ShaderVertex || !InPipelineCreateInformation.ShaderVertex->IsShaderCompiled())
				return;

			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InOutContext.GetDevice());

			D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDescription	= {};

			if (!InitializePipelineStateDescription(InD3DDevice, InPipelineCreateInformation, PipelineStateDescription))
				return;

			const vector<D3D12_INPUT_ELEMENT_DESC>& InputElements		= static_cast<D3D12InputLayout*>(InPipelineCreateInformation.PipelineInputLayout)->GetD3D12InputElements();
			PipelineStateDescription.InputLayout.pInputElementDescs		= InputElements.size() ? InputElements.data() : nullptr;
			PipelineStateDescription.InputLayout.NumElements			= static_cast<UINT>(InputElements.size());
	
			static_cast<D3D12Shader*>(InPipelineCreateInformation.ShaderVertex)->GetD3D12Shader(PipelineStateDescription.VS);
			ConditionalGetD3D12Shader(InPipelineCreateInformation.ShaderGeometry,	PipelineStateDescription.GS);
			ConditionalGetD3D12Shader(InPipelineCreateInformation.ShaderDomain,		PipelineStateDescription.DS);
			ConditionalGetD3D12Shader(InPipelineCreateInformation.ShaderHull,		PipelineStateDescription.HS);

			PipelineStateDescription.StreamOutput.pSODeclaration		= nullptr;
			PipelineStateDescription.StreamOutput.NumEntries			= 0;
			PipelineStateDescription.StreamOutput.pBufferStrides		= nullptr;
			PipelineStateDescription.StreamOutput.NumStrides			= 0;
			PipelineStateDescription.StreamOutput.RasterizedStream		= 0;

			PipelineStateDescription.IBStripCutValue					= D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
			PipelineStateDescription.PrimitiveTopologyType				= ConvertPrimitiveTopologyToD3D12PrimitiveTopologyType(InPipelineCreateInformation.PipelinePrimitiveTopology);

			VerifySuccess(
				InD3DDevice.GetD3D12Device()->CreateGraphicsPipelineState(
					&PipelineStateDescription,
					__uuidof(ID3D12PipelineState),
					reinterpret_cast<void**>(&_Pipeline.PipelineState)
				)
			);

			std::string PipelineStateName = "Vertex_" + string(InPipelineCreateInformation.ShaderVertex->GetName()) + (InPipelineCreateInformation.ShaderPixel ? " Pixel_" + string(InPipelineCreateInformation.ShaderPixel->GetName()) : "_Only");
			std::wstring UTF8PipelineStateName(PipelineStateName.begin(), PipelineStateName.end());
			VerifySuccess(
				_Pipeline.PipelineState->SetName(UTF8PipelineStateName.c_str())
			);
		}

		D3D12Pipeline::D3D12Pipeline(
			_Inout_ GraphicsContext& InOutContext,
			_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InOutContext, InPipelineCreateInformation)
		{
			if (!InPipelineCreateInformation.ShaderCompute || !InPipelineCreateInformation.ShaderCompute->IsShaderCompiled())
				return;

			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InOutContext.GetDevice());
			
			D3D12_COMPUTE_PIPELINE_STATE_DESC PipelineStateDesc = {};
			
			PipelineStateDesc.pRootSignature	= static_cast<const D3D12RootSignature&>(InPipelineCreateInformation.PipelineRootSignature).GetD3D12RootSignature();
			static_cast<D3D12Shader*>(InPipelineCreateInformation.ShaderCompute)->GetD3D12Shader(PipelineStateDesc.CS);
			PipelineStateDesc.NodeMask			= InD3DDevice.GetDeviceMask();

			VerifySuccess(
				InD3DDevice.GetD3D12Device()->CreateComputePipelineState(
					&PipelineStateDesc,
					__uuidof(ID3D12PipelineState),
					reinterpret_cast<void**>(&_Pipeline.PipelineState)
				)
			);

			std::string PipelineStateName = "Compute_" + string(InPipelineCreateInformation.ShaderCompute->GetName());
			std::wstring UTF8PipelineStateName(PipelineStateName.begin(), PipelineStateName.end());
			VerifySuccess(
				_Pipeline.PipelineState->SetName(UTF8PipelineStateName.c_str())
			);
			_Pipeline.PipelineState->SetName(UTF8PipelineStateName.c_str());
		}

		D3D12Pipeline::D3D12Pipeline(
			_Inout_ GraphicsContext& InOutContext,
			_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InOutContext, InPipelineCreateInformation)
		{
			ETERNAL_BREAK();

			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InOutContext.GetDevice());

			D3DX12_MESH_SHADER_PIPELINE_STATE_DESC PipelineStateDesc = {};

			if (!InitializePipelineStateDescription(InD3DDevice, InPipelineCreateInformation, PipelineStateDesc))
				return;

			static_cast<D3D12Shader*>(InPipelineCreateInformation.ShaderMesh)->GetD3D12Shader(PipelineStateDesc.MS);
			if (InPipelineCreateInformation.ShaderAmplification)
				static_cast<D3D12Shader*>(InPipelineCreateInformation.ShaderAmplification)->GetD3D12Shader(PipelineStateDesc.AS);
			static_cast<D3D12Shader*>(InPipelineCreateInformation.ShaderPixel)->GetD3D12Shader(PipelineStateDesc.PS);

			CD3DX12_PIPELINE_MESH_STATE_STREAM PipelineMeshStateStream(PipelineStateDesc);

			D3D12_PIPELINE_STATE_STREAM_DESC PipelineStateStreamDesc;
			PipelineStateStreamDesc.pPipelineStateSubobjectStream	= &PipelineMeshStateStream;
			PipelineStateStreamDesc.SizeInBytes						= sizeof(PipelineMeshStateStream);

			VerifySuccess(
				InD3DDevice.GetD3D12Device5()->CreatePipelineState(
					&PipelineStateStreamDesc,
					__uuidof(ID3D12PipelineState),
					reinterpret_cast<void**>(&_Pipeline.PipelineState)
				)
			);
		}

		D3D12Pipeline::D3D12Pipeline(
			_Inout_ GraphicsContext& InOutContext,
			_In_ const RayTracingPipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InOutContext, InPipelineCreateInformation)
		{
			using namespace Eternal::Graphics::GraphicsPrivate;

			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InOutContext.GetDevice());

			ID3D12Device5* InD3D12Device5 = InD3DDevice.GetD3D12Device5();

			D3D12_STATE_OBJECT_CONFIG StateObjectConfig =
			{
				D3D12_STATE_OBJECT_FLAG_ALLOW_LOCAL_DEPENDENCIES_ON_EXTERNAL_DEFINITIONS |
				D3D12_STATE_OBJECT_FLAG_ALLOW_EXTERNAL_DEPENDENCIES_ON_LOCAL_DEFINITIONS |
				D3D12_STATE_OBJECT_FLAG_ALLOW_STATE_OBJECT_ADDITIONS
			};

			D3D12_RAYTRACING_PIPELINE_CONFIG1 RayTracingPipelineConfig;
			RayTracingPipelineConfig.MaxTraceRecursionDepth	= 1;
			RayTracingPipelineConfig.Flags					= D3D12_RAYTRACING_PIPELINE_FLAG_NONE;

			D3D12_RAYTRACING_SHADER_CONFIG RayTracingShaderConfig;
			RayTracingShaderConfig.MaxPayloadSizeInBytes	= 16;
			RayTracingShaderConfig.MaxAttributeSizeInBytes	= 8;

			D3D12_GLOBAL_ROOT_SIGNATURE GlobalRootSignature;
			GlobalRootSignature.pGlobalRootSignature		= static_cast<D3D12RootSignature&>(InPipelineCreateInformation.PipelineRootSignature).GetD3D12RootSignature();

			uint32_t DXILLibrariesCount = 0u;
			D3D12_DXIL_LIBRARY_DESC* DXILLibraryDescriptions	= reinterpret_cast<D3D12_DXIL_LIBRARY_DESC*>(alloca(sizeof(D3D12_DXIL_LIBRARY_DESC) * RayTracingPipelineDXILLibrariesMaxCount));
			D3D12_EXPORT_DESC* ExportDescriptions				= reinterpret_cast<D3D12_EXPORT_DESC*>(alloca(sizeof(D3D12_EXPORT_DESC) * RayTracingPipelineDXILLibrariesMaxCount));

			auto AddDXILLibrary = [DXILLibraryDescriptions, ExportDescriptions, &DXILLibrariesCount](_In_ Shader* InShader) -> void
			{
				if (!InShader)
					return;

				D3D12_EXPORT_DESC& ExportDescription	= ExportDescriptions[DXILLibrariesCount];
				ExportDescription						= {};
				ExportDescription.Name					= static_cast<D3D12Shader*>(InShader)->GetD3D12StageEntryPoint();

				D3D12_DXIL_LIBRARY_DESC& DXILLibraryDescription	= DXILLibraryDescriptions[DXILLibrariesCount++];
				DXILLibraryDescription							= {};
				static_cast<D3D12Shader*>(InShader)->GetD3D12Shader(DXILLibraryDescription.DXILLibrary);
				DXILLibraryDescription.NumExports				= 1;
				DXILLibraryDescription.pExports					= &ExportDescription;
			};

			AddDXILLibrary(InPipelineCreateInformation.ShaderRayTracingRayGeneration);
			AddDXILLibrary(InPipelineCreateInformation.ShaderRayTracingMiss);
			AddDXILLibrary(InPipelineCreateInformation.ShaderRayTracingClosestHit);
			AddDXILLibrary(InPipelineCreateInformation.ShaderRayTracingAnyHit);

			D3D12_HIT_GROUP_DESC HitGroupDescription		= {};
			HitGroupDescription.HitGroupExport				= L"HitGroup";
			HitGroupDescription.Type						= D3D12_HIT_GROUP_TYPE_TRIANGLES;
			HitGroupDescription.ClosestHitShaderImport		= InPipelineCreateInformation.ShaderRayTracingClosestHit ? static_cast<D3D12Shader*>(InPipelineCreateInformation.ShaderRayTracingClosestHit)->GetD3D12StageEntryPoint() : nullptr;
			HitGroupDescription.AnyHitShaderImport			= InPipelineCreateInformation.ShaderRayTracingAnyHit ? static_cast<D3D12Shader*>(InPipelineCreateInformation.ShaderRayTracingAnyHit)->GetD3D12StageEntryPoint() : nullptr;
			HitGroupDescription.IntersectionShaderImport	= {};

			uint32_t StateSubObjectsCount = 0u;
			D3D12_STATE_SUBOBJECT* StateSubObjects = reinterpret_cast<D3D12_STATE_SUBOBJECT*>(alloca(sizeof(D3D12_STATE_SUBOBJECT) * RayTracingPipelineStateSubObjectsMaxCount));

			auto AddStateSubObject = [StateSubObjects, &StateSubObjectsCount](_In_ const D3D12_STATE_SUBOBJECT& InStateSubObject)
			{
				StateSubObjects[StateSubObjectsCount++] = InStateSubObject;
			};
			AddStateSubObject(D3D12_STATE_SUBOBJECT_STATE_OBJECT_CONFIG(StateObjectConfig));
			AddStateSubObject(D3D12_STATE_SUBOBJECT_RAYTRACING_PIPELINE_CONFIG1(RayTracingPipelineConfig));
			AddStateSubObject(D3D12_STATE_SUBOBJECT_RAYTRACING_SHADER_CONFIG(RayTracingShaderConfig));
			AddStateSubObject(D3D12_STATE_SUBOBJECT_GLOBAL_ROOT_SIGNATURE(GlobalRootSignature));
			for (uint32_t DXILLibraryIndex = 0; DXILLibraryIndex < DXILLibrariesCount; ++DXILLibraryIndex)
				AddStateSubObject(D3D12_STATE_SUBOBJECT_DXIL_LIBRARY_DESC(DXILLibraryDescriptions[DXILLibraryIndex]));

			if (InPipelineCreateInformation.ShaderRayTracingClosestHit || InPipelineCreateInformation.ShaderRayTracingAnyHit)
				AddStateSubObject(D3D12_STATE_SUBOBJECT_HIT_GROUP_DESC(HitGroupDescription));

			D3D12_STATE_OBJECT_DESC StateObjectDescription;
			StateObjectDescription.Type				= D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
			StateObjectDescription.NumSubobjects	= StateSubObjectsCount;
			StateObjectDescription.pSubobjects		= StateSubObjects;

			VerifySuccess(
				InD3D12Device5->CreateStateObject(
					&StateObjectDescription,
					__uuidof(ID3D12StateObject),
					reinterpret_cast<void**>(&_Pipeline.StateObject)
				)
			);

			std::string PipelineStateName	= "RayGeneration_" + string(InPipelineCreateInformation.ShaderRayTracingRayGeneration->GetName())
											+ (InPipelineCreateInformation.ShaderRayTracingClosestHit	? " ClosestHit_" + string(InPipelineCreateInformation.ShaderRayTracingClosestHit->GetName())	: string())
											+ (InPipelineCreateInformation.ShaderRayTracingMiss			? " Miss_" + string(InPipelineCreateInformation.ShaderRayTracingMiss->GetName())				: string())
											+ (InPipelineCreateInformation.ShaderRayTracingAnyHit		? " AnyHit_" + string(InPipelineCreateInformation.ShaderRayTracingAnyHit->GetName())			: string());
			std::wstring UTF8PipelineStateName(PipelineStateName.begin(), PipelineStateName.end());
			VerifySuccess(
				_Pipeline.StateObject->SetName(UTF8PipelineStateName.c_str())
			);
			_Pipeline.StateObject->SetName(UTF8PipelineStateName.c_str());
		}

		D3D12Pipeline::~D3D12Pipeline()
		{
			if (IsPipelineCompiled())
			{
				if (IsRayTracingPipeline())
				{
					_Pipeline.StateObject->Release();
					_Pipeline.StateObject = nullptr;
				}
				else
				{
					_Pipeline.PipelineState->Release();
					_Pipeline.PipelineState = nullptr;
				}
			}
		}

		const D3D12RootSignature& D3D12Pipeline::GetD3D12RootSignature() const
		{
			return static_cast<const D3D12RootSignature&>(GetRootSignature());
		}

		D3D12Pipeline& D3D12Pipeline::operator=(_In_ const D3D12Pipeline& InPipeline)
		{
			if (this != &InPipeline)
			{
				Pipeline::operator=(InPipeline);
				if (IsRayTracingPipeline())
					_Pipeline.StateObject	= InPipeline._Pipeline.StateObject;
				else
					_Pipeline.PipelineState	= InPipeline._Pipeline.PipelineState;
				_PrimitiveTopology			= InPipeline._PrimitiveTopology;
			}
			return *this;
		}

		bool D3D12Pipeline::IsPipelineCompiled() const
		{
			return IsRayTracingPipeline() ? !!_Pipeline.StateObject : !!_Pipeline.PipelineState;
		}

		bool D3D12Pipeline::IsRayTracingPipeline() const
		{
			return GetPipelineCreateInformation().PipelineShaderTypes != ShaderTypeFlags::SHADER_TYPE_FLAGS_COMPUTE
				&& (GetPipelineCreateInformation().PipelineShaderTypes & ShaderTypeFlags::SHADER_TYPE_FLAGS_RAYTRACING_ALL) != ShaderTypeFlags::SHADER_TYPE_FLAGS_NONE;
		}
	}
}
