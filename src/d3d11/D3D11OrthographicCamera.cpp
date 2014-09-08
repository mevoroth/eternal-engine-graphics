#include "d3d11/D3D11OrthographicCamera.hpp"

#include "d3d11/D3D11Device.hpp"
#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D11OrthographicCamera::D3D11OrthographicCamera()
{
	SetFocus(XMVectorSet(0.f, 1.f, 0.f, 1.f));
	SetUp(XMVectorSet(0.f, 0.f, 1.f, 1.f));
	SetPosition(XMVectorSet(0.f, 0.f, 0.f, 1.f));
	assert(!XMVector3Equal(GetFocus(), XMVectorZero()));
	assert(!XMVector3Equal(GetUp(), XMVectorZero()));
}

void D3D11OrthographicCamera::GetProjectionMatrix(_Out_ XMMATRIX* matrix)
{
	//*matrix = XMMatrixOrthographicOffCenterLH(
	//	0.f, D3D11DeviceType::WIDTH,
	//	D3D11DeviceType::HEIGHT, 0.f,
	//	0.f, 1000.f
	//);
	*matrix = XMMatrixTranspose(
		XMMatrixOrthographicOffCenterLH(
			0.f, D3D11DeviceType::WIDTH,
			D3D11DeviceType::HEIGHT, 0.f,
			0.f, 1000.f
		)
		//XMMatrixOrthographicLH(
		//	D3D11DeviceType::WIDTH,
		//	D3D11DeviceType::HEIGHT,
		//	0.f,
		//	1000.f
		//)
	);
}

void D3D11OrthographicCamera::GetViewMatrix(_Out_ XMMATRIX* matrix)
{
	*matrix = XMMatrixTranspose(
		XMMatrixLookToLH(
			GetPosition(),
			GetFocus(),
			GetUp()
		)
	);
}
