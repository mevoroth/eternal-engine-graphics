#include "d3d11/D3D11PosUVNormalVertexBuffer.hpp"

using namespace Eternal::Graphics;

size_t D3D11PosUVNormalVertexBuffer::_Size = sizeof(PosUVNormalVertex);

Eternal::Graphics::D3D11PosUVNormalVertexBuffer::D3D11PosUVNormalVertexBuffer(_In_ vector<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex>& Vertex)
	: _Vertex(Vertex)
	, D3D11VertexBuffer(Vertex.size() * sizeof(PosUVNormalVertex), (void*)&Vertex[0])
{
}

size_t D3D11PosUVNormalVertexBuffer::GetSize() const
{
	return _Size;
}

size_t D3D11PosUVNormalVertexBuffer::GetVerticesCount() const
{
	return _Vertex.size();
}
