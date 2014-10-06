#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "Types.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Types;
		class Camera
		{
		public:
			virtual Matrix4x4 GetProjectionMatrix() = 0;
			virtual Matrix4x4 GetViewMatrix() = 0;
			virtual void SetModelMatrix(_In_ const Matrix4x4& model) = 0;
		};
	}
}

#endif
