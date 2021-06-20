#include "d3d12_deprecated/D3D12RenderTarget.hpp"

#include <d3d12.h>

#include "d3d12_deprecated/D3D12Device.hpp"
#include "d3d12_deprecated/D3D12SwapChain.hpp"
#include "d3d12_deprecated/D3D12DescriptorHeap.hpp"

using namespace Eternal::Graphics;

D3D12RenderTarget::D3D12RenderTarget(_In_ ID3D12Resource* BackBuffer)
	: D3D12Resource(BackBuffer)
{
}
