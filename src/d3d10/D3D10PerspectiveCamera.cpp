#include "d3d10/D3D10PerspectiveCamera.hpp"

#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D10PerspectiveCamera::D3D10PerspectiveCamera()
	: _model(NewIdentity())
	, _view(XMMatrixLookAtLH(
		XMVectorSet(0.f, -50.f, 0.f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 1.f),
		XMVectorSet(0.f, 1.f, 0.f, 1.f)
	))
	, _proj(XMMatrixPerspectiveOffCenterRH(
		-Device::WIDTH / 2, Device::WIDTH / 2,
		Device::HEIGHT / 2, -Device::HEIGHT / 2,
		1.f, 10000.f
	))
{
}

Matrix4x4 D3D10PerspectiveCamera::GetProjectionMatrix()
{
	return XMMatrixTranspose(_proj);
}

Matrix4x4 D3D10PerspectiveCamera::GetViewMatrix()
{
	return XMMatrixTranspose(_view * _model);
}

void D3D10PerspectiveCamera::SetModelMatrix(_In_ const Matrix4x4& model)
{
	_model = model;
}
