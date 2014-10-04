#include "d3d11/D3D11OrthographicCamera.hpp"

#include "d3d11/D3D11Device.hpp"
#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D11OrthographicCamera::D3D11OrthographicCamera()
	: _model(XMMatrixIdentity())
	, _view(XMMatrixLookAtLH(
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMVectorSet(0.f, 1.f, 0.f, 1.f),
		XMVectorSet(0.f, 0.f, 1.f, 1.f)
	))
	, _proj(XMMatrixOrthographicOffCenterLH(
		-D3D11DeviceType::WIDTH / 2, D3D11DeviceType::WIDTH / 2,
		D3D11DeviceType::HEIGHT / 2, -D3D11DeviceType::HEIGHT / 2,
		0.f, 1000.f
	))
{
}

void D3D11OrthographicCamera::GetProjectionMatrix(_Out_ Matrix4x4* matrix)
{
	matrix->mat = XMMatrixTranspose(_proj);
}

void D3D11OrthographicCamera::GetViewMatrix(_Out_ Matrix4x4* matrix)
{
	matrix->mat = XMMatrixTranspose(_view * _model);
}
