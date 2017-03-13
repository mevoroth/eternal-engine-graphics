#include "d3d12/D3D12CommandList.hpp"

#include "Macros/Macros.hpp"
#include "Graphics/Viewport.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12State.hpp"
#include "d3d12/D3D12RenderTarget.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"

using namespace Eternal::Graphics;

D3D12CommandList::D3D12CommandList(_In_ D3D12Device& DeviceObj, _In_ D3D12CommandQueue& CommandQueue, _In_ D3D12State& State)
{
	D3D12CommandAllocator* D3D12CommandAllocatorObj = static_cast<D3D12CommandAllocator*>(CommandQueue.GetCommandAllocator(0));
	HRESULT hr = DeviceObj.GetD3D12Device()->CreateCommandList(
		DeviceObj.GetDeviceMask(),
		CommandQueue.GetCommandListType(),
		D3D12CommandAllocatorObj->GetD3D12CommandAllocator(),
		State.GetD3D12PipelineState(),
		__uuidof(ID3D12GraphicsCommandList),
		(void**)&_CommandList
	);
	ETERNAL_ASSERT(hr == S_OK);

	End();
}

void D3D12CommandList::ClearRenderTarget(_In_ D3D12RenderTarget& RenderTargetObj)
{
	const FLOAT ClearColor[] = {
		0.f, 0.f, 0.f, 0.f
	};
	_CommandList->ClearRenderTargetView(RenderTargetObj.GetCpuDescriptor(), ClearColor, 0, nullptr);
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
	ViewportDesc.TopLeftX = (FLOAT)ViewportObj.X();
	ViewportDesc.TopLeftY = (FLOAT)ViewportObj.Y();
	ViewportDesc.Width = (FLOAT)ViewportObj.Width();
	ViewportDesc.Height = (FLOAT)ViewportObj.Height();
	ViewportDesc.MinDepth = 0.f;
	ViewportDesc.MaxDepth = 1.f;
	_CommandList->RSSetViewports(1, &ViewportDesc);
}

void D3D12CommandList::SetScissorRectangle(_In_ Viewport& ViewportObj)
{
	D3D12_RECT ScissorRectangle;
	ScissorRectangle.left = ViewportObj.X();
	ScissorRectangle.top = ViewportObj.Y();
	ScissorRectangle.right = ViewportObj.Width();
	ScissorRectangle.bottom = ViewportObj.Height();
	_CommandList->RSSetScissorRects(1, &ScissorRectangle);
}

void D3D12CommandList::BindRenderTarget(_In_ uint32_t Slot, _In_ D3D12RenderTarget& RenderTargetObj)
{
	const D3D12_CPU_DESCRIPTOR_HANDLE& CpuDescriptorHandle = RenderTargetObj.GetCpuDescriptor();
	_CommandList->OMSetRenderTargets(1, &CpuDescriptorHandle, FALSE, nullptr);
}

void D3D12CommandList::BindConstant(_In_ uint32_t Slot, _In_ D3D12Constant& ConstantBuffer)
{
	//_CommandList->SetGraphicsRootConstantBufferView()
}

void D3D12CommandList::Begin(_In_ CommandAllocator& CommandAllocatorObj, _In_ D3D12State& StateObj)
{
	HRESULT hr = _CommandList->Reset(static_cast<D3D12CommandAllocator&>(CommandAllocatorObj).GetD3D12CommandAllocator(), StateObj.GetD3D12PipelineState());
	ETERNAL_ASSERT(hr == S_OK);
	_CommandList->SetGraphicsRootSignature(StateObj.GetD3D12RootSignature());
}

void D3D12CommandList::End()
{
	HRESULT hr = _CommandList->Close();
	ETERNAL_ASSERT(hr == S_OK);
}
