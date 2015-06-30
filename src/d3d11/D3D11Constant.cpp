#include "d3d11/D3D11Constant.hpp"

#include "Macros/Macros.hpp"
#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11Constant::D3D11Constant(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode)
	: D3D11Buffer(BufferSize, UsageObj, CPUMode, (Bind)D3D11_BIND_CONSTANT_BUFFER)
{
}

D3D11Constant::D3D11Constant(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, void* Data)
	: D3D11Buffer(BufferSize, UsageObj, CPUMode, (Bind)D3D11_BIND_CONSTANT_BUFFER, Data)
{
}
