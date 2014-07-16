#include "D3D11OrthographicCamera.hpp"

#include "Graphics\Device.hpp"

using namespace Eternal::Graphics;

D3D11OrthographicCamera::D3D11OrthographicCamera()
{

}

void D3D11OrthographicCamera::GetProjectionMatrix(_Out_ XMMATRIX* matrix)
{
	*matrix = XMMatrixOrthographicLH(
		Device::get()->WIDTH,
		Device::get()->HEIGHT,
		0.f,
		1000.f
	);
}

void D3D11OrthographicCamera::GetViewMatrix(_Out_ XMMATRIX* matrix)
{
	*matrix = XMMatrixLookAtLH(
		GetPosition(),
		GetFocus(),
		GetUp()
	);
}
