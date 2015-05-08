#include "d3d10/D3D10OrthographicCamera.hpp"

#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D10OrthographicCamera::D3D10OrthographicCamera()
	: _Model(NewIdentity())
{
	XMMATRIX TempMatrix = XMMatrixLookToLH(
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f),
		XMVectorSet(0.f, 1.f, 0.f, 0.f)
		);
	XMStoreFloat4x4(&_View, TempMatrix);

	TempMatrix = XMMatrixOrthographicOffCenterLH(
		0.f, 1.f,
		0.f, 1.f,
		0.f, 1000.f
		);
	XMStoreFloat4x4(&_Proj, TempMatrix);
}

Matrix4x4 D3D10OrthographicCamera::GetProjectionMatrix()
{
	XMMATRIX ProjMatrix = XMMatrixTranspose(XMLoadFloat4x4(&_Proj));
	Matrix4x4 ReturnMatrix;
	XMStoreFloat4x4(&ReturnMatrix, ProjMatrix);
	return ReturnMatrix;
}

Matrix4x4 D3D10OrthographicCamera::GetViewMatrix()
{
	XMMATRIX ViewMatrix = XMMatrixTranspose(XMLoadFloat4x4(&_View)) * XMMatrixTranspose(XMLoadFloat4x4(&_Model));
	Matrix4x4 ReturnMatrix;
	XMStoreFloat4x4(&ReturnMatrix, ViewMatrix);
	return ReturnMatrix;
}

void D3D10OrthographicCamera::SetModelMatrix(_In_ const Matrix4x4& model)
{
	_Model = model;
}
