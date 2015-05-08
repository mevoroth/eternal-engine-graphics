#ifndef _D3D11_ORTHOGRAPHIC_CAMERA_HPP_
#define _D3D11_ORTHOGRAPHIC_CAMERA_HPP_

#include "GraphicsSettings.hpp"
#include "Types/Types.hpp"
#include "D3D11Camera.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Types;
		class D3D11OrthographicCamera : public D3D11Camera
		{
		private:
			Matrix4x4 _Model;
			Matrix4x4 _View;
			Matrix4x4 _Proj;
		public:
			D3D11OrthographicCamera();
			virtual Matrix4x4 GetProjectionMatrix() override;
			virtual Matrix4x4 GetViewMatrix() override;
			virtual void SetModelMatrix(_In_ const Matrix4x4& model) override;
		};
	}
}

#endif
