#include "d3d10/D3D10PerspectiveCamera.hpp"

#include <cstdio>
using namespace std;
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D10PerspectiveCamera::D3D10PerspectiveCamera()
	: _Model(NewIdentity())
{
	XMMATRIX TempMatrix = XMMatrixLookToLH(
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f),
		XMVectorSet(0.f, 1.f, 0.f, 0.f)
		);
	XMStoreFloat4x4(&_View, TempMatrix);

	TempMatrix = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(90.f),
		Device::WIDTH / Device::HEIGHT,
		0.1f, 10000.f
		);
	XMStoreFloat4x4(&_Proj, TempMatrix);
}

Matrix4x4 D3D10PerspectiveCamera::GetProjectionMatrix()
{
	XMMATRIX ProjMatrix = XMMatrixTranspose(XMLoadFloat4x4(&_Proj));
	Matrix4x4 ReturnMatrix;
	XMStoreFloat4x4(&ReturnMatrix, ProjMatrix);
	return ReturnMatrix;
}

Matrix4x4 D3D10PerspectiveCamera::GetViewMatrix()
{
	XMMATRIX ViewMatrix = XMMatrixTranspose(XMLoadFloat4x4(&_View)) * XMMatrixTranspose(XMLoadFloat4x4(&_Model));
	Matrix4x4 ReturnMatrix;
	XMStoreFloat4x4(&ReturnMatrix, ViewMatrix);
	return ReturnMatrix;
}

void D3D10PerspectiveCamera::SetModelMatrix(_In_ const Matrix4x4& model)
{
	_Model = model;
	//char test[256];
	//XMVECTOR v = XMVector4Transform(XMVectorSet(0.f, 0.f, 0.f, 1.f), model);
	//sprintf_s(test, 256, "%f %f %f\n", XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

	//OutputDebugString(test);
}
