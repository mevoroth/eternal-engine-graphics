#ifndef _D3D11_INDEX_BUFFER_HPP_
#define _D3D11_INDEX_BUFFER_HPP_

#include "Graphics/IndexBuffer.hpp"
#include "D3D11Resource.hpp"

enum DXGI_FORMAT;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11IndexBuffer
			: public D3D11Resource
			, public IndexBuffer
		{
		public:
			D3D11IndexBuffer(size_t BufferSize, void* Data);
			DXGI_FORMAT GetD3D11Format() const;
		};
	}
}

#endif
