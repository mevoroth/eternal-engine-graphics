#include "d3d11/D3D11PerspectiveCamera.hpp"

#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

D3D11PerspectiveCamera::D3D11PerspectiveCamera()
{
	//SetPosition(XMVectorSet(2.5f, -8.f, 2.5f, 1.f));
	//SetFocus(XMVectorSet(2.5f, 0.f, 2.5f, 1.f));
	//SetUp(XMVectorSet(0.f, 0.f, 1.f, 1.f));
	//assert(!XMVector3Equal(GetFocus(), XMVectorZero()));
	//assert(!XMVector3Equal(GetUp(), XMVectorZero()));
}

void D3D11PerspectiveCamera::GetProjectionMatrix(_Out_ XMMATRIX* matrix)
{
	*matrix = XMMatrixTranspose(
		XMMatrixPerspectiveFovLH(
			XM_PI / 3.f, 16.f/9.f,
			.001f, 5000.f
		)
	);
}

void D3D11PerspectiveCamera::GetViewMatrix(_Out_ XMMATRIX* matrix)
{
	//*matrix = XMMatrixTranspose(
	//	XMMatrixLookAtLH(
	//		GetPosition(),
	//		GetFocus(),
	//		GetUp()
	//	)
	//);
}
