#ifndef _D3D11_VERTEX_BUFFER_HPP_
#define _D3D11_VERTEX_BUFFER_HPP_

#include "Graphics/VertexBuffer.hpp"
#include "D3D11Buffer.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11VertexBuffer
			: public D3D11Buffer
			, public VertexBuffer
		{
		public:
			D3D11VertexBuffer(size_t BufferSize, void* Data);
		};
	}
}

#endif
