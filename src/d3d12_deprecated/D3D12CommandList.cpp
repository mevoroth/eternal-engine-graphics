#include "d3d12_deprecated/D3D12CommandList.hpp"

#define WIN32_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#include "Graphics/Viewport.hpp"
#include "d3d12_deprecated/D3D12Device.hpp"
#include "d3d12_deprecated/D3D12CommandQueue.hpp"
#include "d3d12_deprecated/D3D12Pipeline.hpp"
#include "d3d12_deprecated/D3D12RenderTarget.hpp"
#include "d3d12_deprecated/D3D12CommandAllocator.hpp"
#include "d3d12_deprecated/D3D12View.hpp"
#include "d3d12_deprecated/D3D12RootSignature.hpp"
#include "d3d12_deprecated/D3D12RenderPass.hpp"
#include "d3d12_deprecated/D3D12View.hpp"
#include "d3d12_deprecated/D3D12Format.hpp"
#include "d3d12_deprecated/D3D12DescriptorHeap.hpp"
#include "d3d12_deprecated/D3D12DescriptorTable.hpp"

using namespace Eternal::Graphics;

namespace Eternal
{
	namespace Graphics
	{
		const D3D12_COMMAND_LIST_TYPE D3D12_COMMAND_LIST_TYPES[] =
		{
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			D3D12_COMMAND_LIST_TYPE_COMPUTE,
			D3D12_COMMAND_LIST_TYPE_COPY
		};
	}
}

/*
TRANSITION_UNDEFINED				= 0x0,
TRANSITION_INDIRECT					= 0x1,
TRANSITION_INDEX_READ				= 0x2,
TRANSITION_VERTEX_ATTRIBUTE_READ	= 0x4,
TRANSITION_CONSTANT_READ			= 0x8,
TRANSITION_RENDER_TARGET_READ		= 0x10,
TRANSITION_SHADER_READ				= 0x20,
TRANSITION_SHADER_WRITE				= 0x40,
TRANSITION_BLEND_READ				= 0x80,
TRANSITION_RENDER_TARGET_WRITE		= 0x100,
TRANSITION_DEPTH_STENCIL_READ		= 0x200,
TRANSITION_DEPTH_STENCIL_WRITE		= 0x400,
TRANSITION_COPY_READ				= 0x800,
TRANSITION_COPY_WRITE				= 0x1000,
TRANSITION_CPU_READ					= 0x2000,
TRANSITION_CPU_WRITE				= 0x4000,
TRANSITION_PRESENT					= 0x8000
*/
/*
//D3D12_RESOURCE_STATE_COMMON	= 0,
//D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER	= 0x1,
//D3D12_RESOURCE_STATE_INDEX_BUFFER	= 0x2,
//D3D12_RESOURCE_STATE_RENDER_TARGET	= 0x4,
//D3D12_RESOURCE_STATE_UNORDERED_ACCESS	= 0x8,
//D3D12_RESOURCE_STATE_DEPTH_WRITE	= 0x10,
//D3D12_RESOURCE_STATE_DEPTH_READ	= 0x20,
//D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE	= 0x40,
//D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE	= 0x80,
D3D12_RESOURCE_STATE_STREAM_OUT	= 0x100,
//D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT	= 0x200,
//D3D12_RESOURCE_STATE_COPY_DEST	= 0x400,
//D3D12_RESOURCE_STATE_COPY_SOURCE	= 0x800,
D3D12_RESOURCE_STATE_RESOLVE_DEST	= 0x1000,
D3D12_RESOURCE_STATE_RESOLVE_SOURCE	= 0x2000,

D3D12_RESOURCE_STATE_GENERIC_READ	= ( ( ( ( ( 0x1 | 0x2 )  | 0x40 )  | 0x80 )  | 0x200 )  | 0x800 ) ,
D3D12_RESOURCE_STATE_PRESENT	= 0,
D3D12_RESOURCE_STATE_PREDICATION	= 0x200
*/

D3D12_RESOURCE_STATES BuildResourceStates(_In_ const TransitionState& State)
{
	D3D12_RESOURCE_STATES ResourceState = D3D12_RESOURCE_STATE_COMMON;

	//if (State & TRANSITION_PRESENT)
	//{
	//	ETERNAL_ASSERT(!(State & ~TRANSITION_PRESENT));
	//}
	if (State & TRANSITION_PRESENT)
		return D3D12_RESOURCE_STATE_PRESENT;

	if (State & (TRANSITION_VERTEX_ATTRIBUTE_READ | TRANSITION_CONSTANT_READ))
		ResourceState |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

	if (State & TRANSITION_INDEX_READ)
		ResourceState |= D3D12_RESOURCE_STATE_INDEX_BUFFER;

	if (State & TRANSITION_RENDER_TARGET_WRITE)
		ResourceState |= D3D12_RESOURCE_STATE_RENDER_TARGET;

	if (State & TRANSITION_SHADER_WRITE)
		ResourceState |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

	if (State & TRANSITION_DEPTH_STENCIL_WRITE)
		ResourceState |= D3D12_RESOURCE_STATE_DEPTH_WRITE;

	if (State & TRANSITION_DEPTH_STENCIL_READ)
		ResourceState |= D3D12_RESOURCE_STATE_DEPTH_READ;

	if (State & TRANSITION_SHADER_READ)
		ResourceState |= D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	if (State & TRANSITION_INDIRECT)
		ResourceState |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;

	if (State & TRANSITION_COPY_WRITE)
		ResourceState |= D3D12_RESOURCE_STATE_COPY_DEST;

	if (State & TRANSITION_COPY_READ)
		ResourceState |= D3D12_RESOURCE_STATE_COPY_SOURCE;

	return ResourceState;
}

D3D12CommandList::D3D12CommandList(_In_ Device& DeviceObj, _In_ const CommandListType& Type, _In_ Pipeline* DefaultPipeline)
	: CommandList(DeviceObj, Type)
{
	D3D12CommandAllocator& D3D12CommandAllocatorObj = static_cast<D3D12CommandAllocator&>(*GetCommandAllocator());
	HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateCommandList(
		DeviceObj.GetDeviceMask(),
		D3D12_COMMAND_LIST_TYPES[Type],
		D3D12CommandAllocatorObj.GetD3D12CommandAllocator(),
		DefaultPipeline ? static_cast<D3D12Pipeline*>(DefaultPipeline)->GetD3D12PipelineState() : nullptr,
		__uuidof(ID3D12GraphicsCommandList),
		(void**)&_CommandList
	);
	ETERNAL_ASSERT(hr == S_OK);
	End();
}

D3D12CommandList::~D3D12CommandList()
{
	_CommandList->Release();
	_CommandList = nullptr;
}

//void D3D12CommandList::ClearRenderTarget(_In_ View& RenderTargetView)
//{
//	const FLOAT ClearColor[] = {
//		0.f, 0.f, 0.f, 0.f
//	};
//	_CommandList->ClearRenderTargetView(static_cast<D3D12View&>(RenderTargetView).GetCpuDescriptor(), ClearColor, 0, nullptr);
//}

void D3D12CommandList::DrawPrimitive(_In_ uint32_t PrimitiveCount)
{
	ETERNAL_ASSERT(PrimitiveCount);
	ETERNAL_ASSERT(!(PrimitiveCount % 3));

	_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_CommandList->DrawInstanced(PrimitiveCount, 1, 0, 0);
}

void D3D12CommandList::DrawIndexed(_In_ uint32_t IndicesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation)
{
	_CommandList->DrawIndexedInstanced(IndicesCount, 1, StartIndexLocation, BaseVertexLocation, 0);
}

void D3D12CommandList::DrawIndexedInstanced(_In_ uint32_t IndicesCount, _In_ uint32_t InstancesCount, _In_ uint32_t StartIndexLocation, _In_ int BaseVertexLocation, _In_ uint32_t StartInstanceLocation)
{
	_CommandList->DrawIndexedInstanced(IndicesCount, InstancesCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

void D3D12CommandList::SetViewport(_In_ const Viewport& ViewportObj)
{
	D3D12_VIEWPORT ViewportDesc;
	ViewportDesc.TopLeftX	= (FLOAT)ViewportObj.GetX();
	ViewportDesc.TopLeftY	= (FLOAT)ViewportObj.GetY();
	ViewportDesc.Width		= (FLOAT)ViewportObj.GetWidth();
	ViewportDesc.Height		= (FLOAT)ViewportObj.GetHeight();
	ViewportDesc.MinDepth	= 0.f;
	ViewportDesc.MaxDepth	= 1.f;
	_CommandList->RSSetViewports(1, &ViewportDesc);
}

void D3D12CommandList::SetScissorRectangle(_In_ const Viewport& ViewportObj)
{
	D3D12_RECT ScissorRectangle;
	ScissorRectangle.left	= ViewportObj.GetX();
	ScissorRectangle.top	= ViewportObj.GetY();
	ScissorRectangle.right	= ViewportObj.GetWidth();
	ScissorRectangle.bottom	= ViewportObj.GetHeight();
	_CommandList->RSSetScissorRects(1, &ScissorRectangle);
}

//void D3D12CommandList::BindRenderTarget(_In_ uint32_t Slot, _In_ View& RenderTargetView)
//{
//	const D3D12_CPU_DESCRIPTOR_HANDLE& CpuDescriptorHandle = static_cast<D3D12View&>(RenderTargetView).GetCpuDescriptor();
//	_CommandList->OMSetRenderTargets(1, &CpuDescriptorHandle, FALSE, nullptr);
//}
//
//void D3D12CommandList::BindConstant(_In_ uint32_t Slot, _In_ D3D12Constant& ConstantBuffer)
//{
//	//_CommandList->SetGraphicsRootConstantBufferView()
//}

void D3D12CommandList::Begin(_In_ Pipeline& PipelineObj)
{
	static_cast<D3D12CommandAllocator*>(GetCommandAllocator())->Reset();
	HRESULT hr = _CommandList->Reset(static_cast<D3D12CommandAllocator*>(GetCommandAllocator())->GetD3D12CommandAllocator(), static_cast<D3D12Pipeline&>(PipelineObj).GetD3D12PipelineState());
	ETERNAL_ASSERT(hr == S_OK);
	_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//OutputDebugString("beg\n");
}

void D3D12CommandList::Begin()
{
	static_cast<D3D12CommandAllocator*>(GetCommandAllocator())->Reset();
	HRESULT hr = _CommandList->Reset(static_cast<D3D12CommandAllocator*>(GetCommandAllocator())->GetD3D12CommandAllocator(), nullptr);
	ETERNAL_ASSERT(hr == S_OK);
	_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//OutputDebugString("beg\n");
}

void D3D12CommandList::End()
{
	HRESULT hr = _CommandList->Close();
	ETERNAL_ASSERT(hr == S_OK);
	//OutputDebugString("end\n");
}

void D3D12CommandList::BindPipelineInput(_In_ RootSignature& RootSignatureObj, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount)
{
	vector<ID3D12DescriptorHeap*> D3D12DescriptorHeaps;
	D3D12DescriptorHeaps.resize(DescriptorHeapsCount);

	for (uint32_t DescriptorHeapIndex = 0; DescriptorHeapIndex < DescriptorHeapsCount; ++DescriptorHeapIndex)
	{
		D3D12DescriptorHeaps[DescriptorHeapIndex] = static_cast<D3D12DescriptorHeap*>(DescriptorHeaps[DescriptorHeapIndex])->GetD3D12DescriptorHeap();
	}

	_CommandList->SetGraphicsRootSignature(static_cast<D3D12RootSignature&>(RootSignatureObj).GetD3D12RootSignature());
	_CommandList->SetDescriptorHeaps(D3D12DescriptorHeaps.size(), D3D12DescriptorHeaps.data());
	//for (uint32_t DescriptorHeapIndex = 0; DescriptorHeapIndex < DescriptorHeapsCount; ++DescriptorHeapIndex)
	//{
	//	if (static_cast<D3D12DescriptorHeap*>(DescriptorHeaps[DescriptorHeapIndex])->GetD3D12RootParameterType() == D3D12_ROOT_PARAMETER_TYPE_SRV)
	//	{
	//		D3D12_GPU_VIRTUAL_ADDRESS vaddr = static_cast<D3D12DescriptorHeap*>(DescriptorHeaps[DescriptorHeapIndex])->_GPUDescriptorHandleStart.ptr;
	//		_CommandList->SetGraphicsRootShaderResourceView(DescriptorHeapIndex, vaddr);
	//	}
	//}
}

void D3D12CommandList::BindConstantBuffer(_In_ uint32_t Slot, _In_ View& ConstantBuffer)
{
	_CommandList->SetGraphicsRootConstantBufferView(Slot, static_cast<D3D12View&>(ConstantBuffer).GetHandle().Gpu.ptr);
}

void D3D12CommandList::BindDescriptorTable(_In_ uint32_t Slot, _In_ DescriptorTable& DescriptorTableObj)
{
	_CommandList->SetGraphicsRootDescriptorTable(Slot, static_cast<D3D12DescriptorTable&>(DescriptorTableObj).GetHandle().Gpu);
}

void D3D12CommandList::BindBuffer(_In_ uint32_t Slot, _In_ View& Buffer)
{
	_CommandList->SetGraphicsRootShaderResourceView(Slot, static_cast<D3D12View&>(Buffer).GetHandle().Gpu.ptr);
}

void D3D12CommandList::BindUAV(_In_ uint32_t Slot, _In_ View& UAV)
{
	_CommandList->SetGraphicsRootUnorderedAccessView(Slot, static_cast<D3D12View&>(UAV).GetHandle().Gpu.ptr);
}

void D3D12CommandList::BeginRenderPass(_In_ RenderPass& RenderPassObj)
{
	D3D12RenderPass& D3D12RenderPassObj = static_cast<D3D12RenderPass&>(RenderPassObj);

	vector<D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargets;
	RenderTargets.resize(D3D12RenderPassObj.GetRenderTargets().size());
	for (uint32_t RenderTargetIndex = 0; RenderTargetIndex < D3D12RenderPassObj.GetRenderTargets().size(); ++RenderTargetIndex)
	{
		RenderTargets[RenderTargetIndex] = static_cast<D3D12View*>(D3D12RenderPassObj.GetRenderTargets()[RenderTargetIndex])->GetHandle().Cpu;
	}
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilCpuDescriptor;
	if (D3D12RenderPassObj.GetDepthStencilRenderTarget())
		DepthStencilCpuDescriptor = static_cast<const D3D12View*>(D3D12RenderPassObj.GetDepthStencilRenderTarget())->GetHandle().Cpu;

	_CommandList->OMSetRenderTargets(D3D12RenderPassObj.GetRenderTargets().size(), RenderTargets.data(), FALSE, D3D12RenderPassObj.GetDepthStencilRenderTarget() ? &DepthStencilCpuDescriptor : nullptr);
}

void D3D12CommandList::EndRenderPass()
{
}

void D3D12CommandList::SetIndicesBuffer(_In_ Resource* IndicesBuffer)
{
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	IndexBufferView.BufferLocation	= static_cast<D3D12Resource*>(IndicesBuffer)->GetD3D12Resource()->GetGPUVirtualAddress();
	IndexBufferView.Format			= DXGI_FORMAT_R16_UINT;
	IndexBufferView.SizeInBytes		= IndicesBuffer->GetBufferSize(); // 2-byte indices

	_CommandList->IASetIndexBuffer(&IndexBufferView);
}

void D3D12CommandList::SetVerticesBuffers(_In_ uint32_t StartSlot, _In_ uint32_t NumViews, _In_ Resource* VerticesBuffers[])
{
	vector<D3D12_VERTEX_BUFFER_VIEW> VerticesBufferViews;
	VerticesBufferViews.resize(NumViews);

	for (uint32_t ViewIndex = 0; ViewIndex < NumViews; ++ViewIndex)
	{
		VerticesBufferViews[ViewIndex].BufferLocation	= static_cast<D3D12Resource*>(VerticesBuffers[ViewIndex])->GetD3D12Resource()->GetGPUVirtualAddress();
		VerticesBufferViews[ViewIndex].SizeInBytes		= VerticesBuffers[ViewIndex]->GetBufferSize();
		VerticesBufferViews[ViewIndex].StrideInBytes	= VerticesBuffers[ViewIndex]->GetStride();
	}

	_CommandList->IASetVertexBuffers(StartSlot, NumViews, VerticesBufferViews.data());
}

void D3D12CommandList::Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount)
{
	const uint32_t ResourcesCount = BuffersCount + ImagesCount;

	vector<D3D12_RESOURCE_BARRIER> ResourceBarriers;
	ResourceBarriers.resize(ResourcesCount);
	
	/*
		D3D12_RESOURCE_BARRIER_TYPE Type;
	D3D12_RESOURCE_BARRIER_FLAGS Flags;
	union
		{
		D3D12_RESOURCE_TRANSITION_BARRIER Transition;
		D3D12_RESOURCE_ALIASING_BARRIER Aliasing;
		D3D12_RESOURCE_UAV_BARRIER UAV;
		} 	;
	} 	D3D12_RESOURCE_BARRIER;
	*/

		/*
			ID3D12Resource *pResource;
	UINT Subresource;
	D3D12_RESOURCE_STATES StateBefore;
	D3D12_RESOURCE_STATES StateAfter;*/

	uint32_t ResourceIndex = 0;
	for (uint32_t BufferIndex = 0; BufferIndex < BuffersCount; ++BufferIndex, ++ResourceIndex)
	{
		ResourceBarriers[ResourceIndex].Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		ResourceBarriers[ResourceIndex].Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
		ResourceBarriers[ResourceIndex].Transition.pResource	= static_cast<D3D12Resource*>(Buffers[BufferIndex].ResourceObj)->GetD3D12Resource();
		ResourceBarriers[ResourceIndex].Transition.Subresource	= 0;
		ResourceBarriers[ResourceIndex].Transition.StateBefore	= BuildResourceStates(Buffers[BufferIndex].Before);
		ResourceBarriers[ResourceIndex].Transition.StateAfter	= BuildResourceStates(Buffers[BufferIndex].After);
	}

	for (uint32_t ImageIndex = 0; ImageIndex < ImagesCount; ++ImageIndex, ++ResourceIndex)
	{
		ResourceBarriers[ResourceIndex].Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		ResourceBarriers[ResourceIndex].Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
		ResourceBarriers[ResourceIndex].Transition.pResource	= static_cast<D3D12Resource*>(Images[ImageIndex].ResourceObj)->GetD3D12Resource();
		ResourceBarriers[ResourceIndex].Transition.Subresource	= 0;
		ResourceBarriers[ResourceIndex].Transition.StateBefore	= BuildResourceStates(Images[ImageIndex].Before);
		ResourceBarriers[ResourceIndex].Transition.StateAfter	= BuildResourceStates(Images[ImageIndex].After);

		ETERNAL_ASSERT(static_cast<D3D12Resource*>(Images[ImageIndex].ResourceObj)->GetCurrentTransitionState() == Images[ImageIndex].Before);
		static_cast<D3D12Resource*>(Images[ImageIndex].ResourceObj)->GetCurrentTransitionState() = Images[ImageIndex].After;
	}

	_CommandList->ResourceBarrier(ResourcesCount, ResourceBarriers.data());
}

void D3D12CommandList::CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination)
{
	_Copy(Source, Destination);
}

void D3D12CommandList::_Copy(_In_ Resource& Source, _In_ Resource& Destination)
{
	_CommandList->CopyResource(static_cast<D3D12Resource&>(Destination).GetD3D12Resource(), static_cast<D3D12Resource&>(Source).GetD3D12Resource());
}

void D3D12CommandList::CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination, uint64_t SourceOffset, uint64_t DestinationOffset, uint64_t Size)
{
	_CommandList->CopyBufferRegion(static_cast<D3D12Resource&>(Destination).GetD3D12Resource(), DestinationOffset, static_cast<D3D12Resource&>(Source).GetD3D12Resource(), SourceOffset, Size);
}

void D3D12CommandList::CopyTexture(_In_ Resource& Source, _In_ Resource& Destination, _In_ const Position3D& SourcePosition, _In_ const Position3D& DestinationPosition, _In_ const Extent& Size)
{
	D3D12_TEXTURE_COPY_LOCATION DestinationCopyLocation;
	DestinationCopyLocation.pResource			= static_cast<D3D12Resource&>(Destination).GetD3D12Resource();
	DestinationCopyLocation.Type				= D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	DestinationCopyLocation.SubresourceIndex	= 0;

	D3D12_TEXTURE_COPY_LOCATION SourceCopyLocation;
	DestinationCopyLocation.pResource			= static_cast<D3D12Resource&>(Source).GetD3D12Resource();
	DestinationCopyLocation.Type				= D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	SourceCopyLocation.SubresourceIndex			= 0;
	
	D3D12_BOX SourceBox =
	{
		UINT(SourcePosition.X),					UINT(SourcePosition.Y),					UINT(SourcePosition.Z),
		UINT(SourcePosition.X + Size.Width),	UINT(SourcePosition.Y + Size.Height),	UINT(SourcePosition.Z + Size.Depth)
	};

	_CommandList->CopyTextureRegion(&DestinationCopyLocation, DestinationPosition.X, DestinationPosition.Y, DestinationPosition.Z, &SourceCopyLocation, &SourceBox);
}

void D3D12CommandList::CopyBufferToTexture(_In_ Resource& Buffer, _In_ Resource& Texture, uint64_t BufferOffset, uint64_t BufferSize, _In_ const Position3D& TexturePosition, _In_ const Extent& Size)
{
	D3D12_TEXTURE_COPY_LOCATION SourceCopyLocation;
	SourceCopyLocation.pResource							= static_cast<D3D12Resource&>(Buffer).GetD3D12Resource();
	SourceCopyLocation.Type									= D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	SourceCopyLocation.PlacedFootprint.Footprint.Width		= Size.Width;
	SourceCopyLocation.PlacedFootprint.Footprint.Height		= Size.Height;
	SourceCopyLocation.PlacedFootprint.Footprint.Depth		= Size.Depth;
	SourceCopyLocation.PlacedFootprint.Footprint.Format		= D3D12_FORMATS[static_cast<D3D12Resource&>(Buffer).GetFormat()];
	SourceCopyLocation.PlacedFootprint.Footprint.RowPitch	= Size.Width * D3D12_FORMAT_SIZES[static_cast<D3D12Resource&>(Buffer).GetFormat()];
	SourceCopyLocation.PlacedFootprint.Offset				= 0;

	D3D12_TEXTURE_COPY_LOCATION DestinationCopyLocation;
	DestinationCopyLocation.pResource						= static_cast<D3D12Resource&>(Texture).GetD3D12Resource();
	DestinationCopyLocation.Type							= D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	DestinationCopyLocation.SubresourceIndex				= 0;
	
	_CommandList->CopyTextureRegion(&DestinationCopyLocation, TexturePosition.X, TexturePosition.Y, TexturePosition.Z, &SourceCopyLocation, nullptr);
}
