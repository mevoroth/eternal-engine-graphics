#include "d3d12/D3D12CommandList.hpp"

#include <d3d12.h>

#include "d3d12/D3D12Renderer.hpp"
#include "d3d12/D3D12Context.hpp"

using namespace Eternal::Graphics;

D3D12CommandList::D3D12CommandList(D3D12Context* ContextObj)
{
	//D3D12Context
	//D3D12Renderer* renderer = static_cast<D3D12Renderer*>(Renderer::Get());

	//renderer->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, ContextObj->GetCommandAllocator(), )
}
