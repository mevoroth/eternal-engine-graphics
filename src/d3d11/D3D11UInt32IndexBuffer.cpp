#include "d3d11/D3D11UInt32IndexBuffer.hpp"

using namespace Eternal::Graphics;

D3D11UInt32IndexBuffer::D3D11UInt32IndexBuffer(vector<uint32_t>& Indices)
	: _Indices(Indices)
	, D3D11IndexBuffer(Indices.size() * sizeof(uint32_t), (void*)&Indices[0])
{
}

uint32_t D3D11UInt32IndexBuffer::GetCount() const
{
	return _Indices.size();
}