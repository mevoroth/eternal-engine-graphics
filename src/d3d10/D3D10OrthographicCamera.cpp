#include "d3d10/D3D10OrthographicCamera.hpp"

#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D10OrthographicCamera::D3D10OrthographicCamera()
	: _model(XMMatrixIdentity())
	, _view(XMMatrixLookAtLH(
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMVectorSet(0.f, 1.f, 0.f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 1.f)
	))
	, _proj(XMMatrixOrthographicOffCenterLH(
		-Device::WIDTH / 2, Device::WIDTH / 2,
		Device::HEIGHT / 2, -Device::HEIGHT / 2,
		0.f, 1000.f
	))
{
}

Matrix4x4 D3D10OrthographicCamera::GetProjectionMatrix()
{
	return XMMatrixTranspose(_proj);
}

Matrix4x4 D3D10OrthographicCamera::GetViewMatrix()
{
	return XMMatrixTranspose(_view * _model);
}

void D3D10OrthographicCamera::SetModelMatrix(_In_ const Matrix4x4& model)
{
	_model = model;
}
