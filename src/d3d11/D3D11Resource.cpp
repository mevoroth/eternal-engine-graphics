#include "d3d11/D3D11Resource.hpp"

#ifdef ETERNAL_DEBUG
#include <cstdio>
#endif

#include <d3d11.h>

#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11Context.hpp"

using namespace Eternal::Graphics;

static const D3D11_MAP MAP_TYPES[] = {
	D3D11_MAP_READ,
	D3D11_MAP_WRITE,
	D3D11_MAP_READ_WRITE,
	D3D11_MAP_WRITE_DISCARD
};

D3D11Resource::D3D11Resource(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode)
	: D3D11Resource(BufferSize, UsageObj, CPUMode, BindMode, nullptr)
{
}

D3D11Resource::D3D11Resource(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, void* Data)
{
	ETERNAL_ASSERT(UsageObj != IMMUTABLE || Data != nullptr);
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

D3D11Resource::D3D11Resource(ID3D11Resource* ResourceObj)
	: _D3D11Resource(ResourceObj)
{
}

D3D11Resource::D3D11Resource()
{
}

D3D11Resource::~D3D11Resource()
{
	_D3D11Resource->Release();
	_D3D11Resource = nullptr;
}

UINT D3D11Resource::CPUAccessToD3D11(_In_ const CPUAccess& CPUMode)
{
	return ((CPUMode & READ) != 0 ? D3D11_CPU_ACCESS_READ : 0) | ((CPUMode & WRITE) != 0 ? D3D11_CPU_ACCESS_WRITE : 0);
}

void D3D11Resource::_CreateBuffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, const D3D11_SUBRESOURCE_DATA* SubResourceData)
{
	if (UsageObj < STAGING)
	{
		ETERNAL_ASSERT((CPUMode & READ) == 0);
	}
	if (UsageObj < DYNAMIC)
	{
		ETERNAL_ASSERT((CPUMode & WRITE) == 0);
	}
	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.ByteWidth = (UINT)BufferSize;
	BufferDesc.Usage = (D3D11_USAGE)UsageObj;
	BufferDesc.BindFlags = BindMode;
	BufferDesc.CPUAccessFlags = CPUAccessToD3D11(CPUMode);
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	HRESULT hr;
	ID3D11Buffer* D3D11Buffer;
	hr = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateBuffer(&BufferDesc, SubResourceData, &D3D11Buffer);
#ifdef ETERNAL_DEBUG
	if (hr != S_OK)
	{
		char errors[255];
		DWORD err = GetLastError();
		DWORD removedReason = static_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->GetDeviceRemovedReason();
		sprintf_s(errors, ETERNAL_ARRAYSIZE(errors), "ERROR %d:%x:%x\n", err, err, removedReason);
		OutputDebugString(errors);
		ETERNAL_ASSERT(false);
	}
#endif

	_D3D11Resource = D3D11Buffer;
	_CPUAccess = CPUMode;
}

D3D11Resource::LockedResource D3D11Resource::Lock(Context& ContextObj, const LockMode& LockingMode)
{
#ifdef ETERNAL_DEBUG
	switch (LockingMode)
	{
	case LockMode::LOCK_READ:
		ETERNAL_ASSERT(_CPUAccess & Resource::READ);
		break;

	case LOCK_WRITE:
	case LOCK_WRITE_DISCARD:
		ETERNAL_ASSERT(_CPUAccess & Resource::WRITE);
		break;

	case LOCK_READ_WRITE:
		ETERNAL_ASSERT(_CPUAccess & (Resource::READ | Resource::WRITE));
		break;
	}
#endif

	D3D11Context& D3D11ContexObj = static_cast<D3D11Context&>(ContextObj);
	LockedResource Resource;

	HRESULT hr = D3D11ContexObj.GetD3D11Context()->Map(_D3D11Resource, 0, MAP_TYPES[LockingMode], 0, (D3D11_MAPPED_SUBRESOURCE*)&Resource);
	ETERNAL_ASSERT(hr == S_OK);

	return Resource;
}
void D3D11Resource::Unlock(Context& ContextObj)
{
	D3D11Context& D3D11ContexObj = static_cast<D3D11Context&>(ContextObj);
	D3D11ContexObj.GetD3D11Context()->Unmap(_D3D11Resource, 0);
}

ID3D11Resource* D3D11Resource::GetD3D11Resource()
{
	return _D3D11Resource;
}

void D3D11Resource::_SetD3D11Resource(ID3D11Resource* D3D11ResourceObj)
{
	_D3D11Resource = D3D11ResourceObj;
}

void D3D11Resource::_SetCPUAccess(const CPUAccess& CPUMode)
{
	_CPUAccess = CPUMode;
}
