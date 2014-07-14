#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

//#include "Utils\"
#include "Utils\Transform.hpp"

using namespace Eternal::Utils;

namespace Eternal
{
	namespace Graphics
	{
		class Camera
		{
		private:
			Transform transform;

		public:
			Camera();
		};
	}
}

#endif
	