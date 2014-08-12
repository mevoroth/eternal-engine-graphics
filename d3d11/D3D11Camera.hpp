#ifndef _D3D11_CAMERA_HPP_
#define _D3D11_CAMERA_HPP_

#include <DirectXMath.h>
#include "Graphics\Camera.hpp"

using namespace DirectX;

namespace Eternal
{
	namespace Graphics
	{
		typedef Camera<XMVECTOR, XMMATRIX> D3D11Camera;
	}
}

#endif
