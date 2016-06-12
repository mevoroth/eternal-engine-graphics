#include "d3d11/D3D11PosColorVertexBuffer.hpp"

using namespace Eternal::Graphics;

size_t D3D11PosColorVertexBuffer::_Size = sizeof(PosColorVertex);

D3D11PosColorVertexBuffer::D3D11PosColorVertexBuffer(_In_ vector<D3D11PosColorVertexBuffer::PosColorVertex>& Vertex)
	: _Vertex(Vertex)
	, D3D11VertexBuffer(Vertex.size() * sizeof(PosColorVertex), (void*)&Vertex[0])
{
}

size_t D3D11PosColorVertexBuffer::GetSize() const
{
	return _Size;
}

size_t D3D11PosColorVertexBuffer::GetVerticesCount() const
{
	return _Vertex.size();
}
