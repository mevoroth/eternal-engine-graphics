#ifndef _D3D11_VERTEX_POS_NORM_TEX_HPP_
#define _D3D11_VERTEX_POS_NORM_TEX_HPP_

#include <DirectXMath.h>

#include "Graphics/Vertex.hpp"

using namespace DirectX;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11VertexPosNormTex : public Vertex
		{
		public:
			XMVECTOR Pos;
			XMFLOAT3 Norm;
			XMFLOAT2 Tex;
		};
	}
}

#endif
