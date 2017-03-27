#ifndef _D3D12_FORMAT_HPP_
#define _D3D12_FORMAT_HPP_

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		const DXGI_FORMAT D3D12_FORMATS[] =
		{
			DXGI_FORMAT_B8G8R8A8_UNORM,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS
		};
	}
}

#endif
