#include "d3d11/D3D11OrthographicCamera.hpp"

#include "d3d11/D3D11Device.hpp"
#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D11OrthographicCamera::D3D11OrthographicCamera()
{

}

void D3D11OrthographicCamera::GetProjectionMatrix(_Out_ XMMATRIX* matrix)
{
	*matrix = XMMatrixOrthographicLH(
		D3D11DeviceType::WIDTH,
		D3D11DeviceType::HEIGHT,
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
