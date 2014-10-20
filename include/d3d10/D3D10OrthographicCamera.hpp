#ifndef _D3D10_ORTHOGRAPHIC_CAMERA_HPP_
#define _D3D10_ORTHOGRAPHIC_CAMERA_HPP_

#include "GraphicsSettings.hpp"
#include "Types/Types.hpp"
#include "D3D10Camera.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Types;
		class D3D11OrthographicCamera : public D3D10Camera
		{
		private:
			Matrix4x4 _model;
			Matrix4x4 _view;
			Matrix4x4 _proj;
		public:
			D3D11OrthographicCamera();
			virtual Matrix4x4 GetProjectionMatrix();
			virtual Matrix4x4 GetViewMatrix();
			virtual void SetModelMatrix(_In_ const Matrix4x4& model);
		};
	}
}

#endif
