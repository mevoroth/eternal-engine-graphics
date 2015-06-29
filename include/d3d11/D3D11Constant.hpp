#ifndef _D3D11_CONSTANT_HPP_
#define _D3D11_CONSTANT_HPP_

#include "Graphics/Constant.hpp"
#include "d3d11/D3D11Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Constant
			: public Constant
			, public D3D11Resource
		{
			D3D11Constant(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode);
			D3D11Constant(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, void* Data);
		};
	}
}

#endif
