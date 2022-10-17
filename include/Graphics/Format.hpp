#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class Format
		{
			FORMAT_UNKNOWN				= 0,
			FORMAT_BGRA8888_UNORM,
			FORMAT_RGBA8888_UNORM,
			FORMAT_RGBA8888_SNORM,
			FORMAT_RGB111110_FLOAT,
			FORMAT_RGBA32323232_FLOAT,
			FORMAT_RGB323232_FLOAT,
			FORMAT_R32_FLOAT,
			FORMAT_R32_UINT,
			FORMAT_D32,
			FORMAT_DS328,
			FORMAT_RG328,
			FORMAT_RG3232_FLOAT,
			FORMAT_R16_UINT,
			FORMAT_R8_UNORM,
			FORMAT_COUNT,
			FORMAT_INVALID		= FORMAT_COUNT
		};

		bool IsDepthStencilFormat(_In_ const Format& InFormat);
		Format ConvertDepthStencilFormatToFormat(_In_ const Format& InFormat);
	}
}
