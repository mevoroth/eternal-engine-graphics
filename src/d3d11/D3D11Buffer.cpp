#include "d3d11/D3D11Buffer.hpp"

#include <d3d11.h>

using namespace Eternal::Graphics;

D3D11Buffer::D3D11Buffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode)
	: D3D11Resource(BufferSize, UsageObj, CPUMode, BindMode)
{
}
D3D11Buffer::D3D11Buffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, void* Data)
	: D3D11Resource(BufferSize, UsageObj, CPUMode, BindMode, Data)
{
}

D3D11Buffer::D3D11Buffer(ID3D11Buffer* D3D11BufferObj)
	: D3D11Resource(D3D11BufferObj)
{
}

ID3D11Buffer* D3D11Buffer::GetD3D11Buffer()
{
	return (ID3D11Buffer*)GetD3D11Resource();
}
