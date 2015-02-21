#include "d3d10/D3D10OrthographicCamera.hpp"

#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D10OrthographicCamera::D3D10OrthographicCamera()
	: _model(NewIdentity())
	, _view(XMMatrixLookToLH(
		XMVectorSet(0.f, 0.f, 0.f, 0.f),
		XMVectorSet(0.f, 0.f, 1.f, 0.f),
		XMVectorSet(0.f, 1.f, 0.f, 0.f)
	))
	, _proj(XMMatrixOrthographicOffCenterLH(
		0.f, 1.f,
		0.f, 1.f,
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
	return XMMatrixTranspose(_view);
}

void D3D10OrthographicCamera::SetModelMatrix(_In_ const Matrix4x4& model)
{
	_model = model;
}
