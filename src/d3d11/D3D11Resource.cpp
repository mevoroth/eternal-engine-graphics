#include "d3d11/D3D11Resource.hpp"

#include <cstdint>
#include <d3d11.h>

#include "Macros/Macros.hpp"
#include "d3d11/D3D11Renderer.hpp"

using namespace Eternal::Graphics;

D3D11Resource::D3D11Resource(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode)
	: D3D11Resource(BufferSize, UsageObj, CPUMode, BindMode, nullptr)
{
}


D3D11Resource::D3D11Resource(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, void* Data)
{
	ETERNAL_ASSERT(UsageObj != IMMUTABLE || Data);
	if (Data)
	{
		D3D11_SUBRESOURCE_DATA SubResData;
		SubResData.pSysMem = Data;
		SubResData.SysMemPitch = 0;
		SubResData.SysMemSlicePitch = 0;
		_CreateBuffer(BufferSize, UsageObj, CPUMode, BindMode, &SubResData);
	}
	else
	{
		_CreateBuffer(BufferSize, UsageObj, CPUMode, BindMode, nullptr);
	}
}

void D3D11Resource::_CreateBuffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, const D3D11_SUBRESOURCE_DATA* SubResourceData)
{
	if (UsageObj < Usage::STAGING)
	{
		ETERNAL_ASSERT((CPUMode & READ) == 0);
	}
	if ((UsageObj & 0x1) == 0)
	{
		ETERNAL_ASSERT((CPUMode & WRITE) == 0);
	}
	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.ByteWidth = BufferSize;
	BufferDesc.Usage = (D3D11_USAGE)UsageObj;
	BufferDesc.BindFlags = BindMode;
	BufferDesc.CPUAccessFlags = (CPUMode & READ != 0 ? D3D11_CPU_ACCESS_READ : 0) | (CPUMode & WRITE != 0 ? D3D11_CPU_ACCESS_WRITE : 0);
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	HRESULT hr;
	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateBuffer(&BufferDesc, SubResourceData, &_D3D11Buffer);
	ETERNAL_ASSERT(hr == S_OK);

	_CPUAccess = CPUMode;
}

void* D3D11Resource::Lock(const CPUAccess& LockingMode)
{
	ETERNAL_ASSERT((LockingMode & _CPUAccess) != 0);
	return nullptr;
}
void D3D11Resource::Unlock()
{

}
