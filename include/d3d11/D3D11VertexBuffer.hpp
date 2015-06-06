#ifndef _D3D11_VERTEX_BUFFER_HPP_
#define _D3D11_VERTEX_BUFFER_HPP_

#include "Graphics/VertexBuffer.hpp"
#include "D3D11Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11VertexBuffer
			: public D3D11Resource
			, public VertexBuffer
		{
		};
	}
}

#endif
