#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12CommandList.hpp"

#include <d3d12.h>
#include "d3dx12.h"
#include "Graphics/RenderPass.hpp"
#include "Graphics/DescriptorTable.hpp"
#include "d3d12/D3D12AccelerationStructure.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Format.hpp"
#include "d3d12/D3D12GraphicsContext.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12RootSignature.hpp"
#include "d3d12/D3D12Sampler.hpp"
#include "d3d12/D3D12ShaderTable.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12View.hpp"
#include "WinPixEventRuntime/pix3.h"
#include <array>
#include <string>

#if ETERNAL_USE_DEBUG_VERBOSE
#include "Log/Log.hpp"
#include <sstream>
#include <iomanip>
#endif

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		namespace D3D12
		{
			static constexpr D3D12_RESOURCE_BARRIER DefaultBarrier = {};
		}

		D3D12CommandList::D3D12CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
			: CommandList(InDevice, InCommandAllocator)
		{
			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InDevice);
			D3D12CommandAllocator& InD3DCommandAllocator = static_cast<D3D12CommandAllocator&>(InCommandAllocator);
			VerifySuccess(
				InD3DDevice,
				InD3DDevice.GetD3D12Device()->CreateCommandList(
					InD3DDevice.GetDeviceMask(),
					ConvertCommandTypeToD3D12CommandListType(InD3DCommandAllocator.GetCommandType()),
					InD3DCommandAllocator.GetD3D12CommandAllocator(),
					nullptr,
					__uuidof(ID3D12GraphicsCommandList6),
					reinterpret_cast<void**>(&_GraphicCommandList6)
				)
			);
			End();

#if ETERNAL_USE_NVIDIA_AFTERMATH
			_AftermathContext.InitializeAftermathContext(_GraphicCommandList6);
#endif
		}

		D3D12CommandList::~D3D12CommandList()
		{
#if ETERNAL_USE_NVIDIA_AFTERMATH
			_AftermathContext.ReleaseAftermathContext();
#endif

			_GraphicCommandList6->Release();
			_GraphicCommandList6 = nullptr;
		}

		void D3D12CommandList::SetName(_In_ GraphicsContext& InContext, _In_ const std::string& InName)
		{
			CommandList::SetName(InContext, InName);
			
			std::wstring InNameUTF8(InName.begin(), InName.end());
			VerifySuccess(
				_GraphicCommandList6->SetName(InNameUTF8.c_str())
			);
		}

		void D3D12CommandList::BeginEvent(_In_ GraphicsContext& InContext, _In_ const char* InEventName)
		{
			(void)InContext;
			(void)InEventName;
			PIXBeginEvent(_GraphicCommandList6, 0, InEventName);

#if ETERNAL_USE_NVIDIA_AFTERMATH
			static_cast<D3D12Device&>(InContext.GetDevice()).GetNVIDIANsightAftermath().SetEventMarker(_AftermathContext, InEventName);
#endif
		}

		void D3D12CommandList::EndEvent(_In_ GraphicsContext& InContext)
		{
			(void)InContext;
			PIXEndEvent(_GraphicCommandList6);
		}

		void D3D12CommandList::Begin(_In_ GraphicsContext& InContext)
		{
			VerifySuccess(
				InContext.GetDevice(),
				_GraphicCommandList6->Reset(
					GetD3D12CommandAllocator().GetD3D12CommandAllocator(),
					nullptr
				)
			);

			D3D12GraphicsContext& InD3DContext = static_cast<D3D12GraphicsContext&>(InContext);
			ID3D12DescriptorHeap* DescriptorHeaps[] =
			{
				InD3DContext.GetCBV_SRV_UAV_DescriptorHeap(),
				InD3DContext.GetSamplerDescriptorHeap()
			};
			_GraphicCommandList6->SetDescriptorHeaps(
				ETERNAL_ARRAYSIZE(DescriptorHeaps),
				DescriptorHeaps
			);
		}

		void D3D12CommandList::End()
		{
			VerifySuccess(
				GetDevice(),
				_GraphicCommandList6->Close()
			);
		}

		void D3D12CommandList::BeginRenderPass(_In_ const RenderPass& InRenderPass)
		{
			ETERNAL_PROFILER(INFO)();
			CommandList::BeginRenderPass(InRenderPass);

			// Cache this in D3D12RenderPass?

			static constexpr D3D12_RENDER_PASS_RENDER_TARGET_DESC DefaultRenderPassRenderTargetDesc = {};

			std::array<D3D12_RENDER_PASS_RENDER_TARGET_DESC, MAX_RENDER_TARGETS> RenderPassRenderTargetsDescs;
			RenderPassRenderTargetsDescs.fill(DefaultRenderPassRenderTargetDesc);

			const vector<RenderTargetInformation>& InRenderTargets = InRenderPass.GetRenderTargets();

			for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < InRenderTargets.size(); ++RenderTargetIndex)
			{
				const RenderTargetInformation& CurrentRenderTarget = InRenderTargets[RenderTargetIndex];

				RenderPassRenderTargetsDescs[RenderTargetIndex].cpuDescriptor			= static_cast<D3D12View*>(CurrentRenderTarget.RenderTarget)->GetD3D12CPUDescriptorHandle();

				RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Type	= ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(CurrentRenderTarget.Operator.Load);
				if (CurrentRenderTarget.Operator.Load == LoadOperator::CLEAR)
				{
					RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Clear.ClearValue.Format	= ConvertFormatToD3D12Format(CurrentRenderTarget.RenderTarget->GetViewFormat()).Format;
					memcpy(
						RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Clear.ClearValue.Color,
						CurrentRenderTarget.RenderTarget->GetResource().GetClearValue(),
						sizeof(float) * TextureCreateInformation::ComponentCount
					);
				}

				RenderPassRenderTargetsDescs[RenderTargetIndex].EndingAccess.Type		= ConvertStoreOperatorToD3D12RenderPassEndingAccessType(CurrentRenderTarget.Operator.Store);

				if (CurrentRenderTarget.Operator.Store == StoreOperator::RESOLVE)
				{
					ETERNAL_BREAK(); // Not implemented yet
				}
			}

			D3D12_RENDER_PASS_DEPTH_STENCIL_DESC RenderPassDepthStencilDesc;

			const View* DepthStencilView = InRenderPass.GetDepthStencilRenderTarget();
			if (DepthStencilView)
			{
				LoadOperator DepthStencilLoadOperator	= InRenderPass.GetDepthStencilOperator().Load;
				StoreOperator DepthStencilStoreOperator	= InRenderPass.GetDepthStencilOperator().Store;

				RenderPassDepthStencilDesc.cpuDescriptor				= static_cast<const D3D12View*>(DepthStencilView)->GetD3D12CPUDescriptorHandle();
				RenderPassDepthStencilDesc.DepthBeginningAccess.Type	= ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(DepthStencilLoadOperator);
				if (DepthStencilLoadOperator == LoadOperator::CLEAR)
				{
					RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.Format					= ConvertFormatToD3D12Format(DepthStencilView->GetViewFormat()).Format;
					RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth		= DepthStencilView->GetResource().GetClearValue()[0];
					RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Stencil	= DepthStencilView->GetResource().GetStencilClearValue();
				}
				RenderPassDepthStencilDesc.StencilBeginningAccess.Type	= ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(DepthStencilLoadOperator);
				if (DepthStencilLoadOperator == LoadOperator::CLEAR)
				{
					RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.Format				= ConvertFormatToD3D12Format(DepthStencilView->GetViewFormat()).Format;
					RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Depth	= DepthStencilView->GetResource().GetClearValue()[0];
					RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Stencil	= DepthStencilView->GetResource().GetStencilClearValue();
				}
				RenderPassDepthStencilDesc.DepthEndingAccess.Type		= ConvertStoreOperatorToD3D12RenderPassEndingAccessType(InRenderPass.GetDepthStencilOperator().Store);
				if (DepthStencilStoreOperator == StoreOperator::RESOLVE)
				{
					ETERNAL_BREAK();
				}
				RenderPassDepthStencilDesc.StencilEndingAccess.Type		= ConvertStoreOperatorToD3D12RenderPassEndingAccessType(InRenderPass.GetDepthStencilOperator().Store);
				if (DepthStencilStoreOperator == StoreOperator::RESOLVE)
				{
					ETERNAL_BREAK();
				}
			}

			_GraphicCommandList6->BeginRenderPass(
				static_cast<UINT>(InRenderTargets.size()),
				InRenderTargets.size() > 0 ? RenderPassRenderTargetsDescs.data() : nullptr,
				DepthStencilView ? &RenderPassDepthStencilDesc : nullptr,
				D3D12_RENDER_PASS_FLAG_NONE
			);

			const Viewport& InViewport = InRenderPass.GetViewport();
			SetViewport(InViewport);

			D3D12_RECT ScissorRectangle;
			ScissorRectangle.left			= InViewport.GetX();
			ScissorRectangle.top			= InViewport.GetY();
			ScissorRectangle.right			= InViewport.GetWidth();
			ScissorRectangle.bottom			= InViewport.GetHeight();

			_GraphicCommandList6->RSSetScissorRects(
				1, &ScissorRectangle
			);
		}

		void D3D12CommandList::EndRenderPass()
		{
			CommandList::EndRenderPass();

			_GraphicCommandList6->EndRenderPass();
		}

		void D3D12CommandList::Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
		{
			ETERNAL_PROFILER(VERBOSE)();
			std::array<D3D12_RESOURCE_BARRIER, MaxResourceTransitionsPerSubmit> ResourceBarriers;
			ResourceBarriers.fill(DefaultBarrier);

			uint32_t EffectiveTransitionsCount = 0;
			for (uint32_t TransitionIndex = 0; TransitionIndex < InResourceTransitionsCount; ++TransitionIndex)
			{
				ResourceTransition& CurrentResourceTransition = InResourceTransitions[TransitionIndex];

				D3D12Resource& D3DResource = static_cast<D3D12Resource&>(CurrentResourceTransition.GetResource());

				bool IsWhole = CurrentResourceTransition.SubResource.IsWhole();

				UINT SubResource = [&D3DResource, &CurrentResourceTransition]() -> UINT
				{
					if (D3DResource.GetResourceType() != ResourceType::RESOURCE_TYPE_TEXTURE)
						return ~0u;

					return D3D12CalcSubresource(
						CurrentResourceTransition.SubResource.MipSlice,
						CurrentResourceTransition.SubResource.ArraySlice,
						static_cast<UINT>(CurrentResourceTransition.SubResource.Plane),
						CurrentResourceTransition.SubResource.MipLevelsNeedsResolve() ? D3DResource.GetMIPLevels() : CurrentResourceTransition.SubResource.MipLevels,
						CurrentResourceTransition.SubResource.ArraySizeNeedsResolve() ? D3DResource.GetDepthOrArraySize() : CurrentResourceTransition.SubResource.ArraySize
					);
				}();

				const TransitionState& Before	= IsWhole ? CurrentResourceTransition.GetBefore() : CurrentResourceTransition.GetBefore(SubResource);
				const TransitionState& After	= CurrentResourceTransition.GetAfter();

				if ((Before & After) == After)
					continue;

				ResourceBarriers[EffectiveTransitionsCount].Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				ResourceBarriers[EffectiveTransitionsCount].Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
				ResourceBarriers[EffectiveTransitionsCount].Transition.pResource	= D3DResource.GetD3D12Resource();
				ResourceBarriers[EffectiveTransitionsCount].Transition.Subresource	= IsWhole ? D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES : SubResource;
				ResourceBarriers[EffectiveTransitionsCount].Transition.StateBefore	= ConvertTransitionStateToD3D12ResourceStates(Before);
				ResourceBarriers[EffectiveTransitionsCount].Transition.StateAfter	= ConvertTransitionStateToD3D12ResourceStates(After);

				if (IsWhole)
					D3DResource.SetResourceState(After);
				else
					D3DResource.SetSubResourceState(SubResource, After);

				++EffectiveTransitionsCount;
			}

			if (EffectiveTransitionsCount > 0)
			{
				_GraphicCommandList6->ResourceBarrier(
					EffectiveTransitionsCount,
					ResourceBarriers.data()
				);
			}
		}

		void D3D12CommandList::TransitionUAV(_In_ Resource* InResources[], _In_ uint32_t InResourcesCount)
		{
			ETERNAL_PROFILER(VERBOSE)();
			std::array<D3D12_RESOURCE_BARRIER, MaxResourceTransitionsPerSubmit> ResourceBarriers;
			ResourceBarriers.fill(DefaultBarrier);

			uint32_t EffectiveTransitionsCount = 0;
			for (uint32_t TransitionIndex = 0; TransitionIndex < InResourcesCount; ++TransitionIndex)
			{
				ResourceBarriers[EffectiveTransitionsCount].Type			= D3D12_RESOURCE_BARRIER_TYPE_UAV;
				ResourceBarriers[EffectiveTransitionsCount].Flags			= D3D12_RESOURCE_BARRIER_FLAG_NONE;
				ResourceBarriers[EffectiveTransitionsCount].UAV.pResource	= static_cast<D3D12Resource*>(InResources[TransitionIndex])->GetD3D12Resource();

				++EffectiveTransitionsCount;
			}

			if (EffectiveTransitionsCount > 0)
			{
				_GraphicCommandList6->ResourceBarrier(
					EffectiveTransitionsCount,
					ResourceBarriers.data()
				);
			}
		}

		void D3D12CommandList::SetViewport(_In_ const Viewport& InViewport)
		{
			D3D12_VIEWPORT ViewportInformation;

			ViewportInformation.TopLeftX	= static_cast<FLOAT>(InViewport.GetX());
			ViewportInformation.TopLeftY	= static_cast<FLOAT>(InViewport.GetY());
			ViewportInformation.Width		= static_cast<FLOAT>(InViewport.GetWidth());
			ViewportInformation.Height		= static_cast<FLOAT>(InViewport.GetHeight());
			ViewportInformation.MinDepth	= 0.0f;
			ViewportInformation.MaxDepth	= 1.0f;

			_GraphicCommandList6->RSSetViewports(
				1, &ViewportInformation
			);
		}

		void D3D12CommandList::SetScissorRectangle(_In_ const ScissorRectangle& InScissorRectangle)
		{
			D3D12_RECT D3D12Rect =
			{
				InScissorRectangle.Left,
				InScissorRectangle.Top,
				InScissorRectangle.Right,
				InScissorRectangle.Bottom
			};
			_GraphicCommandList6->RSSetScissorRects(
				1, &D3D12Rect
			);
		}

		void D3D12CommandList::SetGraphicsPipeline(_In_ const Pipeline& InPipeline)
		{
			CommandList::SetGraphicsPipeline(InPipeline);
			ETERNAL_ASSERT((InPipeline.GetShaderTypes() & ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX) == ShaderTypeFlags::SHADER_TYPE_FLAGS_VERTEX);

			const D3D12Pipeline& InD3DPipeline				= static_cast<const D3D12Pipeline&>(InPipeline);
			const D3D12RootSignature* InD3DRootSignature	= static_cast<const D3D12RootSignature*>(InD3DPipeline.GetRootSignature());

			_GraphicCommandList6->SetGraphicsRootSignature(
				InD3DRootSignature->GetD3D12RootSignature()
			);
			_GraphicCommandList6->SetPipelineState(
				InD3DPipeline.GetD3D12PipelineState()
			);
			_GraphicCommandList6->IASetPrimitiveTopology(InD3DPipeline.GetD3D12PrimitiveTopology());
			_GraphicCommandList6->OMSetStencilRef(InPipeline.GetPipelineCreateInformation().PipelineDepthStencil.GetStencilTest().GetReference());
			SetCurrentRootSignature(InD3DRootSignature);
		}

		void D3D12CommandList::SetIndexBuffer(_In_ const Resource& InIndexBuffer, _In_ uint32_t InOffset /* = 0 */, _In_ const IndexBufferType& InIndexBufferType /* = IndexBufferType::INDEX_BUFFER_TYPE_16BITS */)
		{
			const D3D12Resource& InD3DIndexBuffer = static_cast<const D3D12Resource&>(InIndexBuffer);

			D3D12_INDEX_BUFFER_VIEW D3D12IndexBufferView;
			D3D12IndexBufferView.BufferLocation	= InD3DIndexBuffer.GetD3D12Resource()->GetGPUVirtualAddress() + InOffset * ConvertIndexBufferTypeToD3D12Stride(InIndexBufferType);
			D3D12IndexBufferView.Format			= ConvertIndexBufferTypeToDXGIFormat(InIndexBufferType);
			D3D12IndexBufferView.SizeInBytes	= InIndexBuffer.GetBufferSize();

			_GraphicCommandList6->IASetIndexBuffer(&D3D12IndexBufferView);
		}

		void D3D12CommandList::SetVertexBuffers(_In_ const Resource* InVertexBuffers[], _In_ uint32_t InBufferCount /* = 1 */, _In_ uint32_t InFirstVertexBuffer /* = 0 */, _In_ VertexBufferParameters InParameters[] /* = */)
		{
			ETERNAL_ASSERT((InFirstVertexBuffer + InBufferCount) <= MaxVertexBuffers);
			std::array<D3D12_VERTEX_BUFFER_VIEW, MaxVertexBuffers> D3D12VertexBufferViews;
			D3D12VertexBufferViews.fill({});

			uint32_t CurrentBufferIndex = 0;
			for (uint32_t VertexBufferIndex = 0; VertexBufferIndex < InBufferCount; ++VertexBufferIndex)
			{
				D3D12_VERTEX_BUFFER_VIEW& CurrentD3D12VertexBufferView = D3D12VertexBufferViews[CurrentBufferIndex++];

				const D3D12Resource* CurrentD3D12Resource = static_cast<const D3D12Resource*>(InVertexBuffers[VertexBufferIndex]);
				CurrentD3D12VertexBufferView.BufferLocation		= CurrentD3D12Resource->GetD3D12Resource()->GetGPUVirtualAddress() + (InParameters ? InParameters[VertexBufferIndex].Offset : 0);
				CurrentD3D12VertexBufferView.SizeInBytes		= CurrentD3D12Resource->GetBufferSize();
				CurrentD3D12VertexBufferView.StrideInBytes		= CurrentD3D12Resource->GetBufferStride();
			}

			_GraphicCommandList6->IASetVertexBuffers(
				InFirstVertexBuffer,
				InBufferCount,
				D3D12VertexBufferViews.data()
			);
		}

		void D3D12CommandList::SetGraphicsDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			CommandList::SetGraphicsDescriptorTable(InContext, InDescriptorTable);
			_SetDescriptorTable<
				&ID3D12GraphicsCommandList6::SetGraphicsRoot32BitConstants,
				&ID3D12GraphicsCommandList6::SetGraphicsRootShaderResourceView,
				&ID3D12GraphicsCommandList6::SetGraphicsRootConstantBufferView,
				&ID3D12GraphicsCommandList6::SetGraphicsRootUnorderedAccessView,
				&ID3D12GraphicsCommandList6::SetGraphicsRootDescriptorTable
			>(InContext, InDescriptorTable);
		}

		void D3D12CommandList::DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount /* = 1 */, _In_ uint32_t InFirstVertex /* = 0 */, _In_ uint32_t InFirstInstance /* = 0 */)
		{
			_GraphicCommandList6->DrawInstanced(
				InVertexCountPerInstance,
				InInstanceCount,
				InFirstVertex,
				InFirstInstance
			);
		}

		void D3D12CommandList::DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount /* = 1 */, _In_ uint32_t InFirstIndex /* = 0 */, _In_ uint32_t InFirstVertex /* = 0 */, _In_ uint32_t InFirstInstance /* = 0 */)
		{
			_GraphicCommandList6->DrawIndexedInstanced(
				InIndexCountPerInstance,
				InInstanceCount,
				InFirstIndex,
				InFirstVertex,
				InFirstInstance
			);
		}

		void D3D12CommandList::SetComputePipeline(_In_ const Pipeline& InPipeline)
		{
			ETERNAL_ASSERT(InPipeline.GetShaderTypes() == ShaderTypeFlags::SHADER_TYPE_FLAGS_COMPUTE);

			const D3D12Pipeline& InD3DPipeline				= static_cast<const D3D12Pipeline&>(InPipeline);
			const D3D12RootSignature* InD3DRootSignature	= static_cast<const D3D12RootSignature*>(InD3DPipeline.GetRootSignature());

			_GraphicCommandList6->SetComputeRootSignature(
				InD3DRootSignature->GetD3D12RootSignature()
			);
			_GraphicCommandList6->SetPipelineState(
				InD3DPipeline.GetD3D12PipelineState()
			);
			SetCurrentRootSignature(InD3DRootSignature);
		}

		void D3D12CommandList::SetComputeDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			_SetDescriptorTable<
				&ID3D12GraphicsCommandList6::SetComputeRoot32BitConstants,
				&ID3D12GraphicsCommandList6::SetComputeRootShaderResourceView,
				&ID3D12GraphicsCommandList6::SetComputeRootConstantBufferView,
				&ID3D12GraphicsCommandList6::SetComputeRootUnorderedAccessView,
				&ID3D12GraphicsCommandList6::SetComputeRootDescriptorTable
			>(InContext, InDescriptorTable);
		}

		void D3D12CommandList::Dispatch(_In_ uint32_t InX, _In_ uint32_t InY, _In_ uint32_t InZ)
		{
			CommandList::Dispatch(InX, InY, InZ);
			_GraphicCommandList6->Dispatch(InX, InY, InZ);
		}

		void D3D12CommandList::CopyResource(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			switch (InDestinationResource.GetResourceType())
			{
			case ResourceType::RESOURCE_TYPE_BUFFER:
			{
				_CopyResourceToBuffer(InDestinationResource, InSourceResource, InCopyRegion);
			} break;
			case ResourceType::RESOURCE_TYPE_TEXTURE:
			{
				_CopyResourceToTexture(InDestinationResource, InSourceResource, InCopyRegion);
			} break;
			}
		}

		void D3D12CommandList::DispatchMesh(_In_ uint32_t InTaskBatchesCount /* = 1 */)
		{
			_GraphicCommandList6->DispatchMesh(InTaskBatchesCount, 1, 1);
		}

		void D3D12CommandList::SetRayTracingPipeline(_In_ const Pipeline& InPipeline)
		{
			const D3D12Pipeline& InD3DPipeline				= static_cast<const D3D12Pipeline&>(InPipeline);
			ETERNAL_ASSERT(InD3DPipeline.IsRayTracingPipeline());
			const D3D12RootSignature* InD3DRootSignature	= static_cast<const D3D12RootSignature*>(InD3DPipeline.GetRootSignature());

			_GraphicCommandList6->SetComputeRootSignature(
				InD3DRootSignature->GetD3D12RootSignature()
			);
			_GraphicCommandList6->SetPipelineState1(
				InD3DPipeline.GetD3D12StateObject()
			);
			SetCurrentRootSignature(InD3DRootSignature);
		}

		void D3D12CommandList::DispatchRays(_In_ const ShaderTable& InShaderTable, _In_ uint32_t InX /* = 1 */, _In_ uint32_t InY /* = 1 */)
		{
			const D3D12ShaderTable& InD3D12ShaderTable = static_cast<const D3D12ShaderTable&>(InShaderTable);

			D3D12_DISPATCH_RAYS_DESC DispatchRaysDescription	= {};
			DispatchRaysDescription.RayGenerationShaderRecord	= InD3D12ShaderTable.GetD3D12RayGenerationShaderRecord();
			DispatchRaysDescription.MissShaderTable				= InD3D12ShaderTable.GetD3D12MissShaderTable();
			DispatchRaysDescription.HitGroupTable				= InD3D12ShaderTable.GetD3D12HitGroupTable();
			//DispatchRaysDescription.CallableShaderTable			= {};
			DispatchRaysDescription.Width						= InX;
			DispatchRaysDescription.Height						= InY;
			DispatchRaysDescription.Depth						= 1;

#if ETERNAL_USE_DEBUG_VERBOSE
			stringstream DebugInfoStream;
			DebugInfoStream << "DispatchRays:\n";
			DebugInfoStream << "\tRayGenerationShaderRecord: { 0x" << std::hex << std::setfill('0') << std::setw(16) << InD3D12ShaderTable.GetD3D12RayGenerationShaderRecord().StartAddress << ", " << std::dec << InD3D12ShaderTable.GetD3D12RayGenerationShaderRecord().SizeInBytes << " }\n";
			DebugInfoStream << "\tMissShaderTable: { " << std::hex << std::setfill('0') << std::setw(16) << InD3D12ShaderTable.GetD3D12MissShaderTable().StartAddress << ", " << std::dec << InD3D12ShaderTable.GetD3D12MissShaderTable().SizeInBytes << ", " << InD3D12ShaderTable.GetD3D12MissShaderTable().StrideInBytes << " }\n";
			DebugInfoStream << "\tHitGroupTable: { " << std::hex << std::setfill('0') << std::setw(16) << InD3D12ShaderTable.GetD3D12HitGroupTable().StartAddress << ", " << std::dec << InD3D12ShaderTable.GetD3D12HitGroupTable().SizeInBytes << ", " << InD3D12ShaderTable.GetD3D12HitGroupTable().StrideInBytes << " }\n";
			LogWrite(LogError, LogGraphics, DebugInfoStream.str());
#endif

			_GraphicCommandList6->DispatchRays(&DispatchRaysDescription);
		}

		void D3D12CommandList::BuildRaytracingAccelerationStructure(_In_ GraphicsContext& InContext, _In_ AccelerationStructure& InAccelerationStructure)
		{
			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC BuildRaytracingAccelerationStructureDescription = {};
			static_cast<D3D12AccelerationStructure&>(InAccelerationStructure).GetD3D12BuildRaytracingAccelerationStructureInputs(BuildRaytracingAccelerationStructureDescription.Inputs);

			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO PrebuildInfo;
			static_cast<D3D12Device&>(InContext.GetDevice()).GetD3D12Device5()->GetRaytracingAccelerationStructurePrebuildInfo(&BuildRaytracingAccelerationStructureDescription.Inputs, &PrebuildInfo);
			ETERNAL_ASSERT(PrebuildInfo.ScratchDataSizeInBytes < GraphicsContext::ScratchAccelerationStructureBufferSize);

			BuildRaytracingAccelerationStructureDescription.DestAccelerationStructureData		= static_cast<D3D12Resource*>(InAccelerationStructure.GetAccelerationStructure())->GetD3D12Resource()->GetGPUVirtualAddress();
			BuildRaytracingAccelerationStructureDescription.ScratchAccelerationStructureData	= static_cast<D3D12Resource*>(InContext.GetScratchAccelerationStructureBuffer())->GetD3D12Resource()->GetGPUVirtualAddress();
			_GraphicCommandList6->BuildRaytracingAccelerationStructure(&BuildRaytracingAccelerationStructureDescription, 0, nullptr);
		}

		void D3D12CommandList::_CopyResourceToBuffer(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			switch (InSourceResource.GetResourceType())
			{
			case ResourceType::RESOURCE_TYPE_BUFFER:
			{
				_GraphicCommandList6->CopyBufferRegion(
					static_cast<const D3D12Resource&>(InDestinationResource).GetD3D12Resource(),
					InCopyRegion.Buffer.DestinationOffset,
					static_cast<const D3D12Resource&>(InSourceResource).GetD3D12Resource(),
					InCopyRegion.Buffer.SourceOffset,
					InCopyRegion.Buffer.Size
				);
			} break;
			case ResourceType::RESOURCE_TYPE_TEXTURE:
			{
				const D3D12Resource& InD3DDestinationResource = static_cast<const D3D12Resource&>(InDestinationResource);

				D3D12_PLACED_SUBRESOURCE_FOOTPRINT Footprint	= {};
				UINT NumRows									= 0;
				UINT64 RowSizeInBytes							= 0;
				UINT64 TotalBytes								= 0;

				D3D12_RESOURCE_DESC DestinationResourceDesc = InD3DDestinationResource.GetD3D12Resource()->GetDesc();
				GetD3D12Device().GetD3D12Device()->GetCopyableFootprints(
					&DestinationResourceDesc,
					0, 1, InCopyRegion.BufferFromTexture.DestinationOffset,
					&Footprint,
					&NumRows,
					&RowSizeInBytes,
					&TotalBytes
				);
				
				D3D12_TEXTURE_COPY_LOCATION	DestinationTextureCopyLocation;
				DestinationTextureCopyLocation.pResource		= InD3DDestinationResource.GetD3D12Resource();
				DestinationTextureCopyLocation.Type				= D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
				DestinationTextureCopyLocation.PlacedFootprint	= Footprint;

				D3D12_TEXTURE_COPY_LOCATION SourceTextureCopyLocation;
				SourceTextureCopyLocation.pResource				= static_cast<const D3D12Resource&>(InDestinationResource).GetD3D12Resource();
				SourceTextureCopyLocation.Type					= D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				SourceTextureCopyLocation.SubresourceIndex		= D3D12CalcSubresource(
					InCopyRegion.BufferFromTexture.SourceMIPIndex,
					InCopyRegion.BufferFromTexture.Source.GetArraySlice(InSourceResource.GetResourceDimension()),
					InCopyRegion.BufferFromTexture.Source.GetPlaneSlice(InSourceResource.GetResourceDimension()),
					InSourceResource.GetMIPLevels(),
					InSourceResource.GetArraySize()
				);

				const Position3D& InSourcePosition	= InCopyRegion.BufferFromTexture.Source;
				const Extent3D& InSourceExtent		= InCopyRegion.BufferFromTexture.SourceExtent;

				D3D12_BOX SourceBox;
				SourceBox.left		= InSourcePosition.X;
				SourceBox.top		= InSourcePosition.Y;
				SourceBox.front		= InSourcePosition.Z;
				SourceBox.right		= InSourceExtent.Width;
				SourceBox.bottom	= InSourceExtent.Height;
				SourceBox.back		= InSourceExtent.Depth;

				_GraphicCommandList6->CopyTextureRegion(
					&DestinationTextureCopyLocation,
					0, 0, 0,
					&SourceTextureCopyLocation,
					&SourceBox
				);
			} break;
			}

		}

		void D3D12CommandList::_CopyResourceToTexture(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			uint32_t SubresourceIndex				= 0;
			const Position3D& DestinationPosition	= InSourceResource.GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER ? InCopyRegion.TextureFromBuffer.Destination : InCopyRegion.Texture.Destination;
			switch (InSourceResource.GetResourceType())
			{
			case ResourceType::RESOURCE_TYPE_BUFFER:
			{
				SubresourceIndex = D3D12CalcSubresource(
					InCopyRegion.TextureFromBuffer.DestinationMIPIndex,
					InCopyRegion.TextureFromBuffer.Destination.GetArraySlice(InDestinationResource.GetResourceDimension()),
					InCopyRegion.TextureFromBuffer.Destination.GetPlaneSlice(InDestinationResource.GetResourceDimension()),
					InDestinationResource.GetMIPLevels(),
					InDestinationResource.GetArraySize()
				);
			} break;
			case ResourceType::RESOURCE_TYPE_TEXTURE:
			{
				SubresourceIndex = D3D12CalcSubresource(
					InCopyRegion.Texture.DestinationMIPIndex,
					InCopyRegion.Texture.Destination.GetArraySlice(InDestinationResource.GetResourceDimension()),
					InCopyRegion.Texture.Destination.GetPlaneSlice(InDestinationResource.GetResourceDimension()),
					InDestinationResource.GetMIPLevels(),
					InDestinationResource.GetArraySize()
				);
			} break;
			}
			ID3D12Resource* InD3DDestinationResource = static_cast<const D3D12Resource&>(InDestinationResource).GetD3D12Resource();

			D3D12_TEXTURE_COPY_LOCATION DestinationTextureCopyLocation;
			DestinationTextureCopyLocation.pResource		= InD3DDestinationResource;
			DestinationTextureCopyLocation.Type				= D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			DestinationTextureCopyLocation.SubresourceIndex	= SubresourceIndex;

			const D3D12Resource& InD3DSourceResource = static_cast<const D3D12Resource&>(InSourceResource);

			D3D12_BOX					SourceBox = {};
			D3D12_TEXTURE_COPY_LOCATION	SourceTextureCopyLocation;
			SourceTextureCopyLocation.pResource				= InD3DSourceResource.GetD3D12Resource();
			switch (InSourceResource.GetResourceType())
			{
			case ResourceType::RESOURCE_TYPE_BUFFER:
			{
				D3D12_PLACED_SUBRESOURCE_FOOTPRINT Footprint	= {};
				UINT NumRows									= 0;
				UINT64 RowSizeInBytes							= 0;
				UINT64 TotalBytes								= 0;

				D3D12_RESOURCE_DESC DestinationResourceDesc	= InD3DDestinationResource->GetDesc();
				GetD3D12Device().GetD3D12Device()->GetCopyableFootprints(
					&DestinationResourceDesc,
					0, 1, InCopyRegion.TextureFromBuffer.SourceOffset,
					&Footprint,
					&NumRows,
					&RowSizeInBytes,
					&TotalBytes
				);
				
				Footprint.Footprint.Format					= ConvertFormatToD3D12Format(InDestinationResource.GetFormat()).Format;

				SourceTextureCopyLocation.Type				= D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
				SourceTextureCopyLocation.PlacedFootprint	= Footprint;

				uint32_t SourceStride = Footprint.Footprint.RowPitch / Footprint.Footprint.Width;

				SourceBox.left		= static_cast<UINT>((Footprint.Offset % RowSizeInBytes) / SourceStride);
				SourceBox.top		= static_cast<UINT>((Footprint.Offset / RowSizeInBytes) / SourceStride);
				SourceBox.front		= 0;
				SourceBox.right		= Footprint.Footprint.Width;
				SourceBox.bottom	= Footprint.Footprint.Height;
				SourceBox.back		= Footprint.Footprint.Depth;
			} break;
			case ResourceType::RESOURCE_TYPE_TEXTURE:
			{
				SourceTextureCopyLocation.Type					= D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				SourceTextureCopyLocation.SubresourceIndex		= D3D12CalcSubresource(
					InCopyRegion.Texture.SourceMIPIndex,
					InCopyRegion.Texture.Source.GetArraySlice(InSourceResource.GetResourceDimension()),
					InCopyRegion.Texture.Source.GetPlaneSlice(InSourceResource.GetResourceDimension()),
					InSourceResource.GetMIPLevels(),
					InSourceResource.GetArraySize()
				);

				const Position3D& InSourcePosition	= InCopyRegion.Texture.Source;
				const Extent3D InSourceExtent		= InCopyRegion.Texture.Extent;

				SourceBox.left		= InSourcePosition.X;
				SourceBox.top		= InSourcePosition.Y;
				SourceBox.front		= InSourcePosition.Z;
				SourceBox.right		= InSourceExtent.Width;
				SourceBox.bottom	= InSourceExtent.Height;
				SourceBox.back		= InSourceExtent.Depth;
			} break;
			}

			_GraphicCommandList6->CopyTextureRegion(
				&DestinationTextureCopyLocation,
				DestinationPosition.X,
				DestinationPosition.Y,
				DestinationPosition.Z,
				&SourceTextureCopyLocation,
				&SourceBox
			);
		}

		template<
			SetRoot32BitConstantsType SetRoot32BitConstants,
			SetRootShaderResourceViewType SetRootShaderResourceView,
			SetRootConstantBufferViewType SetRootConstantBufferView,
			SetRootUnorderedAccessViewType SetRootUnorderedAccessView,
			SetRootDescriptorTableType SetRootDescriptorTable
		>
		void D3D12CommandList::_SetDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			(void)InContext;
			ETERNAL_PROFILER(VERBOSE)();

			const RootSignatureCreateInformation& DescriptorTableLayout	= GetCurrentSignature()->GetCreateInformation();
			const vector<RootSignatureConstants>& Constants				= DescriptorTableLayout.Constants;
			const vector<RootSignatureParameter>& Parameters			= DescriptorTableLayout.Parameters;
			
			const vector<DescriptorTableConstants>& InConstants			= static_cast<const DescriptorTable&>(InDescriptorTable).GetConstants();
			const vector<DescriptorTableResource>& InResources			= static_cast<const DescriptorTable&>(InDescriptorTable).GetResources();
			ResourcesDirtyFlagsType& InConstantsDirtyFlags				= InDescriptorTable.GetConstantsDirtyFlags();
			ResourcesDirtyFlagsType& InResourcesDirtyFlags				= InDescriptorTable.GetResourcesDirtyFlags();

			uint32_t RootParameterIndex = 0;
			for (uint32_t ConstantIndex = 0; ConstantIndex < Constants.size(); ++ConstantIndex, ++RootParameterIndex)
			{
				if (InConstantsDirtyFlags.IsSet(ConstantIndex))
				{
					(_GraphicCommandList6->*SetRoot32BitConstants)(
						RootParameterIndex,
						static_cast<UINT>(InConstants[ConstantIndex].Constants.size()),
						InConstants[ConstantIndex].Constants.data(),
						RootParameterIndex
					);
					InConstantsDirtyFlags.Unset(ConstantIndex);
				}
			}

			for (uint32_t ParameterIndex = 0; ParameterIndex < Parameters.size(); ++ParameterIndex, ++RootParameterIndex)
			{
				const RootSignatureParameter& CurrentParameter = Parameters[ParameterIndex];

				D3D12_GPU_DESCRIPTOR_HANDLE DescriptorTableHandle = {};

				switch (CurrentParameter.Parameter)
				{
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER:
				{
					DescriptorTableHandle = static_cast<const D3D12Sampler*>(InResources[ParameterIndex].ResourceSampler)->GetD3D12GPUDescriptorHandle();
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_TEXTURE:
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_TEXTURE:
				{
					DescriptorTableHandle = static_cast<const D3D12View*>(InResources[ParameterIndex].ResourceView)->GetD3D12GPUDescriptorHandle();
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER:
				{
					if (InResourcesDirtyFlags.IsSet(ParameterIndex))
					{
						(_GraphicCommandList6->*SetRootShaderResourceView)(
							RootParameterIndex,
							static_cast<const D3D12View*>(InResources[ParameterIndex].ResourceView)->GetD3D12OffsettedBuffer()
						);

						InResourcesDirtyFlags.Unset(ParameterIndex);
					}
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER:
					ETERNAL_BREAK(); // Not implemented yet
					break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_CONSTANT_BUFFER:
				{
					if (InResourcesDirtyFlags.IsSet(ParameterIndex))
					{
						(_GraphicCommandList6->*SetRootConstantBufferView)(
							RootParameterIndex,
							static_cast<const D3D12View*>(InResources[ParameterIndex].ResourceView)->GetD3D12OffsettedConstantBuffer()
						);
						InResourcesDirtyFlags.Unset(ParameterIndex);
					}
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_BUFFER:
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_BUFFER:
					ETERNAL_BREAK(); // Not implemented yet
					break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RAYTRACING_ACCELERATION_STRUCTURE:
				{
					if (InResourcesDirtyFlags.IsSet(ParameterIndex))
					{
						(_GraphicCommandList6->*SetRootShaderResourceView)(
							RootParameterIndex,
							const_cast<D3D12Resource&>(static_cast<const D3D12Resource&>(InResources[ParameterIndex].ResourceView->GetResource())).GetD3D12Resource()->GetGPUVirtualAddress()
						);
						InResourcesDirtyFlags.Unset(ParameterIndex);
					}
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE:
				{
					D3D12_GPU_DESCRIPTOR_HANDLE SubResourceTableDescriptorHandle = {};
					switch (CurrentParameter.DescriptorTable.Parameters[0].Parameter)
					{
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER:
					{
						SubResourceTableDescriptorHandle = static_cast<const D3D12Sampler*>(InResources[ParameterIndex].ResourceDescriptorTable->GetResources()[0].ResourceSampler)->GetD3D12GPUDescriptorHandle();
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_TEXTURE:
					{
						SubResourceTableDescriptorHandle = static_cast<const D3D12View*>(InResources[ParameterIndex].ResourceDescriptorTable->GetResources()[0].ResourceView)->GetD3D12GPUDescriptorHandle();
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_TEXTURE:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_CONSTANT_BUFFER:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_BUFFER:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_BUFFER:
						ETERNAL_BREAK();
						break;
					}
					if (InResourcesDirtyFlags.IsSet(ParameterIndex))
					{
						(_GraphicCommandList6->*SetRootDescriptorTable)(
							RootParameterIndex,
							SubResourceTableDescriptorHandle
						);
						InResourcesDirtyFlags.Unset(ParameterIndex);
					}
				} break;
				}

				if (DescriptorTableHandle.ptr)
				{
					bool IsDirty = InResourcesDirtyFlags.IsSet(ParameterIndex);
					if (IsDirty)
					{
						(_GraphicCommandList6->*SetRootDescriptorTable)(
							RootParameterIndex,
							DescriptorTableHandle
						);
						InResourcesDirtyFlags.Unset(ParameterIndex);
					}
				}
			}
		}
	}
}

#endif
