#include "d3d11/D3D11PerspectiveCamera.hpp"

#include <cstdio>
using namespace std;
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D11PerspectiveCamera::D3D11PerspectiveCamera()
	: _model(NewIdentity())
	, _view(XMMatrixLookToLH(
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f),
		XMVectorSet(0.f, 1.f, 0.f, 0.f)
	))
	//, _proj(XMMatrixPerspectiveOffCenterLH(
	//	0.f, 1.f,
	//	0.f, 1.f,
	//	.001f, 1000.f
	//))
	, _proj(XMMatrixPerspectiveFovLH(
		XMConvertToRadians(90.f),
		Device::WIDTH / Device::HEIGHT,
		0.1f, 10000.f
	))
	//, _proj(XMMatrixPerspectiveLH(
	//	1.f,
	//	1.f,
	//	0.001f, 1000.f
	//))
{
}

Matrix4x4 D3D11PerspectiveCamera::GetProjectionMatrix()
{
	return XMMatrixTranspose(_proj);
}

Matrix4x4 D3D11PerspectiveCamera::GetViewMatrix()
{
	return XMMatrixTranspose(_view) * XMMatrixTranspose(_model);
}

void D3D11PerspectiveCamera::SetModelMatrix(_In_ const Matrix4x4& model)
{
	_model = model;
	//char test[256];
	//XMVECTOR v = XMVector4Transform(XMVectorSet(0.f, 0.f, 0.f, 1.f), model);
	//sprintf_s(test, 256, "%f %f %f\n", XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

	//OutputDebugString(test);
}
