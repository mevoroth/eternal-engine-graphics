#include "d3d12/D3D12CommandList.hpp"

#include <vector>
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
			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InDevice);
			D3D12CommandAllocator& InD3DCommandAllocator = static_cast<D3D12CommandAllocator&>(InCommandAllocator);
			D3D12::VerifySuccess(
				InD3DDevice.GetD3D12Device()->CreateCommandList(
					InD3DDevice.GetDeviceMask(),
					D3D12_COMMAND_LIST_TYPES[static_cast<int32_t>(InD3DCommandAllocator.GetCommandType())],
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

		void D3D12CommandList::BeginRenderPass(RenderPass& InRenderPass)
		{
			std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> RenderPassRenderTargetsDescs;
			RenderPassRenderTargetsDescs.resize(InRenderPass.GetRenderTargets().size());
			for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < InRenderPass.GetRenderTargets().size(); ++RenderTargetIndex)
			{
				const RenderTargetInformation& CurrentRenderTarget = InRenderPass.GetRenderTargets()[RenderTargetIndex];

				RenderPassRenderTargetsDescs[RenderTargetIndex].cpuDescriptor			= static_cast<D3D12View*>(CurrentRenderTarget.RenderTarget)->GetD3D12CPUDescriptorHandle();

				RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Type	= D3D12_LOAD_OPS[static_cast<int32_t>(CurrentRenderTarget.Operator.LoadOp)];
				if (CurrentRenderTarget.Operator.LoadOp == LoadOperator::CLEAR)
				{
					RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Clear.ClearValue.Format	= D3D12_FORMATS[static_cast<int32_t>(CurrentRenderTarget.RenderTarget->GetViewFormat())].Format;
					CurrentRenderTarget.RenderTarget->GetClearValue(
						RenderPassRenderTargetsDescs[RenderTargetIndex].BeginningAccess.Clear.ClearValue.Color
					);
				}

				RenderPassRenderTargetsDescs[RenderTargetIndex].EndingAccess.Type	= D3D12_STORE_OPS[static_cast<int32_t>(CurrentRenderTarget.Operator.StoreOp)];

				if (CurrentRenderTarget.Operator.StoreOp == StoreOperator::RESOLVE)
				{
					ETERNAL_BREAK(); // Not implemented yet
				}
			}

			LoadOperator DepthStencilLoadOperator = InRenderPass.GetDepthStencilOperator().LoadOp;
			StoreOperator DepthStencilStoreOperator = InRenderPass.GetDepthStencilOperator().StoreOp;

			D3D12_RENDER_PASS_DEPTH_STENCIL_DESC RenderPassDepthStencilDesc;
			RenderPassDepthStencilDesc.cpuDescriptor				= static_cast<const D3D12View*>(InRenderPass.GetDepthStencilRenderTarget())->GetD3D12CPUDescriptorHandle();
			RenderPassDepthStencilDesc.DepthBeginningAccess.Type	= D3D12_LOAD_OPS[static_cast<int32_t>(DepthStencilLoadOperator)];
			if (DepthStencilLoadOperator == LoadOperator::CLEAR)
			{
				RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.Format					= D3D12_FORMATS[static_cast<int32_t>(InRenderPass.GetDepthStencilRenderTarget()->GetViewFormat())].Format;
				RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth		= 0.0f;
				RenderPassDepthStencilDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Stencil	= 0x0;
			}
			RenderPassDepthStencilDesc.StencilBeginningAccess.Type	= D3D12_LOAD_OPS[static_cast<int32_t>(DepthStencilLoadOperator)];
			if (DepthStencilLoadOperator == LoadOperator::CLEAR)
			{
				RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.Format				= D3D12_FORMATS[static_cast<int32_t>(InRenderPass.GetDepthStencilRenderTarget()->GetViewFormat())].Format;
				RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Depth	= 0.0f;
				RenderPassDepthStencilDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Stencil	= 0x0;
			}
			RenderPassDepthStencilDesc.DepthEndingAccess.Type		= D3D12_STORE_OPS[static_cast<int32_t>(InRenderPass.GetDepthStencilOperator().StoreOp)];
			if (DepthStencilStoreOperator == StoreOperator::RESOLVE)
			{
				ETERNAL_BREAK();
			}
			RenderPassDepthStencilDesc.StencilEndingAccess.Type		= D3D12_STORE_OPS[static_cast<int32_t>(InRenderPass.GetDepthStencilOperator().StoreOp)];
			if (DepthStencilStoreOperator == StoreOperator::RESOLVE)
			{
				ETERNAL_BREAK();
			}

			_GraphicCommandList5->BeginRenderPass(
				static_cast<UINT>(RenderPassRenderTargetsDescs.size()),
				RenderPassRenderTargetsDescs.data(),
				&RenderPassDepthStencilDesc,
				D3D12_RENDER_PASS_FLAG_NONE
			);
		}

		void D3D12CommandList::EndRenderPass()
		{
			_GraphicCommandList5->EndRenderPass();
		}
	}
}
