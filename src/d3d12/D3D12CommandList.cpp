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

		void D3D12CommandList::Begin()
		{
			_GraphicCommandList5->Reset(
				GetD3D12CommandAllocator().GetD3D12CommandAllocator(),
				nullptr
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
			for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < InRenderPass.GetRenderTargets().size(); ++RenderTargetIndex)
			{
				const RenderTargetInformation& CurrentRenderTarget = InRenderPass.GetRenderTargets()[RenderTargetIndex];

				RenderPassRenderTargetsDescs[RenderTargetIndex].cpuDescriptor			= static_cast<D3D12View*>(CurrentRenderTarget.RenderTarget)->GetD3D12CPUDescriptorHandle();

				RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Type	= ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(CurrentRenderTarget.Operator.Load);
				if (CurrentRenderTarget.Operator.Load == LoadOperator::CLEAR)
				{
					RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Clear.ClearValue.Format	= D3D12_FORMATS[static_cast<int32_t>(CurrentRenderTarget.RenderTarget->GetViewFormat())].Format;
					memcpy(RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Clear.ClearValue.Color, CurrentRenderTarget.ClearValue, sizeof(float) * ETERNAL_ARRAYSIZE(CurrentRenderTarget.ClearValue));
				}

				RenderPassRenderTargetsDescs[RenderTargetIndex].EndingAccess.Type		= ConvertStoreOperatorToD3D12RenderPassEndingAccessType(CurrentRenderTarget.Operator.Store);

				if (CurrentRenderTarget.Operator.Store == StoreOperator::RESOLVE)
				{
					ETERNAL_BREAK(); // Not implemented yet
				}
			}

			D3D12_RENDER_PASS_DEPTH_STENCIL_DESC RenderPassDepthStencilDesc;

			if (InRenderPass.GetDepthStencilRenderTarget())
			{
				LoadOperator DepthStencilLoadOperator	= InRenderPass.GetDepthStencilOperator().Load;
				StoreOperator DepthStencilStoreOperator	= InRenderPass.GetDepthStencilOperator().Store;

				RenderPassDepthStencilDesc.cpuDescriptor				= static_cast<const D3D12View*>(InRenderPass.GetDepthStencilRenderTarget())->GetD3D12CPUDescriptorHandle();
				RenderPassDepthStencilDesc.DepthBeginningAccess.Type	= ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(DepthStencilLoadOperator);
				if (DepthStencilLoadOperator == LoadOperator::CLEAR)
				{
					RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.Format					= D3D12_FORMATS[static_cast<int32_t>(InRenderPass.GetDepthStencilRenderTarget()->GetViewFormat())].Format;
					RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth		= 0.0f;
					RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Stencil	= 0x0;
				}
				RenderPassDepthStencilDesc.StencilBeginningAccess.Type	= ConvertLoadOperatorToD3D12RenderPassBeginningAccessType(DepthStencilLoadOperator);
				if (DepthStencilLoadOperator == LoadOperator::CLEAR)
				{
					RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.Format				= D3D12_FORMATS[static_cast<int32_t>(InRenderPass.GetDepthStencilRenderTarget()->GetViewFormat())].Format;
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
				static_cast<UINT>(InRenderPass.GetRenderTargets().size()),
				InRenderPass.GetRenderTargets().size() > 0 ? RenderPassRenderTargetsDescs.data() : nullptr,
				InRenderPass.GetDepthStencilRenderTarget() ? &RenderPassDepthStencilDesc : nullptr,
				D3D12_RENDER_PASS_FLAG_NONE
			);
		}

		void D3D12CommandList::EndRenderPass()
		{
			_GraphicCommandList5->EndRenderPass();
		}

		void D3D12CommandList::Transition(_In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
		{
			using namespace Eternal::Graphics::D3D12;

			static constexpr D3D12_RESOURCE_BARRIER DefaultBarrier = {};
			std::array<D3D12_RESOURCE_BARRIER, MaxResourceTransitionsPerSubmit> ResourceBarriers;
			ResourceBarriers.fill(DefaultBarrier);

			for (uint32_t TransitionIndex = 0; TransitionIndex < InResourceTransitionsCount; ++TransitionIndex)
			{
				const ResourceTransition& CurrentResourceTransition = InResourceTransitions[TransitionIndex];

				D3D12Resource& D3DResource = static_cast<D3D12Resource&>(CurrentResourceTransition.ResourceToTransition->GetResource());

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
	}
}
