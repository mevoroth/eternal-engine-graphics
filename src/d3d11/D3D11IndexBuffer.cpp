#include "d3d11/D3D11IndexBuffer.hpp"

#include <d3d11.h>

using namespace Eternal::Graphics;

D3D11IndexBuffer::D3D11IndexBuffer(size_t BufferSize, void* Data)
	: D3D11Resource(BufferSize, DYNAMIC, (CPUAccess)0, INDEX_BUFFER, Data)
{
}

DXGI_FORMAT D3D11IndexBuffer::GetD3D11Format() const
{
	return DXGI_FORMAT_R32_UINT;
}
