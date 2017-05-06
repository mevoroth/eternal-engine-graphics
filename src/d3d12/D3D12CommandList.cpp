#include "d3d12/D3D12CommandList.hpp"

#include "Macros/Macros.hpp"
#include "Graphics/Viewport.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "d3d12/D3D12RenderTarget.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "d3d12/D3D12View.hpp"
#include "d3d12/D3D12RootSignature.hpp"

using namespace Eternal::Graphics;

D3D12CommandList::D3D12CommandList(_In_ Device& DeviceObj, _In_ CommandAllocator& CommandAllocatorObj, _In_ Pipeline* DefaultPipeline)
{
	D3D12CommandAllocator& D3D12CommandAllocatorObj = static_cast<D3D12CommandAllocator&>(CommandAllocatorObj);
	//HRESULT hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateCommandList(
	//	DeviceObj.GetDeviceMask(),
	//	static_cast<D3D12CommandQueue&>(CommandQueueObj).GetCommandListType(),
	//	D3D12CommandAllocatorObj.GetD3D12CommandAllocator(),
	//	DefaultPipeline ? static_cast<D3D12Pipeline*>(DefaultPipeline)->GetD3D12PipelineState() : nullptr,
	//	__uuidof(ID3D12GraphicsCommandList),
	//	(void**)&_CommandList
	//);
	//ETERNAL_ASSERT(hr == S_OK);
	ETERNAL_ASSERT(false);
	End();
}

D3D12CommandList::~D3D12CommandList()
{
	_CommandList->Release();
	_CommandList = nullptr;
}

void D3D12CommandList::ClearRenderTarget(_In_ View& RenderTargetView)
{
	const FLOAT ClearColor[] = {
		0.f, 0.f, 0.f, 0.f
	};
	_CommandList->ClearRenderTargetView(static_cast<D3D12View&>(RenderTargetView).GetCpuDescriptor(), ClearColor, 0, nullptr);
}

void D3D12CommandList::DrawPrimitive(_In_ uint32_t PrimitiveCount)
{
	ETERNAL_ASSERT(PrimitiveCount);
	ETERNAL_ASSERT(!(PrimitiveCount % 3));

	_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_CommandList->DrawInstanced(PrimitiveCount, 1, 0, 0);
}

void D3D12CommandList::SetViewport(_In_ Viewport& ViewportObj)
{
	D3D12_VIEWPORT ViewportDesc;
	ViewportDesc.TopLeftX	= (FLOAT)ViewportObj.X();
	ViewportDesc.TopLeftY	= (FLOAT)ViewportObj.Y();
	ViewportDesc.Width		= (FLOAT)ViewportObj.Width();
	ViewportDesc.Height		= (FLOAT)ViewportObj.Height();
	ViewportDesc.MinDepth	= 0.f;
	ViewportDesc.MaxDepth	= 1.f;
	_CommandList->RSSetViewports(1, &ViewportDesc);
}

void D3D12CommandList::SetScissorRectangle(_In_ Viewport& ViewportObj)
{
	D3D12_RECT ScissorRectangle;
	ScissorRectangle.left	= ViewportObj.X();
	ScissorRectangle.top	= ViewportObj.Y();
	ScissorRectangle.right	= ViewportObj.Width();
	ScissorRectangle.bottom	= ViewportObj.Height();
	_CommandList->RSSetScissorRects(1, &ScissorRectangle);
}

void D3D12CommandList::BindRenderTarget(_In_ uint32_t Slot, _In_ View& RenderTargetView)
{
	const D3D12_CPU_DESCRIPTOR_HANDLE& CpuDescriptorHandle = static_cast<D3D12View&>(RenderTargetView).GetCpuDescriptor();
	_CommandList->OMSetRenderTargets(1, &CpuDescriptorHandle, FALSE, nullptr);
}

void D3D12CommandList::BindConstant(_In_ uint32_t Slot, _In_ D3D12Constant& ConstantBuffer)
{
	//_CommandList->SetGraphicsRootConstantBufferView()
}

void D3D12CommandList::Begin(_In_ CommandAllocator& CommandAllocatorObj, _In_ Pipeline& PipelineObj)
{
	HRESULT hr = _CommandList->Reset(static_cast<D3D12CommandAllocator&>(CommandAllocatorObj).GetD3D12CommandAllocator(), static_cast<D3D12Pipeline&>(PipelineObj).GetD3D12PipelineState());
	ETERNAL_ASSERT(hr == S_OK);
}

void D3D12CommandList::End()
{
	HRESULT hr = _CommandList->Close();
	ETERNAL_ASSERT(hr == S_OK);
}

void D3D12CommandList::BindPipelineInput(_In_ RootSignature& RootSignatureObj, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount)
{
	_CommandList->SetGraphicsRootSignature(static_cast<D3D12RootSignature&>(RootSignatureObj).GetD3D12RootSignature());
}

void D3D12CommandList::BeginRenderPass(_In_ RenderPass& RenderPassObj, RenderTarget& RenderTargetObj, _In_ Viewport& ViewportObj)
{
}

void D3D12CommandList::EndRenderPass()
{
}

void D3D12CommandList::Transition(_In_ ResourceTransition Buffers[], _In_ uint32_t BuffersCount, _In_ ResourceTransition Images[], _In_ uint32_t ImagesCount)
{

}

void D3D12CommandList::CopyBuffer(_In_ Resource& Source, _In_ Resource& Destination)
{
	Copy(Source, Destination);
}

void D3D12CommandList::Copy(_In_ Resource& Source, _In_ Resource& Destination)
{
	_CommandList->CopyResource(static_cast<D3D12Resource&>(Source).GetResource(), static_cast<D3D12Resource&>(Destination).GetResource());
}
