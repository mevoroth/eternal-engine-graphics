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
			virtual Matrix4x4 GetProjectionMatrix();
			virtual Matrix4x4 GetViewMatrix();
			virtual void SetModelMatrix(_In_ const Matrix4x4& model);
		};
	}
}

#endif
