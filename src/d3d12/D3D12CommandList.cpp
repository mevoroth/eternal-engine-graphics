#include "d3d12/D3D12CommandList.hpp"

#include <d3d12.h>
#include <d3dx12.h>
#include "Graphics/RenderPass.hpp"
#include "Graphics/DescriptorTable.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Format.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "d3d12/D3D12RootSignature.hpp"
#include "d3d12/D3D12Sampler.hpp"
#include "d3d12/D3D12GraphicsContext.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		D3D12CommandList::D3D12CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
			: CommandList(InDevice, InCommandAllocator)
		{
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
			SetViewport(InViewport);

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
			CommandList::EndRenderPass();

			_GraphicCommandList5->EndRenderPass();
		}

		void D3D12CommandList::Transition(_In_ ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount)
		{
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

		void D3D12CommandList::SetViewport(_In_ const Viewport& InViewport)
		{
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
			_GraphicCommandList5->RSSetScissorRects(
				1, &D3D12Rect
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
			SetCurrentRootSignature(&InD3DRootSignature);
		}

		void D3D12CommandList::SetIndexBuffer(_In_ const Resource& InIndexBuffer, _In_ uint32_t InOffset /* = 0 */, _In_ const IndexBufferType& InIndexBufferType /* = IndexBufferType::INDEX_BUFFER_TYPE_16BITS */)
		{
			const D3D12Resource& InD3DIndexBuffer = static_cast<const D3D12Resource&>(InIndexBuffer);

			D3D12_INDEX_BUFFER_VIEW D3D12IndexBufferView;
			D3D12IndexBufferView.BufferLocation	= InD3DIndexBuffer.GetD3D12Resource()->GetGPUVirtualAddress() + InOffset * ConvertIndexBufferTypeToD3D12Stride(InIndexBufferType);
			D3D12IndexBufferView.Format			= ConvertIndexBufferTypeToDXGIFormat(InIndexBufferType);
			D3D12IndexBufferView.SizeInBytes	= InIndexBuffer.GetBufferSize();

			_GraphicCommandList5->IASetIndexBuffer(&D3D12IndexBufferView);
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

			_GraphicCommandList5->IASetVertexBuffers(
				InFirstVertexBuffer,
				InBufferCount,
				D3D12VertexBufferViews.data()
			);
		}

		void D3D12CommandList::SetGraphicsDescriptorTable(_In_ GraphicsContext& InContext, _In_ DescriptorTable& InDescriptorTable)
		{
			ETERNAL_ASSERT(GetCurrentSignature() && *GetCurrentSignature() == *InDescriptorTable.GetRootSignature());
			//ETERNAL_ASSERT(_CurrentRootSignature && GetCurrentSignature() == InDescriptorTable.GetRootSignature()); Faster but limiting possibilities
			
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
					_GraphicCommandList5->SetGraphicsRoot32BitConstants(
						RootParameterIndex,
						static_cast<UINT>(InConstants[ConstantIndex].Constants.size()),
						InConstants[ConstantIndex].Constants.data(),
						RootParameterIndex
					);
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
				{
					DescriptorTableHandle = static_cast<const D3D12View*>(InResources[ParameterIndex].ResourceView)->GetD3D12GPUDescriptorHandle();
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_TEXTURE:
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER:
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER:
					ETERNAL_BREAK(); // Not implemented yet
					break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_CONSTANT_BUFFER:
				{
					if (InResourcesDirtyFlags.IsSet(ParameterIndex))
					{
						_GraphicCommandList5->SetGraphicsRootConstantBufferView(
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
						_GraphicCommandList5->SetGraphicsRootDescriptorTable(
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
					InResourcesDirtyFlags.Unset(ParameterIndex);
					if (IsDirty)
					{
						_GraphicCommandList5->SetGraphicsRootDescriptorTable(
							RootParameterIndex,
							DescriptorTableHandle
						);
					}
				}
			}
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

		void D3D12CommandList::_CopyResourceToBuffer(_In_ const Resource& InDestinationResource, _In_ const Resource& InSourceResource, _In_ const CopyRegion& InCopyRegion)
		{
			switch (InSourceResource.GetResourceType())
			{
			case ResourceType::RESOURCE_TYPE_BUFFER:
			{
				_GraphicCommandList5->CopyBufferRegion(
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

				_GraphicCommandList5->CopyTextureRegion(
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

			D3D12_BOX					SourceBox;
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

			_GraphicCommandList5->CopyTextureRegion(
				&DestinationTextureCopyLocation,
				DestinationPosition.X,
				DestinationPosition.Y,
				DestinationPosition.Z,
				&SourceTextureCopyLocation,
				&SourceBox
			);
		}
	}
}
