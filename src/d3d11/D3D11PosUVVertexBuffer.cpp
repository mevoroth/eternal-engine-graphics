#include "d3d11/D3D11PosUVVertexBuffer.hpp"

using namespace Eternal::Graphics;

size_t D3D11PosUVVertexBuffer::_Size = sizeof(PosUVVertex);

D3D11PosUVVertexBuffer::D3D11PosUVVertexBuffer(vector<PosUVVertex>& Vertex)
	: _Vertex(Vertex)
	, D3D11VertexBuffer(Vertex.size() * sizeof(PosUVVertex), (void*)&Vertex[0])
{
}

size_t D3D11PosUVVertexBuffer::GetSize() const
{
	return _Size;
}
