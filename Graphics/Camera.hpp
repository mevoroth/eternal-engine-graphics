#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

using namespace Eternal::Utils;

namespace Eternal
{
	namespace Graphics
	{
		template <typename Vector3, typename Matrix4x4> class Camera
		{
		private:
			Vector3 _pos;
			Vector3 _focus;
			Vector3 _up;
		public:
			virtual void GetProjectionMatrix(_Out_ Matrix4x4* matrix) = 0;
			virtual void GetViewMatrix(_Out_ Matrix4x4* matrix) = 0;
			virtual void SetPosition(_In_ const Vector3& v) = 0;
			virtual void SetFocus(_In_ const Vector3& v) = 0;
			virtual void SetUp(_In_ const Vector3& v) = 0;
		};
	}
}

#endif
	