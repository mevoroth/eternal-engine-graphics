#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Matrix4x4;
		class Camera
		{
		public:
			virtual void GetProjectionMatrix(_Out_ Matrix4x4* matrix) = 0;
			virtual void GetViewMatrix(_Out_ Matrix4x4* matrix) = 0;
			//virtual void SetPosition(_In_ const Vector4& v) { _pos = v; };
			//virtual void SetFocus(_In_ const Vector4& v) { _focus = v; };
			//virtual void SetUp(_In_ const Vector4& v) { _up = v; };
			//inline virtual Vector4 GetPosition() const { return _pos; };
			//inline virtual Vector4 GetFocus() const { return _focus; };
			//inline virtual Vector4 GetUp() const { return _up; };
		};
	}
}

#endif
