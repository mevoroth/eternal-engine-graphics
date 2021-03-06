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
			FORMAT_RGBA32323232_FLOAT,
			FORMAT_R32_FLOAT,
			FORMAT_RG328,
			FORMAT_RG3232_FLOAT,
			FORMAT_D32,
			FORMAT_R16_UINT,
			FORMAT_COUNT,
			FORMAT_INVALID		= FORMAT_COUNT
		};
	}
}
