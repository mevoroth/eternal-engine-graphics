#ifndef _D3D11_CAMERA_HPP_
#define _D3D11_CAMERA_HPP_

#include <DirectXMath.h>
#include "Graphics\Camera.hpp"

using namespace DirectX;

namespace Eternal
{
	namespace Graphics
	{
		class Matrix4x4
		{
		public:
			XMMATRIX mat;
		};
		typedef Camera D3D11Camera;
	}
}

#endif
