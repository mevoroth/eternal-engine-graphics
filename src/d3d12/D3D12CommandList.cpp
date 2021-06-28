#include "d3d12/D3D12CommandList.hpp"

#include <array>
#include <d3d12.h>
#include "Graphics/RenderPass.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Format.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "d3d12/D3D12RootSignature.hpp"
#include "d3d12/D3D12GraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12CommandList::D3D12CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
			: CommandList(InCommandAllocator)
		{
			using namespace Eternal::Graphics::D3D12;

			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InDevice);
			D3D12CommandAllocator& InD3DCommandAllocator = static_cast<D3D12CommandAllocator&>(InCommandAllocator);
			VerifySuccess(
				InD3DDevice.GetD3D12Device()->CreateCommandList(
					InD3DDevice.GetDeviceMask(),
					ConvertCommandTypeToD3D12CommandListType(InD3DCommandAllocator.GetCommandType()),
					InD3DCommandAllocator.GetD3D12CommandAllocator(),
					nullptr,
					__uuidof(ID3D12GraphicsCommandList5),
					(void**)&_GraphicCommandList5
				)
			);
			End();
		}

		D3D12CommandList::~D3D12CommandList()
		{
			_GraphicCommandList5->Release();
			_GraphicCommandList5 = nullptr;
		}

		void D3D12CommandList::Begin(_In_ GraphicsContext& InContext)
		{
			_GraphicCommandList5->Reset(
				GetD3D12CommandAllocator().GetD3D12CommandAllocator(),
				nullptr
			);

			D3D12GraphicsContext& InD3DContext = static_cast<D3D12GraphicsContext&>(InContext);
			ID3D12DescriptorHeap* DescriptorHeaps[] =
			{
				InD3DContext.GetCBV_SRV_UAV_DescriptorHeap(),
				InD3DContext.GetSamplerDescriptorHeap()
			};
			_GraphicCommandList5->SetDescriptorHeaps(
				ETERNAL_ARRAYSIZE(DescriptorHeaps),
				DescriptorHeaps
			);
		}

		void D3D12CommandList::End()
		{
			_GraphicCommandList5->Close();
		}

		void D3D12CommandList::BeginRenderPass(_In_ const RenderPass& InRenderPass)
		{
			using namespace Eternal::Graphics::D3D12;

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
						sizeof(float) * ETERNAL_ARRAYSIZE(CurrentRenderTarget.RenderTarget->GetResource().GetClearValue())
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
					RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth		= 0.0f;
					RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Stencil	= 0x0;
				}
				RenderPassDepthStencilDesc.StencilBeginningAccess.Type	= ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(DepthStencilLoadOperator);
				if (DepthStencilLoadOperator == LoadOperator::CLEAR)
				{
					RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.Format				= ConvertFormatToD3D12Format(DepthStencilView->GetViewFormat()).Format;
					RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Depth	= 0.0f;
					RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Stencil	= 0x0;
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

			_GraphicCommandList5->BeginRenderPass(
				static_cast<UINT>(InRenderTargets.size()),
				InRenderTargets.size() > 0 ? RenderPassRenderTargetsDescs.data() : nullptr,
				DepthStencilView ? &RenderPassDepthStencilDesc : nullptr,
				D3D12_RENDER_PASS_FLAG_NONE
			);

			const Viewport& InViewport = InRenderPass.GetViewport();
			D3D12_VIEWPORT ViewportInformation;

			ViewportInformation.TopLeftX	= static_cast<FLOAT>(InViewport.GetX());
			ViewportInformation.TopLeftY	= static_cast<FLOAT>(InViewport.GetY());
			ViewportInformation.Width		= static_cast<FLOAT>(InViewport.GetWidth());
			ViewportInformation.Height		= static_cast<FLOAT>(InViewport.GetHeight());
			ViewportInformation.MinDepth	= 0.0f;
			ViewportInformation.MaxDepth	= 1.0f;

			_GraphicCommandList5->RSSetViewports(
				1, &ViewportInformation
			);

			D3D12_RECT ScissorRectangle;
			ScissorRectangle.left			= InViewport.GetX();
			ScissorRectangle.top			= InViewport.GetY();
			ScissorRectangle.right			= InViewport.GetWidth();
			ScissorRectangle.bottom			= InViewport.GetHeight();

			_GraphicCommandList5->RSSetScissorRects(
				1, &ScissorRectangle
			);
		}

		void D3D12CommandList::EndRenderPass()
		{
			_GraphicCommandList5->EndRenderPass();
		}

		void D3D12CommandList::Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
		{
			using namespace Eternal::Graphics::D3D12;

			static constexpr D3D12_RESOURCE_BARRIER DefaultBarrier = {};
			std::array<D3D12_RESOURCE_BARRIER, MaxResourceTransitionsPerSubmit> ResourceBarriers;
			ResourceBarriers.fill(DefaultBarrier);

			for (uint32_t TransitionIndex = 0; TransitionIndex < InResourceTransitionsCount; ++TransitionIndex)
			{
				ResourceTransition& CurrentResourceTransition = InResourceTransitions[TransitionIndex];

				D3D12Resource& D3DResource = static_cast<D3D12Resource&>(CurrentResourceTransition.GetResource());

				ResourceBarriers[TransitionIndex].Type						= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				ResourceBarriers[TransitionIndex].Flags						= D3D12_RESOURCE_BARRIER_FLAG_NONE;
				ResourceBarriers[TransitionIndex].Transition.pResource		= D3DResource.GetD3D12Resource();
				ResourceBarriers[TransitionIndex].Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				ResourceBarriers[TransitionIndex].Transition.StateBefore	= ConvertTransitionStateToD3D12ResourceStates(CurrentResourceTransition.GetBefore());
				ResourceBarriers[TransitionIndex].Transition.StateAfter		= ConvertTransitionStateToD3D12ResourceStates(CurrentResourceTransition.GetAfter());
				
				D3DResource.SetResourceState(CurrentResourceTransition.After);
			}

			_GraphicCommandList5->ResourceBarrier(
				InResourceTransitionsCount,
				ResourceBarriers.data()
			);
		}

		void D3D12CommandList::SetGraphicsPipeline(_In_ const Pipeline& InPipeline)
		{
			const D3D12Pipeline& InD3DPipeline				= static_cast<const D3D12Pipeline&>(InPipeline);
			const D3D12RootSignature& InD3DRootSignature	= static_cast<const D3D12RootSignature&>(InD3DPipeline.GetRootSignature());

			_GraphicCommandList5->SetGraphicsRootSignature(
				InD3DRootSignature.GetD3D12RootSignature()
			);
			_GraphicCommandList5->SetPipelineState(
				InD3DPipeline.GetD3D12PipelineState()
			);
			_GraphicCommandList5->IASetPrimitiveTopology(InD3DPipeline.GetD3D12PrimitiveTopology());
		}

		void D3D12CommandList::DrawInstanced(_In_ uint32_t InVertexCountPerInstance, _In_ uint32_t InInstanceCount /* = 1 */, _In_ uint32_t InFirstVertex /* = 0 */, _In_ uint32_t InFirstInstance /* = 0 */)
		{
			_GraphicCommandList5->DrawInstanced(
				InVertexCountPerInstance,
				InInstanceCount,
				InFirstVertex,
				InFirstInstance
			);
		}

		void D3D12CommandList::DrawIndexedInstanced(_In_ uint32_t InIndexCountPerInstance, _In_ uint32_t InInstanceCount /* = 1 */, _In_ uint32_t InFirstIndex /* = 0 */, _In_ uint32_t InFirstVertex /* = 0 */, _In_ uint32_t InFirstInstance /* = 0 */)
		{
			_GraphicCommandList5->DrawIndexedInstanced(
				InIndexCountPerInstance,
				InInstanceCount,
				InFirstIndex,
				InFirstVertex,
				InFirstInstance
			);
		}
	}
}
