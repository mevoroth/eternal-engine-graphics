#pragma once

#include "Graphics/Format.hpp"

enum DXGI_FORMAT;

namespace Eternal
{
	namespace Graphics
	{
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

		extern const D3D12Format D3D12_FORMATS[];
	}
}
