#include "d3d12/D3D12Context.hpp"

#include <d3d12.h>

#include "Macros/Macros.hpp"
#include "Graphics/Renderer.hpp"
#include "d3d12/D3D12Renderer.hpp"

using namespace Eternal::Graphics;

D3D12Context::D3D12Context()
{
	ID3D12Device* D3DDevice = (ID3D12Device*)0xF0;// = static_cast<D3D12Renderer*>(Renderer::Get())->GetDevice();

	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;

	CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	CommandQueueDesc.NodeMask = 0;

	HRESULT hr = D3DDevice->CreateCommandQueue(&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&_CommandQueue);
	ETERNAL_ASSERT(hr == S_OK);

	hr = D3DDevice->CreateCommandAllocator(CommandQueueDesc.Type, __uuidof(ID3D12CommandAllocator), (void**)&_CommandAllocator);
	ETERNAL_ASSERT(hr == S_OK);

	hr = D3DDevice->CreateFence(_FenceValue, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&_Fence);
	ETERNAL_ASSERT(hr == S_OK);
}
