#ifndef _D3D11_BUFFER_HPP_
#define _D3D11_BUFFER_HPP_

#include "d3d11/D3D11Resource.hpp"

struct ID3D11Buffer;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Buffer : public D3D11Resource
		{
		public:
			D3D11Buffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode);
			D3D11Buffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, void* Data);
			ID3D11Buffer* GetD3D11Buffer();
		};
	}
}

#endif
