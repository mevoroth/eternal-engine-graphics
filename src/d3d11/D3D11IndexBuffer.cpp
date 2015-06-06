#include "d3d11/D3D11IndexBuffer.hpp"

#include <d3d11.h>

using namespace Eternal::Graphics;

DXGI_FORMAT D3D11IndexBuffer::GetD3D11Format() const
{
	return DXGI_FORMAT_R32_UINT;
}
