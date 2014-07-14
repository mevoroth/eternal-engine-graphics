#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <DirectXMath.h>

using namespace DirectX;

namespace Eternal
{
	namespace Graphics
	{
		class Camera
		{
		private:
			XMVECTOR up;
			XMVECTOR pos;
			XMVECTOR look;

		public:
			Camera();
		};
	}
}

#endif
	