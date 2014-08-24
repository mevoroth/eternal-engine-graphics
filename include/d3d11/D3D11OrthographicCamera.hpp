#ifndef _D3D11_ORTHOGRAPHIC_CAMERA_HPP_
#define _D3D11_ORTHOGRAPHIC_CAMERA_HPP_

#include "D3D11Camera.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11OrthographicCamera : public D3D11Camera
		{
		public:
			D3D11OrthographicCamera();
			virtual void GetProjectionMatrix(_Out_ XMMATRIX* matrix);
			virtual void GetViewMatrix(_Out_ XMMATRIX* matrix);
		};
	}
}

#endif
