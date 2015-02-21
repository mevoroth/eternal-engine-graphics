#include "d3d10/D3D10PerspectiveCamera.hpp"

#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D10PerspectiveCamera::D3D10PerspectiveCamera()
	: _model(NewIdentity())
	, _view(XMMatrixLookToLH(
		XMVectorSet(0.f, 0.f, 0.f, 0.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f),
		XMVectorSet(0.f, 1.f, 0.f, 0.f)
	))
	//, _proj(XMMatrixPerspectiveOffCenterLH(
	//	0.f, 1.f,
	//	0.f, 1.f,
	//	.001f, 1000.f
	//))
	, _proj(XMMatrixPerspectiveFovLH(
		90.f * 3.14159265359f / 180.f,
		Device::WIDTH / Device::HEIGHT,
		0.0001f, 1000.f
	))
	//, _proj(XMMatrixPerspectiveLH(
	//	1.f,
	//	1.f,
	//	0.001f, 1000.f
	//))
{
}

Matrix4x4 D3D10PerspectiveCamera::GetProjectionMatrix()
{
	return XMMatrixTranspose(_proj);
}

Matrix4x4 D3D10PerspectiveCamera::GetViewMatrix()
{
	return XMMatrixTranspose(_view);
}

void D3D10PerspectiveCamera::SetModelMatrix(_In_ const Matrix4x4& model)
{
	_model = model;
}
