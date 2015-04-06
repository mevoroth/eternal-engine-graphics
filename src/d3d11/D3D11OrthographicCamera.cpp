#include "d3d11/D3D11OrthographicCamera.hpp"

#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D11OrthographicCamera::D3D11OrthographicCamera()
	: _model(NewIdentity())
	, _view(XMMatrixLookToLH(
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
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

Matrix4x4 D3D11OrthographicCamera::GetProjectionMatrix()
{
	return XMMatrixTranspose(_proj);
}

Matrix4x4 D3D11OrthographicCamera::GetViewMatrix()
{
	return XMMatrixTranspose(_view * _model);
}

void D3D11OrthographicCamera::SetModelMatrix(_In_ const Matrix4x4& model)
{
	_model = model;
}
