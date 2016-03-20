#ifndef _D3D12_CONSTANT_HPP_
#define _D3D12_CONSTANT_HPP_

#include <cstdint>
#include "d3d12/D3D12Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;

		class D3D12Constant
			: public D3D12Resource
		{
		public:
			D3D12Constant(_In_ D3D12Device& DeviceObj, _In_ uint32_t Size);
		};
	}
}

#endif
