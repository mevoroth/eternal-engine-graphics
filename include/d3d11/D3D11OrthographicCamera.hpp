#ifndef _D3D11_ORTHOGRAPHIC_CAMERA_HPP_
#define _D3D11_ORTHOGRAPHIC_CAMERA_HPP_

#include "D3D11Camera.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11OrthographicCamera : public D3D11Camera
		{
		private:
			XMMATRIX _model;
			XMMATRIX _view;
			XMMATRIX _proj;
		public:
			D3D11OrthographicCamera();
			virtual void GetProjectionMatrix(_Out_ Matrix4x4* matrix);
			virtual void GetViewMatrix(_Out_ Matrix4x4* matrix);
			void translate(float x, float y, float z)
			{
				//_model
			}
		};
	}
}

#endif
