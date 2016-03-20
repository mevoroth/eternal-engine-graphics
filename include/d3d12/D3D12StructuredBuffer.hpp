#ifndef _D3D12_STRUCTURED_BUFFER_HPP_
#define _D3D12_STRUCTURED_BUFFER_HPP_

#include <cstdint>
#include "d3d12/D3D12Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;

		class D3D12StructuredBuffer
			: public D3D12Resource
		{
		public:
			D3D12StructuredBuffer(_In_ D3D12Device& DeviceObj, _In_ uint32_t Stride, _In_ uint32_t Count);
		};
	}
}

#endif
