#ifndef _D3D12_FORMAT_HPP_
#define _D3D12_FORMAT_HPP_

#include <d3d12.h>
#include "Graphics/Format.hpp"

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
			DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		};

		const uint32_t D3D12_FORMAT_SIZES[] =
		{
			4,
			4,
			16,
			4,
			12,
			8,
			4
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_FORMATS) == int32_t(Format::FORMAT_COUNT), "D3D12 Formats declaration not complete");
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_FORMAT_SIZES) == int32_t(Format::FORMAT_COUNT), "D3D12 Format Sizes declaration not complete");
	}
}

#endif
