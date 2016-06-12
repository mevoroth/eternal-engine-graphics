#include "d3d11/D3D11PosUVVertexBuffer.hpp"

using namespace Eternal::Graphics;

size_t D3D11PosUVVertexBuffer::_Size = sizeof(PosUVVertex);

Eternal::Graphics::D3D11PosUVVertexBuffer::D3D11PosUVVertexBuffer(_In_ vector<D3D11PosUVVertexBuffer::PosUVVertex>& Vertex)
	: _Vertex(Vertex)
	, D3D11VertexBuffer(Vertex.size() * sizeof(PosUVVertex), (void*)&Vertex[0])
{
}

size_t D3D11PosUVVertexBuffer::GetSize() const
{
	return _Size;
}

size_t D3D11PosUVVertexBuffer::GetVerticesCount() const
{
	return _Vertex.size();
}
