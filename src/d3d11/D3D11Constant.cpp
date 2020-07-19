#include "d3d11/D3D11Constant.hpp"

#include <d3d11.h>

#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11Constant::D3D11Constant(_In_ size_t BufferSize, _In_ const Usage& UsageObj, _In_ const CPUAccess& CPUMode)
	: D3D11Buffer(BufferSize, UsageObj, CPUMode, (Bind)D3D11_BIND_CONSTANT_BUFFER)
{
}

D3D11Constant::D3D11Constant(_In_ size_t BufferSize, _In_ const Usage& UsageObj, _In_ const CPUAccess& CPUMode, _In_ void* Data)
	: D3D11Buffer(BufferSize, UsageObj, CPUMode, (Bind)D3D11_BIND_CONSTANT_BUFFER, Data)
{
}

Resource* D3D11Constant::GetAsResource()
{
	return this;
}
