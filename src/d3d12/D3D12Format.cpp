#include "d3d12/D3D12Format.hpp"

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		static const D3D12Format D3D12_FORMATS[] = {
			D3D12Format(DXGI_FORMAT_UNKNOWN,					0),
			D3D12Format(DXGI_FORMAT_B8G8R8A8_UNORM,				4),
			D3D12Format(DXGI_FORMAT_R8G8B8A8_UNORM,				4),
			D3D12Format(DXGI_FORMAT_R32G32B32A32_FLOAT,			16),
			D3D12Format(DXGI_FORMAT_R32_FLOAT,					4),
			D3D12Format(DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,	12),
			D3D12Format(DXGI_FORMAT_R32G32_FLOAT,				8),
			D3D12Format(DXGI_FORMAT_D32_FLOAT,					4),
			D3D12Format(DXGI_FORMAT_R16_UINT,					2)
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_FORMATS) == int32_t(Format::FORMAT_COUNT), "D3D12 Formats declaration not complete");

		const D3D12Format& ConvertFormatToD3D12Format(const Format& InFormat)
		{
			return D3D12_FORMATS[static_cast<int32_t>(InFormat)];
		}
	}
}
