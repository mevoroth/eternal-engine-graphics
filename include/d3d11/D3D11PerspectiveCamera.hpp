#ifndef _D3D11_PERSPECTIVE_CAMERA_HPP_
#define _D3D11_PERSPECTIVE_CAMERA_HPP_

#include "D3D11Camera.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11PerspectiveCamera : public D3D11Camera
		{
		public:
			D3D11PerspectiveCamera();
			virtual void GetProjectionMatrix(_Out_ XMMATRIX* matrix);
			virtual void GetViewMatrix(_Out_ XMMATRIX* matrix);
		};
	}
}

#endif
