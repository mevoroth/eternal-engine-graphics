#pragma once

#if ETERNAL_ENABLE_D3D12

#include "Graphics/Format.hpp"

enum DXGI_FORMAT;

namespace Eternal
{
	namespace Graphics
	{
		enum class Format;

		struct D3D12Format
		{
			D3D12Format(const DXGI_FORMAT& InFormat, uint32_t InSize)
				: Format(InFormat)
				, Size(InSize)
			{
			}

			DXGI_FORMAT	Format;
			uint32_t	Size;
		};

		const D3D12Format& ConvertFormatToD3D12Format(const Format& InFormat);
	}
}

#endif
