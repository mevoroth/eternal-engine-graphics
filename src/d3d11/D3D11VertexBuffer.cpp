#include "d3d11/D3D11VertexBuffer.hpp"

using namespace Eternal::Graphics;

D3D11VertexBuffer::D3D11VertexBuffer(size_t BufferSize, void* Data)
	: D3D11Buffer(BufferSize, DYNAMIC, (CPUAccess)0, VERTEX_BUFFER, Data)
{
}
