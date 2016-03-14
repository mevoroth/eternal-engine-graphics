#ifndef _D3D12_RENDER_TARGET_HPP_
#define _D3D12_RENDER_TARGET_HPP_

#include "d3d12/D3D12Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;

		class D3D12RenderTarget
			: public D3D12Resource
		{
		public:
			D3D12RenderTarget(D3D12Device& Device);
		};
	}
}

#endif
